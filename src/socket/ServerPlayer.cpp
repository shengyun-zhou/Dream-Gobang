#include "ServerPlayer.h"
#ifdef _MSC_VER
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#endif

ServerPlayer::ServerPlayer() : ServerSocket()
{
}

ServerPlayer::~ServerPlayer()
{
}

void ServerPlayer::flush_message()
{
	while (!server_msg_queue_.empty())
		server_msg_queue_.pop();
}

void ServerPlayer::start()
{
	flush_message();
	ServerSocket::start();
}

ServerPlayer::ServerMessage ServerPlayer::get_message()
{
	ServerMessage msg;
	if (server_msg_queue_.empty())
	{
		msg.msg_type = MESSAGE_NONE;
		return msg;
	}
	msg = server_msg_queue_.front();
	server_msg_queue_.pop();
	return msg;
}

ServerPlayer::ActionInfo ServerPlayer::get_opposite_action(const ServerMessage& msg)
{
	ActionInfo action_info;
	action_info.action_type = ACTION_NONE;
	if (msg.msg_type == MESSAGE_COMPLETE && msg.msg_content.complete_msg == COMPLETE_RECEIVE)
	{
		if (msg.receive_str == Gobang::message_ready)
			action_info.action_type = ACTION_READY;
		else if (msg.receive_str == Gobang::message_quit)
			action_info.action_type = ACTION_QUIT;
		else if (msg.receive_str.length() > strlen(Gobang::message_head_user_name) &&
			msg.receive_str.substr(0, strlen(Gobang::message_head_user_name)) == Gobang::message_head_user_name)
		{
			action_info.action_type = ACTION_TELL_USER_NAME;
			strcpy(action_info.ex_data.user_name, msg.receive_str.substr(strlen(Gobang::message_head_user_name)).c_str());
		}
		else if (msg.receive_str.length() > strlen(Gobang::message_head_chess_pos) &&
			msg.receive_str.substr(0, strlen(Gobang::message_head_chess_pos)) == Gobang::message_head_chess_pos)
		{
			action_info.action_type = ACTION_PLAY_CHESS;
			ClientPlayer::parse_chess_pos(msg.receive_str.substr(strlen(Gobang::message_head_chess_pos)), action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1]);
		}
	}
	return action_info;
}

void ServerPlayer::send_user_name(const char* user_name)
{
	send_string((string(Gobang::message_head_user_name) + user_name).c_str());
}

void ServerPlayer::send_quit_message()
{
	send_string(Gobang::message_quit);
}

void ServerPlayer::send_ready_message()
{
	send_string(Gobang::message_ready);
}

void ServerPlayer::send_chess_pos(int row, int col)
{
	char temp[10];
	string row_str, col_str;
	itoa(row, temp, 10);
	row_str = temp;
	itoa(col, temp, 10);
	col_str = temp;
	send_string((string(Gobang::message_head_chess_pos) + row_str + "," + col_str).c_str());
}

void ServerPlayer::send_chess_type(Chess::PieceType opposite_type)
{
	if (opposite_type == Chess::BLACK)
		send_string((string(Gobang::message_head_piece_type) + Gobang::text_black_piece).c_str());
	else if (opposite_type == Chess::WHITE)
		send_string((string(Gobang::message_head_piece_type) + Gobang::text_white_piece).c_str());
}

void ServerPlayer::on_init_failed()
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_WIN_SOCK_INIT_FAILED;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_socket_create_failed(int WSA_error_code)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_SOCKET_CREATE_FAILED;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_socket_bind_failed(SOCKET socket, int WSA_error_code)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_SOCKET_BIND_FAILED;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_socket_listen_success(SOCKET socket)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_COMPLETE;
	msg.msg_content.complete_msg = COMPLETE_LISTEN;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_accept_failed(SOCKET server_socket, int WSA_error_code)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_ACCEPT_FAILED;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_accept_success(SOCKET server_socket, SOCKET connect_socket)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_COMPLETE;
	msg.msg_content.complete_msg = COMPLETE_ACCEPT;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_receive_failed(SOCKET server_socket, SOCKET connect_socket, int WSA_error_code)
{
	ServerMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_RECEIVE_FAILED;
	server_msg_queue_.push(msg);
}

void ServerPlayer::on_receive_completed(SOCKET server_socket, SOCKET connect_socket, char* str, int length)
{
	if (str)
	{
		ServerMessage msg;
		msg.msg_type = MESSAGE_COMPLETE;
		msg.msg_content.complete_msg = COMPLETE_RECEIVE;
		msg.receive_str = str;
		server_msg_queue_.push(msg);
	}
}

void ServerPlayer::on_send_failed(SOCKET server_socket, SOCKET connect_socket, const char* send_str, int WSA_error_code)
{
	if (WSA_error_code == WSAEWOULDBLOCK)
	{
		Sleep(1000);
		send_string(send_str);															//≥¢ ‘÷ÿ–¬∑¢ÀÕ
	}
	else
	{
		ServerMessage msg;
		msg.msg_type = MESSAGE_ERROR;
		msg.msg_content.error_msg = ERROR_SEND_FAILED;
		server_msg_queue_.push(msg);
	}
}

void ServerPlayer::on_send_completed(SOCKET server_socket, SOCKET connect_socket)
{
	//Sleep(2000);
	ServerMessage msg;
	msg.msg_type = MESSAGE_COMPLETE;
	msg.msg_content.complete_msg = COMPLETE_SEND;
	server_msg_queue_.push(msg);
}