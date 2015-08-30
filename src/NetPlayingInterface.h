#pragma once
#include "NetPlayerInfoView.h"
#include "ChessBoardView.h"
#include "widgets/ImageButton.h"
class NetPlayingInterface
{
private:
	NetPlayerInfoView* self_player_;
	NetPlayerInfoView* opposite_player_;
	ChessBoardView chess_board_;
	const Chess* chess_;
	static ImageButton* button_quit_;
	static Image* game_bg_img_;

	static const int text_x_offset_ = 65;
	static const int text_y_offset_ = 12;
	static const int margin_ = 20;
public:
	NetPlayingInterface(NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player, const Chess* chess);
	~NetPlayingInterface();

	void enter_interface();
	void show_interface();
	void update_interface();
	void play_chess_by_self(int x, int y, Chess::PieceType value);
	void play_chess_by_opposite(int row, int col, Chess::PieceType value);
	void mouse_to_coor(int x, int y, int& target_row, int& target_col);

	enum ACTION_TYPE
	{
		ACTION_NONE,
		ACTION_READY,
		ACTION_QUIT,
		ACTION_PLAY_CHESS
	};

	ACTION_TYPE action_judge(int x, int y);
	void on_mouse_move(int x, int y, ACTION_TYPE action);
	void on_mouse_click(ACTION_TYPE action);
};

