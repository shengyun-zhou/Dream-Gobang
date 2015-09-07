#include "PlayChess.h"
#include "tools/GradientAnimation.h"
#include "image_game_main_bg.h"
#include "image_button_replay.h"
#include "image_button_hover_replay.h"
#include "image_button_press_replay.h"
#include "image_button_quit.h"
#include "image_button_hover_quit.h"
#include "image_button_press_quit.h"
#include "image_result_tie.h"
#include "image_result_win_black.h"
#include "image_result_win_white.h"

const int offset = 10;
ImageTextButton* PlayChess::button_game_quit_ = NULL;
ImageTextButton* PlayChess::button_game_replay_ = NULL;

PlayChess::PlayChess(Chess& c) : chess_(c)
{
	if (!button_game_replay_)
	{
		button_game_replay_ = new ImageTextButton();
		button_game_replay_->add_normal_image(new Image(binary_button_replay_png, sizeof(binary_button_replay_png)), 0, 0);
		button_game_replay_->set_normal_text("重 新 开 始", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_replay_->add_hover_image(new Image(binary_button_hover_replay_png, sizeof(binary_button_hover_replay_png)), 0, 0);
		button_game_replay_->set_hover_text("重 新 开 始", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_replay_->add_press_image(new Image(binary_button_press_replay_png, sizeof(binary_button_press_replay_png)), 0, 0);
		button_game_replay_->set_press_text("重 新 开 始", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}
	if (!button_game_quit_)
	{
		button_game_quit_ = new ImageTextButton();
		button_game_quit_->add_normal_image(new Image(binary_button_quit_png, sizeof(binary_button_quit_png)), 0, 0);
		button_game_quit_->set_normal_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_quit_->add_hover_image(new Image(binary_button_hover_quit_png, sizeof(binary_button_hover_quit_png)), 0, 0);
		button_game_quit_->set_hover_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_quit_->add_press_image(new Image(binary_button_press_quit_png, sizeof(binary_button_press_quit_png)), 0, 0);
		button_game_quit_->set_press_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}
}

PlayChess::~PlayChess()
{
}

void PlayChess::enter_interface()
{
	show_chessboard();
	static Image window_image = Image();
	GradientAnimation::transition_ease_out(&window_image);
}

void PlayChess::show_chessboard()
{
	setcolor(WHITE);
	setbkcolor(WHITE);

	//全局背景
	static Image game_main_bg = Image(binary_game_main_bg_jpg, sizeof(binary_game_main_bg_jpg));
	game_main_bg.show_image(0, 0);

	//棋盘
	chess_board_.set_position(80, 50);
	chess_board_.show_empty_board();

	//重新游戏图标
	button_game_replay_->set_position(930, 100);
	button_game_replay_->show();

	//退出游戏图标
	button_game_quit_->set_position(930, 200);
	button_game_quit_->show();
}


void PlayChess::update_windows()
{
	show_chessboard();
	chess_board_.show_board(chess_);
}

PlayChess::ACTION_TYPE PlayChess::action_judge(int x, int y)
{
	if (chess_board_.is_mouse_in_board(x,y))
	{
		int row, col;
		chess_board_.mouse_to_coor(x, y, row, col);
		if (row < 0 || col < 0)
			return ACTION_NONE;
		if (chess_.get_point(row, col) == Chess::EMPTY)
			return ACTION_PLAY;
		else
			return ACTION_NONE;
	}
	else if (button_game_replay_->is_mouse_in_button(x, y))
		return ACTION_REPLAY;
	else if (button_game_quit_->is_mouse_in_button(x, y))
		return ACTION_QUIT;
	else
		return ACTION_NONE;
}

void PlayChess::show_last_game(ChessSaver& saver)
{
	chess_ = saver.get_chess();
	update_windows();
}


bool PlayChess::show_outcome()
{
	static Image image_tie(binary_result_tie_png, sizeof(binary_result_tie_png));
	static Image image_win_black(binary_result_win_black_png, sizeof(binary_result_win_black_png));
	static Image image_win_white(binary_result_win_white_png, sizeof(binary_result_win_white_png));
	if(chess_.judge_win() == Chess::BLACK)
	{
	  const int image_width = image_win_black.get_width();
	  const int image_height = image_win_black.get_height();
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
	else if(chess_.judge_win() == Chess::WHITE)
	{
		const int image_width = image_win_white.get_width();
		const int image_height = image_win_white.get_height();
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
	else if(chess_.judge_win() == Chess::EMPTY && chess_.is_chess_full())
	{
	  const int image_width = image_tie.get_width();
	  const int image_height = image_tie.get_height();
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
	chess_board_.draw_piece_by_mouse(x, y, value);
	int row, col;
	chess_board_.mouse_to_coor(x, y, row, col);
	printf("x == %d, y ==%d, row == %d, col == %d\n", x, y, row, col);
	chess_.set_point(row, col, value);
}


void PlayChess::play_chess_by_computer(int row, int col, Chess::PieceType value)
{
	chess_board_.draw_piece_by_coor(row, col, value);
	chess_.set_point(row, col, value);
}

void PlayChess::on_mouse_move(int x, int y, PlayChess::ACTION_TYPE action)
{
	chess_board_.on_mouse_move(x, y, chess_);
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
  switch(action)
  {
		case PlayChess::ACTION_PLAY:
			chess_board_.on_mouse_click();
			break;
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
