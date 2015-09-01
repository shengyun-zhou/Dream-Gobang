#include "NetPlayingInterface.h"
#include "Gobang.h"
#include "tools/GradientAnimation.h"
#include "widgets/MessageDialog.h"

ImageTextButton* NetPlayingInterface::button_quit_ = NULL;
Image* NetPlayingInterface::game_bg_img_ = NULL;

NetPlayingInterface::NetPlayingInterface(NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player, const Chess* chess)
{
	self_player_ = self_player;
	opposite_player_ = opposite_player;
	chess_ = chess;
	if (!game_bg_img_)
		game_bg_img_ = new Image("res/game-network-bg.jpg");
	if (!button_quit_)
	{
		button_quit_ = new ImageTextButton();
		button_quit_->add_normal_image(new Image("res/button-quit.png"), 0, 0);
		button_quit_->set_normal_text("退 出 对 战", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_quit_->add_hover_image(new Image("res/button-hover-quit.png"), 0, 0);
		button_quit_->set_hover_text("退 出 对 战", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_quit_->add_press_image(new Image("res/button-press-quit.png"), 0, 0);
		button_quit_->set_press_text("退 出 对 战", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}
}

NetPlayingInterface::~NetPlayingInterface()
{
}

void NetPlayingInterface::enter_interface()
{
	show_interface();
	Image screen_img;
	GradientAnimation::transition_ease_out(&screen_img);
}

void NetPlayingInterface::show_interface()
{
	game_bg_img_->show_image(0, 0);
	PIMAGE black_bg = newimage(1, 1);
	setbkcolor(BLACK, black_bg);
	cleardevice(black_bg);
	putimage_rotatezoom(NULL, black_bg, 0, 0, 0, 0, 0,
		(float)(Gobang::WINDOW_WIDTH < Gobang::WINDOW_HEIGHT) ? Gobang::WINDOW_HEIGHT : Gobang::WINDOW_WIDTH, 0, 130);
	delimage(black_bg);

	chess_board_.set_position((Gobang::WINDOW_WIDTH - chess_board_.get_view_width()) / 2, 30);
	chess_board_.show_empty_board();
	button_quit_->set_position(Gobang::WINDOW_WIDTH - margin_ - button_quit_->get_width(), 
														 Gobang::WINDOW_HEIGHT - 80 - button_quit_->get_height());
	button_quit_->show();
	self_player_->set_position(Gobang::WINDOW_WIDTH - margin_ - self_player_->get_view_width(),
														(Gobang::WINDOW_HEIGHT - self_player_->get_view_height()) / 2);
	self_player_->show();
	opposite_player_->set_position(margin_, (Gobang::WINDOW_HEIGHT - self_player_->get_view_height()) / 2);
	opposite_player_->show();
}

void NetPlayingInterface::update_interface()
{
	show_interface();
	chess_board_.show_board(*chess_);
}

void NetPlayingInterface::play_chess_by_self(int x, int y, Chess::PieceType value)
{
	chess_board_.draw_piece_by_mouse(x, y, value);
}

void NetPlayingInterface::play_chess_by_opposite(int row, int col, Chess::PieceType value)
{
	chess_board_.draw_piece_by_coor(row, col, value);
}

void NetPlayingInterface::mouse_to_coor(int x, int y, int& target_row, int& target_col)
{
	chess_board_.mouse_to_coor(x, y, target_row, target_col);
}

NetPlayingInterface::ACTION_TYPE NetPlayingInterface::action_judge(int x, int y)
{
	if (chess_board_.is_mouse_in_board(x, y))
	{
		int row, col;
		chess_board_.mouse_to_coor(x, y, row, col);
		if (row < 0 || col < 0)
			return ACTION_NONE;
		if (chess_->get_point(row, col) == Chess::EMPTY)
			return ACTION_PLAY_CHESS;
		else
			return ACTION_NONE;
	}
	else if (self_player_->action_judge(x, y) == NetPlayerInfoView::ACTION_READY)
		return ACTION_READY;
	else if (button_quit_->is_mouse_in_button(x, y))
		return ACTION_QUIT;
	else
		return ACTION_NONE;
}

bool NetPlayingInterface::show_result()
{
	MessageDialog dialog(500, 150);
	dialog.set_title("游戏结束");
	Chess::PieceType win_piece = chess_->judge_win();
	if (win_piece != Chess::EMPTY || chess_->is_chess_full())
	{
		if (win_piece == self_player_->get_piece_type())
			dialog.set_text("恭喜你，你赢了！");
		else if (win_piece == opposite_player_->get_piece_type())
			dialog.set_text("很遗憾，你输了！");
		else
			dialog.set_text("双方平局结束！");
		dialog.show();
		return true;
	}
	return false;
}

void NetPlayingInterface::on_mouse_move(int x, int y, ACTION_TYPE action)
{
	chess_board_.on_mouse_move(x, y, *chess_);
	switch (action)
	{
		case ACTION_READY:
			self_player_->on_mouse_move(NetPlayerInfoView::ACTION_READY);
			break;
		case ACTION_QUIT:
			button_quit_->on_mouse_hover();
			break;
		case ACTION_NONE:
			self_player_->on_mouse_move(NetPlayerInfoView::ACTION_NONE);
			if (button_quit_->get_hover_status())
				button_quit_->show();
			break;
		default:
			break;
	}
}

void NetPlayingInterface::on_mouse_click(ACTION_TYPE action)
{
	switch (action)
	{
		case ACTION_PLAY_CHESS:
			chess_board_.on_mouse_click();
			break;
		case ACTION_READY:
			self_player_->on_mouse_click(NetPlayerInfoView::ACTION_READY);
			break;
		case ACTION_QUIT:
			button_quit_->on_mouse_click();
			break;
		default:
			break;
	}
}
