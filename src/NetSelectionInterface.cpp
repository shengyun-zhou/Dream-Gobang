#include "NetSelectionInterface.h"
#include "Gobang.h"
#include "tools/GradientAnimation.h"
#include "image_game_network_bg.h"
#include "image_button_blue.h"
#include "image_button_blue_hover.h"
#include "image_button_blue_press.h"

ImageTextButton* NetSelectionInterface::button_open_room_ = NULL;
ImageTextButton* NetSelectionInterface::button_enter_room_ = NULL;
ImageTextButton* NetSelectionInterface::button_net_settings_ = NULL;
ImageTextButton* NetSelectionInterface::button_back_ = NULL;
Image* NetSelectionInterface::bg_img_ = NULL;

NetSelectionInterface::NetSelectionInterface()
{
  static Image* button_img = new Image(binary_button_blue_png, sizeof(binary_button_blue_png));
	static Image* button_hover_img = new Image(binary_button_blue_hover_png, sizeof(binary_button_blue_hover_png));
	static Image* button_press_img = new Image(binary_button_blue_press_png, sizeof(binary_button_blue_press_png));
	if (!button_open_room_)
	{
		button_open_room_ = new ImageTextButton();
		button_open_room_->add_normal_image(button_img, 0, 0);
		button_open_room_->set_normal_text("开 设 房 间", button_text_size_, -1, -1, Gobang::font_llt);
		button_open_room_->add_hover_image(button_hover_img, 0, 0);
		button_open_room_->set_hover_text("开 设 房 间", button_text_size_, -1, -1, Gobang::font_llt);
		button_open_room_->add_press_image(button_press_img, 0, 0);
		button_open_room_->set_press_text("开 设 房 间", button_text_size_, -1, -1, Gobang::font_llt);
	}
	if (!button_enter_room_)
	{
		button_enter_room_ = new ImageTextButton();
		button_enter_room_->add_normal_image(button_img, 0, 0);
		button_enter_room_->set_normal_text("进 入 房 间", button_text_size_, -1, -1, Gobang::font_llt);
		button_enter_room_->add_hover_image(button_hover_img, 0, 0);
		button_enter_room_->set_hover_text("进 入 房 间", button_text_size_, -1, -1, Gobang::font_llt);
		button_enter_room_->add_press_image(button_press_img, 0, 0);
		button_enter_room_->set_press_text("进 入 房 间", button_text_size_, -1, -1, Gobang::font_llt);
	}
	if (!button_net_settings_)
	{
		button_net_settings_ = new ImageTextButton();
		button_net_settings_->add_normal_image(button_img, 0, 0);
		button_net_settings_->set_normal_text("网 络 设 置", button_text_size_, -1, -1, Gobang::font_llt);
		button_net_settings_->add_hover_image(button_hover_img, 0, 0);
		button_net_settings_->set_hover_text("网 络 设 置", button_text_size_, -1, -1, Gobang::font_llt);
		button_net_settings_->add_press_image(button_press_img, 0, 0);
		button_net_settings_->set_press_text("网 络 设 置", button_text_size_, -1, -1, Gobang::font_llt);
	}
	if (!button_back_)
	{
		button_back_ = new ImageTextButton();
		button_back_->add_normal_image(button_img, 0, 0);
		button_back_->set_normal_text("返回主界面", button_text_size_, -1, -1, Gobang::font_llt);
		button_back_->add_hover_image(button_hover_img, 0, 0);
		button_back_->set_hover_text("返回主界面", button_text_size_, -1, -1, Gobang::font_llt);
		button_back_->add_press_image(button_press_img, 0, 0);
		button_back_->set_press_text("返回主界面", button_text_size_, -1, -1, Gobang::font_llt);
	}
	if (!bg_img_)
		bg_img_ = new Image(binary_game_network_bg_jpg, sizeof(binary_game_network_bg_jpg));
}


NetSelectionInterface::~NetSelectionInterface()
{
}

