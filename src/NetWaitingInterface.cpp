#include "NetWaitingInterface.h"
#include "Gobang.h"
#include "image_button_blue_middle.h"
#include "image_button_blue_middle_hover.h"
#include "image_button_blue_middle_press.h"
#include "image_game_network_bg.h"

Image* NetWaitingInterface::bg_img_ = NULL;
ImageTextButton* NetWaitingInterface::button_cancel_ = NULL;

NetWaitingInterface::NetWaitingInterface()
{
	if (!bg_img_)
		bg_img_ = new Image(binary_game_network_bg_jpg, sizeof(binary_game_network_bg_jpg));
	static Image* button_img = new Image(binary_button_blue_middle_png, sizeof(binary_button_blue_middle_png));
	static Image* button_hover_img = new Image(binary_button_blue_middle_hover_png, sizeof(binary_button_blue_middle_hover_png));
	static Image* button_press_img = new Image(binary_button_blue_middle_press_png, sizeof(binary_button_blue_middle_press_png));
	if (!button_cancel_)
	{
		button_cancel_ = new ImageTextButton();
		button_cancel_->add_normal_image(button_img, 0, 0);
		button_cancel_->set_normal_text("取 消", button_text_size_);
		button_cancel_->add_hover_image(button_hover_img, 0, 0);
		button_cancel_->set_hover_text("取 消", button_text_size_);
		button_cancel_->add_press_image(button_press_img, 0, 0);
		button_cancel_->set_press_text("取 消", button_text_size_);
		button_cancel_->set_recovery_flag(false);
	}
}

NetWaitingInterface::~NetWaitingInterface()
{
}

NetWaitingInterface::ACTION_TYPE NetWaitingInterface::action_judge(int x, int y)
{
	if (button_cancel_->is_mouse_in_button(x, y))
		return ACTION_CANCEL;
	return ACTION_NONE;
}

void NetWaitingInterface::show_interface()
{
	bg_img_->show_image(0, 0);
	PIMAGE black_bg = newimage(1, 1);
	setbkcolor(BLACK, black_bg);
	cleardevice(black_bg);
	putimage_rotatezoom(NULL, black_bg, 0, 0, 0, 0, 0,
		(float)(Gobang::WINDOW_WIDTH < Gobang::WINDOW_HEIGHT) ? Gobang::WINDOW_HEIGHT : Gobang::WINDOW_WIDTH, 0, 130);
	delimage(black_bg);

	Gobang::set_font(Gobang::font_default, tip_text_font_size_);
	setbkmode(TRANSPARENT);
	int target_top = Gobang::WINDOW_HEIGHT / 2 - 100;
	xyprintf((Gobang::WINDOW_WIDTH - textwidth(tip_text_.c_str())) / 2, target_top, tip_text_.c_str());
	target_top += textheight(tip_text_.c_str()) + margin_;
	button_cancel_->set_position((Gobang::WINDOW_WIDTH - button_cancel_->get_width()) / 2, target_top);
	button_cancel_->show();
}

void NetWaitingInterface::on_mouse_move(ACTION_TYPE type)
{
	switch (type)
	{
		case ACTION_NONE:
			if (button_cancel_->get_hover_status())
				button_cancel_->show();
			break;
		case ACTION_CANCEL:
			button_cancel_->on_mouse_hover();
			break;
		default:
			break;
	}
}

void NetWaitingInterface::on_mouse_click(ACTION_TYPE type)
{
	switch (type)
	{
		case ACTION_CANCEL:
			button_cancel_->on_mouse_click();
			break;
		default:
			break;
	}
}
