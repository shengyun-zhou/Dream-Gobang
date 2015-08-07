#include "PlayChess.h"

const int offset = 10;
ImageButton* PlayChess::button_game_quit_ = NULL;
ImageButton* PlayChess::button_game_replay_ = NULL;

PlayChess::PlayChess(Chess *c)
{
	p = c;
	if (!button_game_replay_)
	{
		button_game_replay_ = new ImageButton();
		Image* text_game_replay = new Image("res/text-replay.png");
		button_game_replay_->add_normal_image(new Image("res/button-replay.png"), 0, 0);
		button_game_replay_->add_normal_image(text_game_replay, text_x_offset_, text_y_offset_);
		button_game_replay_->add_hover_image(new Image("res/button-hover-replay.png"), 0, 0);
		button_game_replay_->add_hover_image(text_game_replay, text_x_offset_, text_y_offset_);
		button_game_replay_->add_press_image(new Image("res/button-press-replay.png"), 0, 0);
		button_game_replay_->add_press_image(text_game_replay, text_x_offset_, text_y_offset_);
	}
	if (!button_game_quit_)
	{
		button_game_quit_ = new ImageButton();
		Image* text_game_quit = new Image("res/text-quit.png");
		button_game_quit_->add_normal_image(new Image("res/button-quit.png"), 0, 0);
		button_game_quit_->add_normal_image(text_game_quit, text_x_offset_, text_y_offset_);
		button_game_quit_->add_hover_image(new Image("res/button-hover-quit.png"), 0, 0);
		button_game_quit_->add_hover_image(text_game_quit, text_x_offset_, text_y_offset_);
		button_game_quit_->add_press_image(new Image("res/button-press-quit.png"), 0, 0);
		button_game_quit_->add_press_image(text_game_quit, text_x_offset_, text_y_offset_);
	}
}


PlayChess::~PlayChess()
{
}


void PlayChess::show_chessboard()
{
	setcolor(WHITE);
	setbkcolor(WHITE);

	//全局背景
	static Image game_main_bg = Image("res/game-main-bg.jpg");
	game_main_bg.show_image(0, 0);

	//棋盘
	setfillcolor(WHITE);
	bar(395, 365, 405, 375);
	bar(235, 205, 245, 215);
	bar(555, 205, 565, 215);
	bar(235, 525, 245, 535);
	bar(555, 525, 565, 535);
	setlinestyle(SOLID_LINE, 0, 5);
	line(80, 50, 720, 50);
	line(80, 50, 80, 690);
	line(720, 50, 720, 690);
	line(80, 690, 720, 690);
	setlinestyle(SOLID_LINE, 0, 2);
	for (int i = 0; i < 640; i += 40)
		line(80, 90 + i, 720, 90 + i);
	for (int i = 0; i < 640; i += 40)
		line(120 + i, 50, 120 + i, 690);

	//重新游戏图标
	static Image game_replay("res/button-replay.png");
	button_game_replay_->set_position(930, 100);
	button_game_replay_->show();

	//退出游戏图标
	static Image game_quit("res/button-quit.png");
	button_game_quit_->set_position(930, 200);
	button_game_quit_->show();
}


void PlayChess::update_windows()
{
	show_chessboard();
	int i, j;
	for (i = 0; i < Chess::SIZE; i++)
	{
		for (j = 0; j < Chess::SIZE; j++)
		{
			if (p->get_point(i, j) != Chess::EMPTY)
				play_chess_by_computer(i, j, p->get_point(i, j));
		}
	}
}

PlayChess::ACTION_TYPE PlayChess::action_judge(int x, int y)
{
	if (judge_zone(x, y))
	{
		for (int i = 0; i < 640; i += 40)
		{
			for (int j = 0; j < 640; j += 40)
			{
				if (x > 120 + i - offset && x < 120 + i + offset && y > 90 + j - offset && y < 90 + j + offset)
				{
					int row = mouse_to_row(x, y);
					int col = mouse_to_col(x, y);
					printf("Row:%d, Col:%d\n", row, col);
					if (p->get_point(row, col) == Chess::EMPTY)
						return ACTION_PLAY;
					else
						return ACTION_NONE;
				}
			}
		}
	}
	else if (x >= 930 && y >= 100 && x <= 1170 && y <= 160)
		return ACTION_REPLAY;
	else if (x >= 930 && y >= 200 && x <= 1170 && y <= 260)
		return ACTION_QUIT;
	else
		return ACTION_NONE;
}

void PlayChess::show_last_game(ChessSaver& saver)
{
	*p = saver.get_chess();
	update_windows();
}


