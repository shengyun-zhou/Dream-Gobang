#include "ServerSocket.h"
#include <string.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

ServerSocket::ServerSocket()
{
	server_port_ = "10085";
	server_socket_ = INVALID_SOCKET;
	connect_socket_ = INVALID_SOCKET;
	running_flag_ = stop_flag_ = false;
	clean_mission_queue();
}

ServerSocket::~ServerSocket()
{
	stop();
}

void ServerSocket::clean_mission_queue()
{
	while (mission_queue_.empty() == false)
		mission_queue_.pop();
}

bool ServerSocket::win_socket_init()
{
	WORD version_request = MAKEWORD(1, 1);
	WSADATA wsa_data;
	if (WSAStartup(version_request, &wsa_data) != 0)
		return false;
	if (LOBYTE(wsa_data.wVersion) == 1 && HIBYTE(wsa_data.wVersion) == 1)
		return true;
	WSACleanup();
	return false;
}

DWORD WINAPI ServerSocket::on_socket_running(LPVOID data)
{
	ServerSocket* socket_data = (ServerSocket*)data;
	if (ServerSocket::win_socket_init() == false)								//Windows socket dll初始化失败
	{
		socket_data->on_init_failed();
		socket_data->running_flag_ = false;
		return 0;
	}
	socket_data->server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_data->server_socket_ == INVALID_SOCKET)
	{
		socket_data->on_socket_create_failed(WSAGetLastError());
		WSACleanup();
		socket_data->running_flag_ = false;
		return 0;
	}
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);								//服务端IP为0.0.0.0(即任意地址)
	server_addr.sin_port = htons(atoi(socket_data->server_port_.c_str()));
	int WSA_error_code;

	if (bind(socket_data->server_socket_, (SOCKADDR*)&server_addr, sizeof(server_addr)) != 0)
	{
		WSA_error_code = WSAGetLastError();
		#ifdef _DEBUG
		printf("server bind failed.error code:%d.\n", WSA_error_code);
		#endif
		if (WSA_error_code != WSAEINTR)
			socket_data->on_socket_bind_failed(socket_data->server_socket_, WSA_error_code);
		closesocket(socket_data->server_socket_);
		WSACleanup();
		socket_data->running_flag_ = false;
		return 0;
	}
	listen(socket_data->server_socket_, 5);
	socket_data->on_socket_listen_success(socket_data->server_socket_);
	socket_data->connect_socket_ = INVALID_SOCKET;

	SOCKADDR client_addr;
	int sockaddr_len = sizeof(SOCKADDR);

	socket_mission mission;
	while (true)
	{
		if (socket_data->stop_flag_ || socket_data->running_flag_ == false)
		{
			if (socket_data->connect_socket_ != INVALID_SOCKET)
				closesocket(socket_data->connect_socket_);
			closesocket(socket_data->server_socket_);
			WSACleanup();
			socket_data->stop_flag_ = false;
			socket_data->running_flag_ = false;
			return 0;
		}
		if (socket_data->mission_queue_.empty())
		{
			Sleep(30);
			continue;
		}
		mission = socket_data->mission_queue_.front();
		socket_data->mission_queue_.pop();
		switch (mission.mission_ID)
		{
			case MISSION_ACCEPT:
				if (socket_data->connect_socket_ != INVALID_SOCKET)
					closesocket(socket_data->connect_socket_);
				socket_data->connect_socket_ = accept(socket_data->server_socket_, &client_addr, &sockaddr_len);
				if (socket_data->connect_socket_ == INVALID_SOCKET)
				{
					WSA_error_code = WSAGetLastError();
					#ifdef _DEBUG
					printf("server accept failed.error code:%d.\n", WSA_error_code);
					#endif
					if (WSA_error_code != WSAEINTR && WSA_error_code != WSAESHUTDOWN)
						socket_data->on_accept_failed(socket_data->server_socket_, WSA_error_code);
				}
				else
					socket_data->on_accept_success(socket_data->server_socket_, socket_data->connect_socket_);
				break;
			case MISSION_RECEIVE:
				if (socket_data->connect_socket_ != INVALID_SOCKET)
				{
					int len;
					len = recv(socket_data->connect_socket_, socket_data->receive_buf_, sizeof(socket_data->receive_buf_), 0);
					if (len <= 0)
					{
						WSA_error_code = WSAGetLastError();
						#ifdef _DEBUG
						printf("server receive failed.error code:%d.\n", WSA_error_code);
						#endif
						if (WSA_error_code != WSAEINTR && WSA_error_code != WSAESHUTDOWN)
							socket_data->on_receive_failed(socket_data->server_socket_, socket_data->connect_socket_, WSA_error_code);
					}
					else
					{
						socket_data->receive_buf_[len] = '\0';
						socket_data->on_receive_completed(socket_data->server_socket_, socket_data->connect_socket_, socket_data->receive_buf_, len);
					}
				}
				break;
			case MISSION_SEND:
				if (socket_data->connect_socket_ != INVALID_SOCKET)
				{
					if (send(socket_data->server_socket_, mission.send_str.c_str(),
						mission.send_str.length() + 1, 0) < 0)
					{
						WSA_error_code = WSAGetLastError();
						#ifdef _DEBUG
						printf("server send failed.error code:%d.\n", WSA_error_code);
						#endif
						if (WSA_error_code != WSAEINTR && WSA_error_code != WSAESHUTDOWN)
							socket_data->on_send_failed(socket_data->server_socket_, socket_data->connect_socket_, WSA_error_code);
					}
				}
				break;
			case MISSION_STOP_CONNECTION:
				if (socket_data->connect_socket_ != INVALID_SOCKET)
					closesocket(socket_data->connect_socket_);
				break;
			default:
				break;
		}
		Sleep(30);
	}
	return 0;
}

void ServerSocket::send_stop_message()
{
	if (running_flag_)
	{
		if (connect_socket_ != INVALID_SOCKET)
			shutdown(connect_socket_, 2);
		stop_flag_ = true;
	}
}

void ServerSocket::start()
{
	if (running_flag_)
		stop();
	clean_mission_queue();
	socket_thread_ = CreateThread(NULL, 0, on_socket_running, this, 0, NULL);
	running_flag_ = true;
	stop_flag_ = false;
}

void ServerSocket::stop()
{
	if (running_flag_)
	{
		if (connect_socket_ != INVALID_SOCKET)
			shutdown(connect_socket_, 2);
		stop_flag_ = true;
		while (stop_flag_ && running_flag_)
			Sleep(30);
		CloseHandle(socket_thread_);
		server_socket_ = INVALID_SOCKET;
		connect_socket_ = INVALID_SOCKET;
	}
}

void ServerSocket::accept_new_connection()
{
	if (running_flag_)
	{
		socket_mission accept_mission;
		accept_mission.mission_ID = MISSION_ACCEPT;
		mission_queue_.push(accept_mission);
	}
}

void ServerSocket::receive()
{
	if (running_flag_)
	{
		socket_mission recv_mission;
		recv_mission.mission_ID = MISSION_RECEIVE;
		mission_queue_.push(recv_mission);
	}
}

void ServerSocket::send_string(const char* str)
{
	if (running_flag_)
	{
		socket_mission send_mission;
		send_mission.mission_ID = MISSION_SEND;
		send_mission.send_str = str;
		mission_queue_.push(send_mission);
	}
}

void ServerSocket::stop_connection()
{
	if (running_flag_)
	{
		socket_mission stop_connect_mission;
		stop_connect_mission.mission_ID = MISSION_STOP_CONNECTION;
		mission_queue_.push(stop_connect_mission);
	}
}
