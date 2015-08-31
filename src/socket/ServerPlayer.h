#pragma once
#include "ServerSocket.h"
#include "../Gobang.h"
#include "../Chess.h"
#include "ClientPlayer.h"
#include "../tools/ThreadQueue.h"

using namespace std;

class ServerPlayer : public ServerSocket
{
protected:
	virtual void on_init_failed();
	virtual void on_socket_create_failed(int WSA_error_code);
	virtual void on_socket_bind_failed(SOCKET socket, int WSA_error_code);
	virtual void on_socket_listen_success(SOCKET socket);
	virtual void on_accept_failed(SOCKET server_socket, int WSA_error_code);
	virtual void on_accept_success(SOCKET server_socket, SOCKET connect_socket);
	virtual void on_receive_failed(SOCKET server_socket, SOCKET connect_socket, int WSA_error_code);
	virtual void on_receive_completed(SOCKET server_socket, SOCKET connect_socket, char* str, int length);
	virtual void on_send_failed(SOCKET server_socket, SOCKET connect_socket, const char* send_str, int WSA_error_code);
	virtual void on_send_completed(SOCKET server_socket, SOCKET connect_socket);
public:
	ServerPlayer();
	virtual ~ServerPlayer();
	
	enum ServerError
	{
		ERROR_NONE,
		ERROR_WIN_SOCK_INIT_FAILED,
		ERROR_SOCKET_CREATE_FAILED,
		ERROR_SOCKET_BIND_FAILED,
		ERROR_ACCEPT_FAILED,
		ERROR_RECEIVE_FAILED,
		ERROR_SEND_FAILED,
	};

	enum ServerComplete
	{
		COMPLETE_NONE,
		COMPLETE_LISTEN,
		COMPLETE_ACCEPT,
		COMPLETE_RECEIVE,
		COMPLETE_SEND
	};

	enum MessageType
	{
		MESSAGE_NONE,
		MESSAGE_COMPLETE,
		MESSAGE_ERROR
	};

	enum OppositeActionType
	{
		ACTION_NONE,
		ACTION_PLAY_CHESS,
		ACTION_READY,
		ACTION_TELL_USER_NAME,
		ACTION_QUIT,
	};

	struct ServerMessage
	{
		MessageType msg_type;
		union MessageContent
		{
			ServerComplete complete_msg;
			ServerError error_msg;
		}msg_content;
		string receive_str;
	};

	struct ActionInfo
	{
		OppositeActionType action_type;
		union ExtraData
		{
			int chess_pos[2];
			char user_name[Gobang::USER_NAME_MAX_LEN + 1];
		}ex_data;
	};
	
	void start();																//Override
	ActionInfo get_opposite_action(const ServerMessage& msg);
	ServerMessage get_message();
	void flush_message();
	void send_user_name(const char* name);
	void send_chess_pos(int row, int col);
	void send_chess_type(Chess::PieceType opposite_type);
	void send_quit_message();
	void send_ready_message();
private:
	ThreadQueue<ServerMessage> server_msg_queue_;
};

