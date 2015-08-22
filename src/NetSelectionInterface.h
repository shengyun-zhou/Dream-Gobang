#pragma once
#include "widgets/ImageButton.h"
class NetSelectionInterface
{
private:
	static ImageButton* button_open_room_;
	static ImageButton* button_enter_room_;
	static ImageButton* button_net_settings_;
	static ImageButton* button_back_;
	static Image* bg_img_;

	static const int button_margin_ = 60;
	static const int text_x_offset_ = 50;
	static const int text_y_offset_ = 12;
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

