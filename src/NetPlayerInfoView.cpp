#include "NetPlayerInfoView.h"
#include "Gobang.h"
#include <windef.h>

NetPlayerInfoView::NetPlayerInfoView(bool is_opposite)
{
	is_opposite_ = is_opposite;
	black_piece_ = new Image("res/black-piece.png");
	white_piece_ = new Image("res/white-piece.png");

	button_ready_ = new ImageTextButton();
	static Image button_img("res/button-blue-middle.png");
	static Image button_hover_img("res/button-blue-middle-hover.png");
	static Image button_press_img("res/button-blue-middle-press.png");
	button_ready_->add_normal_image(&button_img, 0, 0);
	button_ready_->set_normal_text("准 备", button_text_size_);
	button_ready_->add_hover_image(&button_hover_img, 0, 0);
	button_ready_->set_hover_text("准 备", button_text_size_);
	button_ready_->add_press_image(&button_press_img, 0, 0);
	button_ready_->set_press_text("准 备", button_text_size_);
	button_ready_->set_recovery_flag(false);

	player_pic_ = NULL;
	is_ready_ = false;
	piece_type_ = Chess::EMPTY;
	pos_x_ = pos_y_ = 0;
	view_width_ = view_height_ = 0;
	buffer_img_.img = NULL;
}

NetPlayerInfoView::~NetPlayerInfoView()
{
	delete black_piece_;
	delete white_piece_;
	delete button_ready_;
}

void NetPlayerInfoView::calc_view_width_height()
{
	view_width_ = 0;
	int height = 0;
	if (player_pic_)
	{
		view_width_ = max(view_width_, player_pic_->get_width());
		height += player_pic_->get_height() + margin_;
	}

	Gobang::set_font(Gobang::font_default, name_font_size_, true);
	if (player_name_.length() > 0)
	{
		view_width_ = max(view_width_, textwidth(player_name_.c_str()));
		height += textheight(player_name_.c_str()) + margin_;
	}

	Gobang::set_font(Gobang::font_default, ready_font_size_);
	if (is_ready_)
	{
		view_width_ = max(view_width_, textwidth("Ready!"));
		height += textheight("Ready!") + margin_;
	}
	else if (!is_opposite_)
	{
		view_width_ = max(view_width_, button_ready_->get_width());
		height += button_ready_->get_height() + margin_;
	}
	view_height_ = height;
}

NetPlayerInfoView::ACTION_TYPE NetPlayerInfoView::action_judge(int x, int y)
{
	if (!is_ready_ && !is_opposite_ &&button_ready_->is_mouse_in_button(x, y))
		return ACTION_READY;
	return ACTION_NONE;
}

void NetPlayerInfoView::on_mouse_move(ACTION_TYPE type)
{
	switch (type)
	{
		case NetPlayerInfoView::ACTION_NONE:
			if (button_ready_->get_hover_status() && !is_ready_ && !is_opposite_)
				button_ready_->show();
			break;
		case NetPlayerInfoView::ACTION_READY:
			button_ready_->on_mouse_hover();
			break;
		default:
			break;
	}
}

void NetPlayerInfoView::on_mouse_click(ACTION_TYPE type)
{
	switch (type)
	{
		case NetPlayerInfoView::ACTION_READY:
			if (!is_ready_ && !is_opposite_)
				button_ready_->on_mouse_click();
			break;
		default:
			break;
	}
}

void NetPlayerInfoView::show()
{
	if (buffer_img_.img)
	{
		putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
		delimage(buffer_img_.img);
		buffer_img_.img = NULL;
	}
	calc_view_width_height();
	buffer_img_.x = pos_x_;
	buffer_img_.y = pos_y_;
	int temp_viewport_left, temp_viewport_right, temp_viewport_top, temp_viewport_bottom;
	getviewport(&temp_viewport_left, &temp_viewport_top, &temp_viewport_right, &temp_viewport_bottom);
	setviewport(pos_x_, pos_y_, pos_x_ + view_width_, pos_y_ + view_height_);
	buffer_img_.img = newimage(view_width_, view_height_);
	putimage(buffer_img_.img, 0, 0, NULL);
	setviewport(temp_viewport_left, temp_viewport_top, temp_viewport_right, temp_viewport_bottom);

	int target_top = pos_y_;
	if (player_pic_)
	{
		player_pic_->show_image_with_alpha(pos_x_ + view_width_ / 2 - player_pic_->get_width() / 2, target_top, 1.0);
		target_top += player_pic_->get_width() + margin_;
	}
	
	setcolor(WHITE);
	if (player_name_.length() > 0)
	{
		Gobang::set_font(Gobang::font_default, name_font_size_, true);
		xyprintf(pos_x_ + view_width_ / 2 - textwidth(player_name_.c_str()) / 2, target_top, player_name_.c_str());
		target_top += textheight(player_name_.c_str()) + margin_;
	}
	
	if (is_ready_)
	{
		Gobang::set_font(Gobang::font_default, ready_font_size_);
		xyprintf(pos_x_ + view_width_ / 2 - textwidth("Ready!") / 2, target_top, "Ready!");
	}
	else if (!is_opposite_)
	{
		button_ready_->set_position(pos_x_ + view_width_ / 2 - button_ready_->get_width() / 2, target_top);
		button_ready_->show();
	}
}
