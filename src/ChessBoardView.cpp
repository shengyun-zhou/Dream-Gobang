#include "ChessBoardView.h"
#include "graphics.h"
Image* ChessBoardView::black_piece_image_ = NULL;
Image* ChessBoardView::white_piece_image_ = NULL;

ChessBoardView::ChessBoardView()
{
	if (!black_piece_image_)
		black_piece_image_ = new Image("res/black-piece.png");
	if (!white_piece_image_)
		white_piece_image_ = new Image("res/white-piece.png");
	pos_x_ = pos_y_ = 10;
	buffer_img_.x = -width_each_row_col_;
	buffer_img_.y = -width_each_row_col_;
	buffer_img_.img = NULL;
}

ChessBoardView::~ChessBoardView()
{
}

void ChessBoardView::show_empty_board()
{
	setfillcolor(WHITE);
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 5);
	bar(pos_x_ + 315, pos_y_ + 315, pos_x_ + 315 + block_edge_length_, pos_y_ + 315 + block_edge_length_);
	bar(pos_x_ + 155, pos_y_ + 155, pos_x_ + 155 + block_edge_length_, pos_y_ + 155 + block_edge_length_);
	bar(pos_x_ + 475, pos_y_ + 155, pos_x_ + 475 + block_edge_length_, pos_y_ + 155 + block_edge_length_);
	bar(pos_x_ + 155, pos_y_ + 475, pos_x_ + 155 + block_edge_length_, pos_y_ + 475 + block_edge_length_);
	bar(pos_x_ + 475, pos_y_ + 475, pos_x_ + 475 + block_edge_length_, pos_y_ + 475 + block_edge_length_);

	line(pos_x_, pos_y_, pos_x_ + chess_edge_length_, pos_y_);
	line(pos_x_, pos_y_, pos_x_, pos_y_ + chess_edge_length_);
	line(pos_x_ + chess_edge_length_, pos_y_, pos_x_ + chess_edge_length_, pos_y_ + chess_edge_length_);
	line(pos_x_, pos_y_ + chess_edge_length_, pos_x_ + chess_edge_length_, pos_y_ + chess_edge_length_);
	setlinestyle(SOLID_LINE, 0, 2);
	for (int i = 1; i <= Chess::SIZE; i++)
		line(pos_x_, pos_y_ + i * width_each_row_col_, pos_x_ + chess_edge_length_, pos_y_ + i * width_each_row_col_);
	for (int i = 1; i <= Chess::SIZE; i++)
		line(pos_x_ + i * width_each_row_col_, pos_y_, pos_x_ + i * width_each_row_col_, pos_y_ + chess_edge_length_);
}

void ChessBoardView::show_board(const Chess& chess)
{
	show_empty_board();
	int i, j;
	for (i = 0; i < Chess::SIZE; i++)
	{
		for (j = 0; j < Chess::SIZE; j++)
			draw_piece_by_coor(i, j, chess.get_point(i, j));
	}
}

bool ChessBoardView::is_mouse_in_board(int x, int y)
{
	if (x >= pos_x_ + width_each_row_col_ - offset_ && x <= pos_x_ + Chess::SIZE * width_each_row_col_ + offset_ && 
			y >= pos_y_ + width_each_row_col_ - offset_ && y <= pos_x_ + Chess::SIZE * width_each_row_col_ + offset_)
		return true;
	else
		return false;
}

void ChessBoardView::mouse_to_coor(int mouse_x, int mouse_y, int& target_row, int& target_col)
{
	target_row = target_col = -1;
	if (is_mouse_in_board(mouse_x, mouse_y))
	{
		for (int i = width_each_row_col_; i <= Chess::SIZE * width_each_row_col_; i += width_each_row_col_)
		{
			if (mouse_y >= pos_y_ + i - offset_ && mouse_y <= pos_y_ + i + offset_)
				target_row = i / width_each_row_col_ - 1;
			if (mouse_x >= pos_x_ + i - offset_ && mouse_x <= pos_x_ + i + offset_)
				target_col = i / width_each_row_col_ - 1;
		}
	}
} 

void ChessBoardView::draw_piece_by_mouse(int x, int y, Chess::PieceType piece_type)
{
	int row, col;
	mouse_to_coor(x, y, row, col);
	if (row < 0 || col < 0)
		return;
	draw_piece_by_coor(row, col, piece_type);
}

