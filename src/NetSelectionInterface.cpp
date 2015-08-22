#include "NetSelectionInterface.h"
#include "Gobang.h"
#include "tools/GradientAnimation.h"

ImageButton* NetSelectionInterface::button_open_room_ = NULL;
ImageButton* NetSelectionInterface::button_enter_room_ = NULL;
ImageButton* NetSelectionInterface::button_net_settings_ = NULL;
ImageButton* NetSelectionInterface::button_back_ = NULL;
Image* NetSelectionInterface::bg_img_ = NULL;

NetSelectionInterface::NetSelectionInterface()
{
	if (!button_open_room_)
	{
		button_open_room_ = new ImageButton();
		Image* text_open_room = new Image("res/text-open-room.png");
		button_open_room_->add_normal_image(new Image("res/button-blue.png"), 0, 0);
		button_open_room_->add_normal_image(text_open_room, text_x_offset_, text_y_offset_);
		button_open_room_->add_hover_image(new Image("res/button-blue-hover.png"), 0, 0);
		button_open_room_->add_hover_image(text_open_room, text_x_offset_, text_y_offset_);
		button_open_room_->add_press_image(new Image("res/button-blue-press.png"), 0, 0);
		button_open_room_->add_press_image(text_open_room, text_x_offset_, text_y_offset_);
	}
	if (!button_enter_room_)
	{
		button_enter_room_ = new ImageButton();
		Image* button_text = new Image("res/text-enter-room.png");
		button_enter_room_->add_normal_image(new Image("res/button-blue.png"), 0, 0);
		button_enter_room_->add_normal_image(button_text, text_x_offset_, text_y_offset_);
		button_enter_room_->add_hover_image(new Image("res/button-blue-hover.png"), 0, 0);
		button_enter_room_->add_hover_image(button_text, text_x_offset_, text_y_offset_);
		button_enter_room_->add_press_image(new Image("res/button-blue-press.png"), 0, 0);
		button_enter_room_->add_press_image(button_text, text_x_offset_, text_y_offset_);
	}
	if (!button_net_settings_)
	{
		button_net_settings_ = new ImageButton();
		Image* button_text = new Image("res/text-net-settings.png");
		button_net_settings_->add_normal_image(new Image("res/button-blue.png"), 0, 0);
		button_net_settings_->add_normal_image(button_text, text_x_offset_, text_y_offset_);
		button_net_settings_->add_hover_image(new Image("res/button-blue-hover.png"), 0, 0);
		button_net_settings_->add_hover_image(button_text, text_x_offset_, text_y_offset_);
		button_net_settings_->add_press_image(new Image("res/button-blue-press.png"), 0, 0);
		button_net_settings_->add_press_image(button_text, text_x_offset_, text_y_offset_);
	}
	if (!button_back_)
	{
		button_back_ = new ImageButton();
		Image* button_text = new Image("res/text-back.png");
		button_back_->add_normal_image(new Image("res/button-blue.png"), 0, 0);
		button_back_->add_normal_image(button_text, text_x_offset_, text_y_offset_);
		button_back_->add_hover_image(new Image("res/button-blue-hover.png"), 0, 0);
		button_back_->add_hover_image(button_text, text_x_offset_, text_y_offset_);
		button_back_->add_press_image(new Image("res/button-blue-press.png"), 0, 0);
		button_back_->add_press_image(button_text, text_x_offset_, text_y_offset_);
	}
	if (!bg_img_)
		bg_img_ = new Image("res/game-network-bg.jpg");
}


NetSelectionInterface::~NetSelectionInterface()
{
	RemoveFontResource("res/font-llt.ttc");
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
	AddFontResource("res/font-llt.ttc");									//加载字体文件
	setfont(-40, 0, "萝莉体 第二版");
	static LOGFONTA current_font;
	getfont(&current_font);
	current_font.lfQuality = ANTIALIASED_QUALITY;					//开启字体抗锯齿
	setfont(&current_font);
	setcolor(WHITE);

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
