#include "ClientSocket.h"
#include <stdlib.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

ClientSocket::ClientSocket()
{
	client_socket_ = INVALID_SOCKET;
	server_IP_addr_ = "127.0.0.1";
	server_port_ = "10085";
	running_flag_ = false;
	stop_flag_ = false;
}

ClientSocket::~ClientSocket()
{
	stop();
}

void ClientSocket::clean_mission_queue()
{
	while (mission_queue_.empty() == false)
		mission_queue_.pop();
}

bool ClientSocket::win_socket_init()
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

DWORD WINAPI ClientSocket::on_socket_running(LPVOID data)
{
	ClientSocket* socket_data = (ClientSocket*)data;
	if (ClientSocket::win_socket_init() == false)								//Windows socket dll初始化失败
	{
		socket_data->on_init_failed();
		socket_data->running_flag_ = false;
		return 0;
	}
	socket_data->client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_data->client_socket_ == INVALID_SOCKET)
	{
		socket_data->on_socket_create_failed(WSAGetLastError());
		WSACleanup();
		socket_data->running_flag_ = false;
		return 0;
	}
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(socket_data->server_IP_addr_.c_str());
	server_addr.sin_port = htons(atoi(socket_data->server_port_.c_str()));
	int WSA_error_code;
	if (connect(socket_data->client_socket_, (SOCKADDR*)&server_addr, sizeof(server_addr)) != 0)
	{
		WSA_error_code = WSAGetLastError();
		#ifdef _DEBUG
		printf("client connect failed.error code:%d.\n", WSA_error_code);
		#endif
		if (WSA_error_code != WSAEINTR)
			socket_data->on_connect_failed(socket_data->client_socket_, WSA_error_code);
		closesocket(socket_data->client_socket_);
		WSACleanup();
		socket_data->running_flag_ = false;
		return 0;
	}
	socket_data->on_connect_success(socket_data->client_socket_);
	socket_mission mission;
	while (true)
	{
		if (socket_data->stop_flag_ || socket_data->running_flag_ == false)
		{
			closesocket(socket_data->client_socket_);
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
			case MISSION_RECEIVE:
				int len;
				len = recv(socket_data->client_socket_, socket_data->receive_buf_, sizeof(socket_data->receive_buf_), 0);
				if (len <= 0)
				{
					WSA_error_code = WSAGetLastError();
					#ifdef _DEBUG
					printf("client receive failed.error code:%d.\n", WSA_error_code);
					#endif
					if (WSA_error_code != WSAEINTR && WSA_error_code != WSAESHUTDOWN)
						socket_data->on_receive_failed(socket_data->client_socket_, WSA_error_code);
				}
				else																				//注意要一次性读取完
				{
					socket_data->receive_buf_[len] = '\0';
					socket_data->on_receive_completed(socket_data->client_socket_, socket_data->receive_buf_, len);
				}
				break;
			case MISSION_SEND:
				if (send(socket_data->client_socket_, mission.send_str.c_str(),
					mission.send_str.length() + 1, 0) < 0)
				{
					WSA_error_code = WSAGetLastError();
					#ifdef _DEBUG
					printf("client send failed.error code:%d.\n", WSA_error_code);
					#endif
					if (WSA_error_code != WSAEINTR && WSA_error_code != WSAESHUTDOWN)
						socket_data->on_send_failed(socket_data->client_socket_, WSA_error_code);
				}
				break;
			default:
				break;
		}
		Sleep(30);
	}
	return 0;
}

void ClientSocket::send_stop_message()
{
	if (running_flag_)
	{
		if (client_socket_ != INVALID_SOCKET)
			shutdown(client_socket_, 2);
		stop_flag_ = true;
	}
}

void ClientSocket::start()
{
	if (running_flag_)
		stop();
	clean_mission_queue();
	running_flag_ = false;
	stop_flag_ = false;
	socket_thread_ = CreateThread(NULL, 0, on_socket_running, this, 0, NULL);
	running_flag_ = true;
}

void ClientSocket::stop()
{
	if (running_flag_)
	{
		shutdown(client_socket_, 2);
		stop_flag_ = true;
		while (stop_flag_ && running_flag_)
			Sleep(30);
		CloseHandle(socket_thread_);
		client_socket_ = INVALID_SOCKET;
	}
}

void ClientSocket::receive()
{
	if (running_flag_)
	{
		socket_mission recv_mission;
		recv_mission.mission_ID = MISSION_RECEIVE;
		mission_queue_.push(recv_mission);
	}
}

void ClientSocket::send_string(const char* str)
{
	if (running_flag_)
	{
		socket_mission send_mission;
		send_mission.mission_ID = MISSION_SEND;
		send_mission.send_str = str;
		mission_queue_.push(send_mission);
	}
}