bool PlayChess::show_outcome()
{
	static Image image_tie("res/result-tie.png");
	static Image image_win_black("res/result-win-black.png");
	static Image image_win_white("res/result-win-white.png");
	if(p->judge_win() == Chess::BLACK)
	{
	  const int image_width = 454;
	  const int image_height = 340;
	  int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
		for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
		{
			if (color > 128)
				i += deltaAlpha;
			if (i > 255)
				i = 255;
			/*由完全透明到不透明渲染图片*/
			image_win_black.show_image_with_alpha(640 - image_width / 2, 360 - image_height / 2, (double)i / 255.0);
		}
	  return true;
	}
	else if(p->judge_win() == Chess::WHITE)
	{
		const int image_width = 454;
		const int image_height = 340;
	  int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
		for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
		{
			if (color > 128)
				i += deltaAlpha;
			if (i > 255)
				i = 255;
			/*由完全透明到不透明渲染Logo图片*/
			image_win_white.show_image_with_alpha(640 - image_width / 2, 360 - image_height / 2, (double)i / 255.0);
		}
	  return true;
	}
	else if(p->judge_win() == Chess::EMPTY && p->is_chess_full())
	{
	  const int image_width = 500;
	  const int image_height = 200;
	  int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
	  for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
	  {
		  if (color > 128)
			  i += deltaAlpha;
		  if (i > 255)
			  i = 255;
		  /*由完全透明到不透明渲染Logo图片*/
			image_tie.show_image_with_alpha(640 - image_width / 2, 360 - image_height / 2, (double)i / 255.0);
	  }
	  return true;
	}
	return false;
}


void PlayChess::play_chess_by_man(int x, int y, Chess::PieceType value)
{
	static Image black_piece("res/black-piece.png");
	static Image white_piece("res/white-piece.png");
	if (judge_zone(x, y))
	{
		for (int i = 0; i < 640; i += 40)
		{
			for (int j = 0; j < 640; j += 40)
			{
				if (x > 120 + i - offset && x < 120 + i + offset && y > 90 + j - offset && y < 90 + j + offset)
				{
					int row = mouse_to_row(x, y);
					int col = mouse_to_col(x, y);
					p -> set_point(row, col, value);
					printf("play_row:%d, play_col:%d\n", row, col);
					if (value == Chess::BLACK)
					{
						black_piece.show_image_with_alpha(120 + i - 15, 90 + j - 15, 1.0);
					}
					else if (value == Chess::WHITE)
					{
						white_piece.show_image_with_alpha(120 + i - 15, 90 + j - 15, 1.0);
					}
				}
			}
		}
	}
}


void PlayChess::play_chess_by_computer(int row, int col, Chess::PieceType value)
{
	static Image black_piece("res/black-piece.png");
	static Image white_piece("res/white-piece.png");
	int x = 80 + (col + 1) * 40;
	int y = 50 + (row + 1) * 40;
	if(judge_zone(x, y))
	{
		p -> set_point(row, col, value);
	  PIMAGE img = newimage();
	  if (value == Chess::BLACK)
	  {
			black_piece.show_image_with_alpha(x - 15, y - 15, 1.0);
	  }
	  else if (value == Chess::WHITE)
	  {
			white_piece.show_image_with_alpha(x - 15, y - 15, 1.0);
	  }
	}
}


int PlayChess::mouse_to_row(int x, int y)
{
	if (judge_zone(x, y))
	{
		for (int i = 0; i < 640; i += 40)
		{
			if (y > 90 + i - offset && y < 90 + i + offset)
			{
				return i / 40;
			}
		}
	}
}


int PlayChess::mouse_to_col(int x, int y)
{
	if (judge_zone(x, y))
	{
		for (int i = 0; i < 640; i += 40)
		{
		  if (x > 120 + i - offset && x < 120 + i + offset)
			{
				return i / 40;
			}
		}
	}
}

bool PlayChess::judge_zone(int x, int y)
{
	if (x >= 120 - offset && x <= 680 + offset && y >= 90 - offset && y <= 650 + offset)
		return true;
	else
		return false;
}

void PlayChess::on_mouse_move(PlayChess::ACTION_TYPE action)
{
  switch(action)
  {
    case PlayChess::ACTION_REPLAY:
			button_game_replay_->on_mouse_hover();
      break;
    case PlayChess::ACTION_QUIT:
			button_game_quit_->on_mouse_hover();
      break;
    case PlayChess::ACTION_NONE:
			if (button_game_replay_->get_hover_status())
				button_game_replay_->show();
			if (button_game_quit_->get_hover_status())
				button_game_quit_->show();
      break;
    default:
      break;
  }
}

void PlayChess::on_mouse_click(PlayChess::ACTION_TYPE action)
{
  static Image button_press_replay("res/button-press-replay.png");
  static Image button_press_quit("res/button-press-quit.png");
  switch(action)
  {
    case PlayChess::ACTION_REPLAY:
			button_game_replay_->on_mouse_click();
      break;
    case PlayChess::ACTION_QUIT:
			button_game_quit_->on_mouse_click();
      break;
    default:
      break;
  }
}
