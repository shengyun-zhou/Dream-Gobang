#pragma once
#include "Chess.h"
#include "tools/Image.h"
class ChessBoardView
{
private:
	static Image* black_piece_image_;
	static Image* white_piece_image_;
	int pos_x_;
	int pos_y_;
	static const int chess_edge_length_ = 640;
	static const int block_edge_length_ = 10;
	static const int width_each_row_col_ = chess_edge_length_ / (Chess::SIZE + 1);
	static const int offset_ = 10;
public:
	ChessBoardView();
	~ChessBoardView();

	void set_position(int x, int y)
	{
		pos_x_ = x;
		pos_y_ = y;
	}

	void show_empty_board();
	void show_board(const Chess& c);
	void draw_piece_by_mouse(int x, int y, Chess::PieceType piece_type);
	void draw_piece_by_coor(int row, int col, Chess::PieceType piece_type);
	bool is_mouse_in_board(int x, int y);
	void mouse_to_coor(int mouse_x, int mouse_y, int& target_row, int& target_col);
};

