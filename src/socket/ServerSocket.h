#pragma once
#include <winsock.h>
#include <windows.h>
#include <string>
#include <stdio.h>
#include "../tools/ThreadQueue.h"
using namespace std;
class ServerSocket
{
private:
	SOCKET server_socket_;
	SOCKET connect_socket_;
	string server_port_;

	static const int max_buf_size_ = 1024;
	static const int max_mission_num_ = 1000;

	HANDLE socket_thread_;
	enum mission_type
	{
		MISSION_ACCEPT,
		MISSION_RECEIVE,
		MISSION_SEND,
		MISSION_STOP_CONNECTION
	};
	struct socket_mission
	{
		mission_type mission_ID;
		string send_str;												//仅当ID为MISSION_SEND时有效
	};
	ThreadQueue<socket_mission> mission_queue_;
	volatile bool running_flag_;
	volatile bool stop_flag_;
	char receive_buf_[max_buf_size_];

	static bool win_socket_init();
	static DWORD WINAPI on_socket_running(LPVOID data);
protected:
	virtual void on_init_failed(){}
	virtual void on_socket_create_failed(int WSA_error_code){}
	virtual void on_socket_bind_failed(SOCKET socket, int WSA_error_code){}
	virtual void on_socket_listen_success(SOCKET socket){}
	virtual void on_accept_failed(SOCKET server_socket, int WSA_error_code){}
	virtual void on_accept_success(SOCKET server_socket, SOCKET connect_socket){}
	virtual void on_receive_failed(SOCKET server_socket, SOCKET connect_socket, int WSA_error_code){}
	virtual void on_receive_completed(SOCKET server_socket, SOCKET connect_socket, char* str, int length){}
	virtual void on_send_failed(SOCKET server_socket, SOCKET connect_socket, const char* send_str, int WSA_error_code){}
	virtual void on_send_completed(SOCKET server_socket, SOCKET connect_socket){}
	void send_stop_message();
public:
	ServerSocket();
	virtual ~ServerSocket();

	void set_server_port(const char* port)
	{
		server_port_ = port;
	}

	bool is_run()
	{
		return running_flag_;
	}

	void clean_mission_queue();
	void start();
	void stop();										//停止socket守护线程，请勿在回调函数中使用。回调函数中请使用send_stop_message()函数
	void accept_new_connection();
	void stop_connection();
	void receive();
	void send_string(const char* str);
};

