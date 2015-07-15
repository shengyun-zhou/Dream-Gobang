#include "Save.h"

void Save::Record(Chess &chess_)
{
	ofstream out;
	out.open("recovery.txt", ios_base::out);
	for(int i = 0; i < Chess::SIZE; i++)
	{
		for(int j = 0; j < Chess::SIZE; j++)
		{
			out << chess_.get_point(i, j) << " ";
		}
		out << endl;
	}
	out.close();
}


Chess::PieceType Save::get_next_piece_color()
{
	ifstream in;
	in.open("recovery.txt");
	int temp;
	int k = 0;
	int v = 0;
	int i = 0;//记录黑子
	int j = 0;//记录白子
	for(; k < Chess::SIZE; k++)
	{
		for(; v < Chess::SIZE; v++)
		{
			in >> temp;
			if(temp == Chess::BLACK)
				i++;
			else if(temp == Chess::WHITE)
				j++;
		}
	}
	in.close();
	if(i >= j)
		next_piece_color = Chess::BLACK;
	else
		next_piece_color = Chess::WHITE;
	return next_piece_color;
}


Save::object Save::get_next_object()
{
	if(this -> get_next_piece_color() == Chess::BLACK)
	{
		if(first == COMPUTER)
			next_object = COMPUTER;
		else
			next_object = PLAYER;
	}
	else
	{
		if(first == COMPUTER)
			next_object = PLAYER;
		else
			next_object = COMPUTER;
	}
	return next_object;
}


void Save::set_first_object(object object_)
{
	first = object_;
}
