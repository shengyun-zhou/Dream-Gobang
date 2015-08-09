#include "ChessSaver.h"
#include <exception>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>

const char* ChessSaver::file_name_ = "recovery.dat";

bool ChessSaver::chess_check(Chess& chess, Chess::PieceType player_type_)
{
	int black_num = 0;
	int white_num = 0;
	Chess::PieceType type;
	int i, j;
	for (i = 0; i < Chess::SIZE; i++)
	{
		for (j = 0; j < Chess::SIZE; j++)
		{
			type = chess.get_point(i, j);
			if (type == Chess::BLACK)
				black_num++;
			else if (type == Chess::WHITE)
				white_num++;
		}
	}
	if (player_type_ == Chess::BLACK)						//玩家执黑子先下
	{
		if (black_num == white_num)
			return true;
		return false;
	}
	else if (player_type_ == Chess::WHITE)
	{
		if (black_num - 1 == white_num)
			return true;
		return false;
	}
	else
		return false;
}

bool ChessSaver::save_chess(Chess& chess, Chess::PieceType player_type_)
{
	if (chess_check(chess, player_type_) == false)
		return false;
	ofstream output_file;
	output_file.open(file_name_, ios::out);
	if (output_file.fail())
		return false;
	int i, j;
	Chess::PieceType type;
	try
	{
		for (i = 0; i < Chess::SIZE; i++)
		{
			for (j = 0; j < Chess::SIZE; j++)
			{
				type = chess.get_point(i, j);
				if (type == Chess::BLACK)
					output_file << 1 << " ";
				else if (type == Chess::WHITE)
					output_file << -1 << " ";
				else
					output_file << 0 << " ";
				if (output_file.fail() || output_file.bad())
				{
					output_file.close();
					return false;
				}
			}
			output_file << endl;
		}
		if (player_type_ == Chess::BLACK)
			output_file << "black";
		else if (player_type_ == Chess::WHITE)
			output_file << "white";
		else
		{
			output_file.close();
			return false;
		}
		output_file.close();
		return true;
	}catch (exception e) {
		cout << e.what() << endl;
		output_file.close();
		return false;
	}
}

bool ChessSaver::read_chess()
{
	ifstream input_file;
	input_file.open(file_name_, ios::in);
	if (input_file.fail())
		return false;
	int i, j;
	int temp_num;
	try
	{
		for (i = 0; i < Chess::SIZE; i++)
		{
			for (j = 0; j < Chess::SIZE; j++)
			{
				if (input_file.eof() || input_file.bad() || input_file.fail())
				{
					input_file.close();
					return false;
				}
				input_file >> temp_num;
				switch (temp_num)
				{
					case 0:
						chess_buf_.set_point(i, j, Chess::EMPTY);
						break;
					case -1:
						chess_buf_.set_point(i, j, Chess::WHITE);
						break;
					case 1:
						chess_buf_.set_point(i, j, Chess::BLACK);
						break;
					default:
						input_file.close();
						return false;
				}
			}
		}
		if (input_file.eof())
		{
			input_file.close();
			return false;
		}
		string type;
		char temp[100];
		input_file >> temp;
		type = temp;
		if (type == "black")
			player_piece_color_ = Chess::BLACK;
		else if (type == "white")
			player_piece_color_ = Chess::WHITE;
		else
		{
			input_file.close();
			return false;
		}
		input_file.close();
		return chess_check(chess_buf_, player_piece_color_);
	}
	catch (exception e) {
		cout << e.what() << endl;
		input_file.close();
		return false;
	}
}

bool ChessSaver::is_recovery_data_exist()
{
	ifstream in;
	in.open(file_name_, ios::in);
	if (in.fail())
		return false;
	in.close();
	return true;
}

bool ChessSaver::delete_recovery_file()
{
	if (remove(file_name_) == 0)
		return true;
	printf(strerror(errno));
	return false;
}
