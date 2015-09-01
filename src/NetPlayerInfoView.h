#pragma once
#include "Chess.h"
#include "tools/Image.h"
#include "widgets/ImageTextButton.h"
#include <string>

using namespace std;

class NetPlayerInfoView
{
private:
	bool is_opposite_;
	bool is_ready_;
	Chess::PieceType piece_type_;
	string player_name_;
	int pos_x_;
	int pos_y_;
	int view_width_;
	int view_height_;

	static const int margin_ = 10;
	static const int name_font_size_ = 30;
	static const int ready_font_size_ = 20;
	static const int button_text_size_ = 22;
	Image* black_piece_;
	Image* white_piece_;
	Image* player_pic_;
	ImageTextButton* button_ready_;

	struct BufferImage
	{
		int x;
		int y;
		PIMAGE img;
	};
	BufferImage buffer_img_;

	void calc_view_width_height();
public:
	NetPlayerInfoView(bool is_opposite);
	~NetPlayerInfoView();

	enum ACTION_TYPE
	{
		ACTION_NONE,
		ACTION_READY
	};

	ACTION_TYPE action_judge(int x, int y);
	void on_mouse_move(ACTION_TYPE type);
	void on_mouse_click(ACTION_TYPE type);

	void set_position(int x, int y)
	{
		pos_x_ = x;
		pos_y_ = y;
	}

	void set_player_name(const char* name)
	{
		player_name_ = name;
	}

	void set_ready_state(bool is_ready)
	{
		is_ready_ = is_ready;
	}

	bool is_ready()
	{
		return is_ready_;
	}

	void set_piece_type(Chess::PieceType piece_type)
	{
		piece_type_ = piece_type;
		switch (piece_type)
		{
			case Chess::BLACK:
				player_pic_ = black_piece_;
				break;
			case Chess::WHITE:
				player_pic_ = white_piece_;
				break;
			default:
				player_pic_ = NULL;
				break;
		}
	}

	Chess::PieceType get_piece_type()
	{
		return piece_type_;
	}

	void show();

	int get_view_width()
	{
		calc_view_width_height();
		return view_width_;
	}

	int get_view_height()
	{
		calc_view_width_height();
		return view_height_;
	}
};