void ChessBoardView::draw_piece_by_coor(int row, int col, Chess::PieceType piece_type)
{
	row++;
	col++;
	int target_x = pos_x_ + col * width_each_row_col_;
	int target_y = pos_y_ + row * width_each_row_col_;
	switch (piece_type)
	{
		case Chess::BLACK:
			black_piece_image_->show_image_with_alpha(target_x - black_piece_image_->get_width() / 2,
				target_y - black_piece_image_->get_height() / 2, 1.0);
			break;
		case Chess::WHITE:
			white_piece_image_->show_image_with_alpha(target_x - white_piece_image_->get_width() / 2,
				target_y - white_piece_image_->get_height() / 2, 1.0);
			break;
		default:
			break;
	}
}

void ChessBoardView::on_mouse_move(int x, int y, const Chess& chess)
{
	if (x >= buffer_img_.x && x <= buffer_img_.x + 2 * offset_ &&
		y >= buffer_img_.y && y <= buffer_img_.y + 2 * offset_)
		return;
	if (buffer_img_.img)
	{
		putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
		delimage(buffer_img_.img);
	}
	buffer_img_.x = buffer_img_.y = -width_each_row_col_;
	buffer_img_.img = NULL;
	if (!is_mouse_in_board(x, y))
		return;
	int row, col;
	mouse_to_coor(x, y, row, col);
	if (row < 0 || col < 0)
		return;
	if (chess.get_point(row, col) != Chess::EMPTY)
		return;
	row++;
	col++;
	int target_left = pos_x_ + col * width_each_row_col_ - offset_;
	int target_top = pos_y_ + row * width_each_row_col_ - offset_;

	int temp_viewport_left, temp_viewport_right, temp_viewport_top, temp_viewport_bottom;
	getviewport(&temp_viewport_left, &temp_viewport_top, &temp_viewport_right, &temp_viewport_bottom);
	setviewport(target_left, target_top, target_left + 2 * offset_, target_top + 2 * offset_);
	buffer_img_.x = target_left;
	buffer_img_.y = target_top;
	buffer_img_.img = newimage(2 * offset_, 2 * offset_);
	putimage(buffer_img_.img, 0, 0, NULL);
	setviewport(temp_viewport_left, temp_viewport_top, temp_viewport_right, temp_viewport_bottom);

	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 2);
	target_left += offset_;
	target_top += offset_;
	line(target_left - block_edge_length_ / 2 - 1, target_top - block_edge_length_ / 2 - 1, target_left - offset_ + 1, target_top - block_edge_length_ / 2 - 1);
	line(target_left - block_edge_length_ / 2 - 1, target_top - block_edge_length_ / 2 - 1, target_left - block_edge_length_ / 2 - 1, target_top - offset_ + 1);
	line(target_left + block_edge_length_ / 2 + 1, target_top - block_edge_length_ / 2 - 1, target_left + offset_ - 1, target_top - block_edge_length_ / 2 - 1);
	line(target_left + block_edge_length_ / 2 + 1, target_top - block_edge_length_ / 2 - 1, target_left + block_edge_length_ / 2 + 1, target_top - offset_ + 1);
	line(target_left - block_edge_length_ / 2 - 1, target_top + block_edge_length_ / 2 + 1, target_left - offset_ + 1, target_top + block_edge_length_ / 2 + 1);
	line(target_left - block_edge_length_ / 2 - 1, target_top + block_edge_length_ / 2 + 1, target_left - block_edge_length_ / 2 - 1, target_top + offset_ - 1);
	line(target_left + block_edge_length_ / 2 + 1, target_top + block_edge_length_ / 2 + 1, target_left + offset_ - 1, target_top + block_edge_length_ / 2 + 1);
	line(target_left + block_edge_length_ / 2 + 1, target_top + block_edge_length_ / 2 + 1, target_left + block_edge_length_ / 2 + 1, target_top + offset_ - 1);
}

void ChessBoardView::on_mouse_click()
{
	if (buffer_img_.img)
	{
		putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
		delimage(buffer_img_.img);
	}
	buffer_img_.x = buffer_img_.y = -width_each_row_col_;
	buffer_img_.img = NULL;
}
