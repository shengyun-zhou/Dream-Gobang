#pragma once
#include "Chess.h"
#include "graphics.h"
#include "tools/Image.h"
#include "widgets/ImageTextButton.h"
#include "ChessSaver.h"
#include "ChessBoardView.h"

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
	PlayChess(Chess& c);
	~PlayChess();

	void enter_interface();
	void show_chessboard();															//显示主界面
	void update_windows();															//刷新界面
	PlayChess::ACTION_TYPE action_judge(int x, int y);	//根据鼠标坐标判断其对应区域，并返回相应操作类型
	void show_last_game(ChessSaver& saver);							//显示残局
	bool show_outcome();																//显示游戏结果
	void play_chess_by_man(int x, int y, Chess::PieceType value);//根据鼠标传来的坐标及棋子类型执行下棋动作
	void play_chess_by_computer(int row, int col, Chess::PieceType value);//根据计算机下棋的行列坐标及棋子类型

	void on_mouse_move(int x, int y, PlayChess::ACTION_TYPE action);
	void on_mouse_click(PlayChess::ACTION_TYPE action);

private:
  static ImageTextButton* button_game_replay_;
  static ImageTextButton* button_game_quit_;
	ChessBoardView chess_board_;
	Chess& chess_;

	static const int button_text_size_ = 28;
	static const int text_x_offset_ = 75;
};
