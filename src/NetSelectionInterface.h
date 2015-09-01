#pragma once
#include "widgets/ImageTextButton.h"
class NetSelectionInterface
{
private:
	static ImageTextButton* button_open_room_;
	static ImageTextButton* button_enter_room_;
	static ImageTextButton* button_net_settings_;
	static ImageTextButton* button_back_;
	static Image* bg_img_;

	static const int button_margin_ = 60;
	static const int button_text_size_ = 35;
	static const int height_offset_ = 60;
public:
	enum ACTION_TYPE
	{
		ACTION_NONE,
		ACTION_OPEN_ROOM,
		ACTION_ENTER_ROOM,
		ACTION_NET_SETTINGS,
		ACTION_BACK
	};
	NetSelectionInterface();
	~NetSelectionInterface();

	void enter_interface();
	void show_interface();
	ACTION_TYPE action_judge(int x, int y);
	void on_mouse_move(ACTION_TYPE action_type);
	void on_mouse_click(ACTION_TYPE action_type);
};

