#pragma once
#include "Chess.h"
#include "Save.h"
#include "graphics.h"
#include "Image.h"
#include "ImageButton.h"

class PlayChess
{
public:
	enum ACTION_TYPE
	{
		ACTION_PLAY = 1,
		ACTION_REPLAY = 2,
		ACTION_QUIT = 3,
		ACTION_NONE = 4
	};
	PlayChess(Chess* c);
	~PlayChess();

	int mouse_to_row(int x, int y);//将鼠标坐标转为行坐标
	int mouse_to_col(int x, int y);//将鼠标坐标转为列坐标
	void show_chessboard();//显示主界面
	void update_windows(Chess &c);//刷新界面
	PlayChess::ACTION_TYPE action_judge(int x, int y);//根据鼠标坐标判断其对应区域，并返回相应操作类型
	void save_last_game(Save &save, Chess &chess_);//保存残局
	void show_last_game();//显示残局
	bool show_outcome(Chess &chess);//显示游戏结果
	void play_chess_by_man(int x, int y, Chess::PieceType value);//根据鼠标传来的坐标及棋子类型执行下棋动作
	void play_chess_by_computer(int row, int col, Chess::PieceType value);//根据计算机下棋的行列坐标及棋子类型

	void on_mouse_move(PlayChess::ACTION_TYPE action);
	void on_mouse_click(PlayChess::ACTION_TYPE action);

private:
  static ImageButton* button_game_replay_;
  static ImageButton* button_game_quit_;

  static const int text_x_offset_ = 65;
	static const int text_y_offset_ = 12;
	Chess* p;

	bool judge_zone(int x, int y);//判断鼠标是否在下棋有效区域内
};
