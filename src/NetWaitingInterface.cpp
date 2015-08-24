#include "NetWaitingInterface.h"
#include "Gobang.h"

Image* NetWaitingInterface::bg_img_ = NULL;
ImageButton* NetWaitingInterface::button_cancel_ = NULL;

NetWaitingInterface::NetWaitingInterface()
{
	if (!bg_img_)
		bg_img_ = new Image("res/game-network-bg.jpg");
	static Image* button_img = new Image("res/button-blue-middle.png");
	static Image* button_hover_img = new Image("res/button-blue-middle-hover.png");
	static Image* button_press_img = new Image("res/button-blue-middle-press.png");
	if (!button_cancel_)
	{
		button_cancel_ = new ImageButton();
		Image* button_text = new Image("res/text-cancel.png");
		button_cancel_->add_normal_image(button_img, 0, 0);
		button_cancel_->add_normal_image(button_text, text_x_offset, text_y_offset);
		button_cancel_->add_hover_image(button_hover_img, 0, 0);
		button_cancel_->add_hover_image(button_text, text_x_offset, text_y_offset);
		button_cancel_->add_press_image(button_press_img, 0, 0);
		button_cancel_->add_press_image(button_text, text_x_offset, text_y_offset);
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

	LOGFONT system_font;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &system_font, NULL);
	system_font.lfQuality = ANTIALIASED_QUALITY;
	system_font.lfHeight = -tip_text_font_size_;
	setfont(&system_font);

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
