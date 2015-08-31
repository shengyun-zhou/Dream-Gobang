#include "ClientPlayer.h"
#include "../Gobang.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#endif

ClientPlayer::ClientPlayer()
{
}

ClientPlayer::~ClientPlayer()
{
}

void ClientPlayer::flush_message()
{
	client_msg_queue_.lock();
	while (!client_msg_queue_.is_empty(false))
		client_msg_queue_.pop(false);
	client_msg_queue_.unlock();
}

void ClientPlayer::start()
{
	flush_message();
	ClientSocket::start();
}

ClientPlayer::ClientMessage ClientPlayer::get_message()
{
	ClientMessage msg;
	if (client_msg_queue_.is_empty())
	{
		msg.msg_type = MESSAGE_NONE;
		return msg;
	}
	return client_msg_queue_.pop();
}

void ClientPlayer::parse_chess_pos(const string& str, int& target_row, int& target_col)
{
	target_row = target_col = -1;
	int i, pos;
	for (i = 0; i < str.length(); i++)
	{
		if ('0' <= str[i] && str[i] <= '9')
			continue;
		else if (str[i] == ',')
			pos = i;
		else
			return;
	}

	target_row = atoi(str.substr(0, pos).c_str());
	target_col = atoi(str.substr(pos + 1).c_str());
}

ClientPlayer::ActionInfo ClientPlayer::get_opposite_action(const ClientMessage& msg)
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
		else if (msg.receive_str.length() > strlen(Gobang::message_head_piece_type) &&
			msg.receive_str.substr(0, strlen(Gobang::message_head_piece_type)) == Gobang::message_head_piece_type)
		{
			action_info.action_type = ACTION_TELL_PIECE_TYPE;
			string piece_value = msg.receive_str.substr(strlen(Gobang::message_head_piece_type));
			if (piece_value == Gobang::text_black_piece)
				action_info.ex_data.piece_type = Chess::BLACK;
			else if (piece_value == Gobang::text_white_piece)
				action_info.ex_data.piece_type = Chess::WHITE;
			else
				action_info.action_type = ACTION_NONE;
		}
		else if (msg.receive_str.length() > strlen(Gobang::message_head_chess_pos) &&
			msg.receive_str.substr(0, strlen(Gobang::message_head_chess_pos)) == Gobang::message_head_chess_pos)
		{
			action_info.action_type = ACTION_PLAY_CHESS;
			parse_chess_pos(msg.receive_str.substr(strlen(Gobang::message_head_chess_pos)), action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1]);
		}
	}
	return action_info;
}

void ClientPlayer::send_user_name(const char* user_name)
{
	send_string((string(Gobang::message_head_user_name) + user_name).c_str());
}

void ClientPlayer::send_quit_message()
{
	send_string(Gobang::message_quit);
}

void ClientPlayer::send_ready_message()
{
	send_string(Gobang::message_ready);
}

void ClientPlayer::send_chess_pos(int row, int col)
{
	char temp[10];
	string row_str, col_str;
	itoa(row, temp, 10);
	row_str = temp;
	itoa(col, temp, 10);
	col_str = temp;
	send_string((string(Gobang::message_head_chess_pos) + row_str + "," + col_str).c_str());
}

void ClientPlayer::on_init_failed()
{
	ClientMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_WIN_SOCK_INIT_FAILED;
	client_msg_queue_.push(msg);
}

void ClientPlayer::on_socket_create_failed(int WSA_error_code)
{
	ClientMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_SOCKET_CREATE_FAILED;
	client_msg_queue_.push(msg);
}

void ClientPlayer::on_connect_failed(SOCKET socket, int WSA_error_code)
{
	ClientMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_CONNECT_FAILED;
	client_msg_queue_.push(msg);
}

void ClientPlayer::on_connect_success(SOCKET socket)
{
	ClientMessage msg;
	msg.msg_type = MESSAGE_COMPLETE;
	msg.msg_content.complete_msg = COMPLETE_CONNECT;
	client_msg_queue_.push(msg);
}

void ClientPlayer::on_receive_failed(SOCKET socket, int WSA_error_code)
{
	ClientMessage msg;
	msg.msg_type = MESSAGE_ERROR;
	msg.msg_content.error_msg = ERROR_RECEIVE_FAILED;
	client_msg_queue_.push(msg);
}

void ClientPlayer::on_receive_completed(SOCKET socket, char* str, int length)
{
	if (str)
	{
		printf("received message:%s\n", str);
		ClientMessage msg;
		msg.msg_type = MESSAGE_COMPLETE;
		msg.msg_content.complete_msg = COMPLETE_RECEIVE;
		msg.receive_str = str;
		client_msg_queue_.push(msg);
	}
}

void ClientPlayer::on_send_failed(SOCKET socket, const char* send_str, int WSA_error_code)
{
	if (WSA_error_code == WSAEWOULDBLOCK)
	{
		Sleep(1000);
		send_string(send_str);															//≥¢ ‘÷ÿ–¬∑¢ÀÕ
	}
	else
	{
		ClientMessage msg;
		msg.msg_type = MESSAGE_ERROR;
		msg.msg_content.error_msg = ERROR_SEND_FAILED;
		client_msg_queue_.push(msg);
	}
}

void ClientPlayer::on_send_completed(SOCKET socket)
{
	//Sleep(1000);
	ClientMessage msg;
	msg.msg_type = MESSAGE_COMPLETE;
	msg.msg_content.complete_msg = COMPLETE_SEND;
	client_msg_queue_.push(msg);
}