void NetSelectionInterface::enter_interface()
{
	GradientAnimation::transition_ease_out(bg_img_);
	show_interface();
}

void NetSelectionInterface::show_interface()
{
	bg_img_->show_image(0, 0);

	setbkmode(TRANSPARENT);
	setcolor(WHITE);
	Gobang::set_font(Gobang::font_llt, 40);

	xyprintf(20, 20, "网 络 对 战");
	setlinestyle(SOLID_LINE, 0, 5);
	line(20, 80, Gobang::WINDOW_WIDTH - 20, 80);

	button_open_room_->set_position(Gobang::WINDOW_WIDTH / 2 - button_margin_ / 2 - button_open_room_->get_width(),
		Gobang::WINDOW_HEIGHT / 2 - button_margin_ / 2 - button_open_room_->get_height() - height_offset_);
	button_open_room_->show();

	button_enter_room_->set_position(Gobang::WINDOW_WIDTH / 2 + button_margin_ / 2,
		Gobang::WINDOW_HEIGHT / 2 - button_margin_ / 2 - button_enter_room_->get_height() - height_offset_);
	button_enter_room_->show();

	button_net_settings_->set_position(Gobang::WINDOW_WIDTH / 2 - button_margin_ / 2 - button_net_settings_->get_width(),
		Gobang::WINDOW_HEIGHT / 2 + button_margin_ / 2 - height_offset_);
	button_net_settings_->show();

	button_back_->set_position(Gobang::WINDOW_WIDTH / 2 + button_margin_ / 2,
		Gobang::WINDOW_HEIGHT / 2 + button_margin_ / 2 - height_offset_);
	button_back_->show();
}

NetSelectionInterface::ACTION_TYPE NetSelectionInterface::action_judge(int x, int y)
{
	if (button_open_room_->is_mouse_in_button(x, y))
		return ACTION_OPEN_ROOM;
	else if (button_enter_room_->is_mouse_in_button(x, y))
		return ACTION_ENTER_ROOM;
	else if (button_net_settings_->is_mouse_in_button(x, y))
		return ACTION_NET_SETTINGS;
	else if (button_back_->is_mouse_in_button(x, y))
		return ACTION_BACK;
	else
		return ACTION_NONE;
}

void NetSelectionInterface::on_mouse_move(NetSelectionInterface::ACTION_TYPE action_type)
{
	switch (action_type)
	{
		case ACTION_NONE:
			if (button_open_room_->get_hover_status())
				button_open_room_->show();
			if (button_enter_room_->get_hover_status())
				button_enter_room_->show();
			if (button_net_settings_->get_hover_status())
				button_net_settings_->show();
			if (button_back_->get_hover_status())
				button_back_->show();
			break;
		case ACTION_OPEN_ROOM:
			button_open_room_->on_mouse_hover();
			break;
		case ACTION_ENTER_ROOM:
			button_enter_room_->on_mouse_hover();
			break;
		case ACTION_NET_SETTINGS:
			button_net_settings_->on_mouse_hover();
			break;
		case ACTION_BACK:
			button_back_->on_mouse_hover();
			break;
	}
}

void NetSelectionInterface::on_mouse_click(NetSelectionInterface::ACTION_TYPE action_type)
{
	switch (action_type)
	{
	case ACTION_NONE:
		if (button_open_room_->get_hover_status())
			button_open_room_->show();
		if (button_enter_room_->get_hover_status())
			button_enter_room_->show();
		if (button_net_settings_->get_hover_status())
			button_net_settings_->show();
		if (button_back_->get_hover_status())
			button_back_->show();
		break;
	case ACTION_OPEN_ROOM:
		button_open_room_->on_mouse_click();
		break;
	case ACTION_ENTER_ROOM:
		button_enter_room_->on_mouse_click();
		break;
	case ACTION_NET_SETTINGS:
		button_net_settings_->on_mouse_click();
		break;
	case ACTION_BACK:
		button_back_->on_mouse_click();
		break;
	}
}
