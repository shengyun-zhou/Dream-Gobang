#include "NetSettingsInterface.h"
#include "Gobang.h"

Image* NetSettingsInterface::bg_img_ = NULL;
ImageTextButton* NetSettingsInterface::button_save_ = NULL;
ImageTextButton* NetSettingsInterface::button_cancel_ = NULL;
ImageTextButton* NetSettingsInterface::button_edit_[NetSettingsInterface::edit_button_num_] = { NULL };

NetSettingsInterface::NetSettingsInterface(Settings* settings)
{
	game_settings_ = settings;
	static Image* button_img = new Image("res/button-blue-middle.png");
	static Image* button_hover_img = new Image("res/button-blue-middle-hover.png");
	static Image* button_press_img = new Image("res/button-blue-middle-press.png");
	if (!bg_img_)
		bg_img_ = new Image("res/game-network-bg.jpg");
	if (!button_save_)
	{
		button_save_ = new ImageTextButton();
		button_save_->add_normal_image(button_img, 0, 0);
		button_save_->set_normal_text("保 存", button_text_size_);
		button_save_->add_hover_image(button_hover_img, 0, 0);
		button_save_->set_hover_text("保 存", button_text_size_);
		button_save_->add_press_image(button_press_img, 0, 0);
		button_save_->set_press_text("保 存", button_text_size_);
		button_save_->set_recovery_flag(false);
	}
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
	static Image* text_edit = new Image("res/text-edit.png");
	for (int i = 0; i < edit_button_num_; i++)
	{
		if (!button_edit_[i])
		{
			button_edit_[i] = new ImageTextButton();
			button_edit_[i]->add_normal_image(button_img, 0, 0);
			button_edit_[i]->set_normal_text("更 改", button_text_size_);
			button_edit_[i]->add_hover_image(button_hover_img, 0, 0);
			button_edit_[i]->set_hover_text("更 改", button_text_size_);
			button_edit_[i]->add_press_image(button_press_img, 0, 0);
			button_edit_[i]->set_press_text("更 改", button_text_size_);
			button_edit_[i]->set_recovery_flag(false);
		}
	}
}

NetSettingsInterface::~NetSettingsInterface()
{
}

void NetSettingsInterface::show_interface()
{
	bg_img_->show_image(0, 0);
	PIMAGE black_bg = newimage(1, 1);
	setbkcolor(BLACK, black_bg);
	cleardevice(black_bg);
	putimage_rotatezoom(NULL, black_bg, 0, 0, 0, 0, 0, 
										  (float)(Gobang::WINDOW_WIDTH < Gobang::WINDOW_HEIGHT)?Gobang::WINDOW_HEIGHT:Gobang::WINDOW_WIDTH, 0, 130);
	delimage(black_bg);

	Gobang::set_font(Gobang::font_default, 40);
	setcolor(WHITE);

	xyprintf(20, 20, "网 络 设 置");
	setlinestyle(SOLID_LINE, 0, 5);
	line(20, 80, Gobang::WINDOW_WIDTH - 20, 80);

	Gobang::set_font(Gobang::font_default, font_size_level_1_);
	int text_height_1 = textheight("A");
	Gobang::set_font(Gobang::font_default, font_size_level_2_);
	int text_height_2 = textheight("A");

	int text_width;
	int target_top = 100;
	Gobang::set_font(Gobang::font_default, font_size_level_1_);
	xyprintf(text_indent_, target_top, "用户设置");
	target_top += text_height_1 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_2_);
	xyprintf(text_indent_ * 2, target_top, "用户名: %s", game_settings_->get_user_name().c_str());
	text_width = textwidth((game_settings_->get_user_name() + "用户名: ").c_str());
	button_edit_[0]->set_position(text_indent_ * 2 + text_width + button_margin_,
																target_top - (button_edit_[0]->get_height() - text_height_2) / 2);
	button_edit_[0]->show();
	target_top += text_height_2 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_1_);
	xyprintf(text_indent_ , target_top, "开设房间-服务端设置");
	target_top += text_height_1 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_2_);
	xyprintf(text_indent_ * 2, target_top, "端口号: %s", game_settings_->get_server_port().c_str());
	text_width = textwidth((game_settings_->get_server_port() + "端口号: ").c_str());
	button_edit_[1]->set_position(text_indent_ * 2 + text_width + button_margin_,
		target_top - (button_edit_[1]->get_height() - text_height_2) / 2);
	button_edit_[1]->show();
	target_top += text_height_2 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_1_);
	xyprintf(text_indent_, target_top, "进入房间-客户端设置");
	target_top += text_height_1 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_2_);
	xyprintf(text_indent_ * 2, target_top, "连接目标IP地址: %s", game_settings_->get_client_connect_IP_addr().c_str());
	text_width = textwidth((game_settings_->get_client_connect_IP_addr() + "连接目标IP地址: ").c_str());
	button_edit_[2]->set_position(text_indent_ * 2 + text_width + button_margin_,
		target_top - (button_edit_[2]->get_height() - text_height_2) / 2);
	button_edit_[2]->show();
	target_top += text_height_2 + text_margin_;

	Gobang::set_font(Gobang::font_default, font_size_level_2_);
	xyprintf(text_indent_ * 2, target_top, "连接目标端口号: %s", game_settings_->get_client_connect_port().c_str());
	text_width = textwidth((game_settings_->get_client_connect_port() + "连接目标端口号: ").c_str());
	button_edit_[3]->set_position(text_indent_ * 2 + text_width + button_margin_,
		target_top - (button_edit_[3]->get_height() - text_height_2) / 2);
	button_edit_[3]->show();
	target_top += text_height_2 + text_margin_;

	button_save_->set_position(Gobang::WINDOW_WIDTH - 20 - button_cancel_->get_width() - button_margin_ -  button_save_->get_width(), 
														 80 - button_margin_ - button_save_->get_height());
	button_save_->show();

	button_cancel_->set_position(Gobang::WINDOW_WIDTH - 20 - button_cancel_->get_width(), 80 - button_margin_ - button_cancel_->get_height());
	button_cancel_->show();
}

