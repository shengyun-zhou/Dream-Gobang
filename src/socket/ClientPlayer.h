#pragma once
#include "ClientSocket.h"
#include "../Gobang.h"
#include "../Chess.h"
#include <queue>
class ClientPlayer : public ClientSocket
{
protected:
	virtual void on_init_failed();
	virtual void on_socket_create_failed(int WSA_error_code);
	virtual void on_connect_failed(SOCKET socket, int WSA_error_code);
	virtual void on_connect_success(SOCKET socket);
	virtual void on_receive_failed(SOCKET socket, int WSA_error_code);
	virtual void on_receive_completed(SOCKET socket, char* str, int length);
	virtual void on_send_failed(SOCKET socket, const char* send_str, int WSA_error_code);
	virtual void on_send_completed(SOCKET socket);
public:
	ClientPlayer();
	~ClientPlayer();

	enum ClientError
	{
		ERROR_NONE,
		ERROR_WIN_SOCK_INIT_FAILED,
		ERROR_SOCKET_CREATE_FAILED,
		ERROR_CONNECT_FAILED,
		ERROR_RECEIVE_FAILED,
		ERROR_SEND_FAILED,
	};

	enum ClientComplete
	{
		COMPLETE_NONE,
		COMPLETE_CONNECT,
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
		ACTION_TELL_PIECE_TYPE,
		ACTION_TELL_USER_NAME,
		ACTION_QUIT,
	};

	struct ClientMessage
	{
		MessageType msg_type;
		union MessageContent
		{
			ClientComplete complete_msg;
			ClientError error_msg;
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
			Chess::PieceType piece_type;
		}ex_data;
	};

	void start();																					//Override
	ActionInfo get_opposite_action(const ClientMessage& msg);
	ClientMessage get_message();
	void flush_message();
	void send_user_name(const char* name);
	void send_chess_pos(int row, int col);
	void send_quit_message();
	void send_ready_message();

	static void parse_chess_pos(const string& str, int& target_row, int& target_col);
private:
	queue<ClientMessage> client_msg_queue_;
};