NetSettingsInterface::ACTION_TYPE NetSettingsInterface::action_judge(int x, int y)
{
	if (button_save_->is_mouse_in_button(x, y))
		return ACTION_SAVE;
	else if (button_cancel_->is_mouse_in_button(x, y))
		return ACTION_CANCEL;
	for (int i = 0; i < edit_button_num_; i++)
	{
		if (button_edit_[i]->is_mouse_in_button(x, y))
		{
			switch (i)
			{
				case 0:
					return ACTION_EDIT_USER_NAME;
				case 1:
					return ACTION_EDIT_SERVER_PORT;
				case 2:
					return ACTION_EDIT_CLIENT_CONNECT_IP;
				case 3:
					return ACTION_EDIT_CLIENT_CONNECT_PORT;
				default:
					break;
			}
		}
	}
	return ACTION_NONE;
}

void NetSettingsInterface::on_mouse_move(ACTION_TYPE type)
{
	switch (type)
	{
		case ACTION_NONE:
			if (button_save_->get_hover_status())
				button_save_->show();
			if (button_cancel_->get_hover_status())
				button_cancel_->show();
			for (int i = 0; i < edit_button_num_; i++)
			{
				if (button_edit_[i]->get_hover_status())
					button_edit_[i]->show();
			}
			break;
		case ACTION_SAVE:
			button_save_->on_mouse_hover();
			break;
		case ACTION_CANCEL:
			button_cancel_->on_mouse_hover();
			break;
		case ACTION_EDIT_USER_NAME:
		case ACTION_EDIT_SERVER_PORT:
		case ACTION_EDIT_CLIENT_CONNECT_IP:
		case ACTION_EDIT_CLIENT_CONNECT_PORT:
			button_edit_[(int)type - (int)ACTION_EDIT_USER_NAME]->on_mouse_hover();
			break;
		default:
			break;
	}
}

void NetSettingsInterface::on_mouse_click(ACTION_TYPE type)
{
	switch (type)
	{
		case ACTION_SAVE:
			button_save_->on_mouse_click();
			break;
		case ACTION_CANCEL:
			button_cancel_->on_mouse_click();
			break;
		case ACTION_EDIT_USER_NAME:
		case ACTION_EDIT_SERVER_PORT:
		case ACTION_EDIT_CLIENT_CONNECT_IP:
		case ACTION_EDIT_CLIENT_CONNECT_PORT:
			button_edit_[(int)type - (int)ACTION_EDIT_USER_NAME]->on_mouse_click();
			break;
		default:
			break;
	}
}
