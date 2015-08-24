#pragma once
#include "widgets/ImageButton.h"
#include "tools/Image.h"
#include <string>

using namespace std;

class NetWaitingInterface
{
private:
	static Image* bg_img_;
	static ImageButton* button_cancel_;

	static const int text_x_offset = 25;
	static const int text_y_offset = 7;
	static const int tip_text_font_size_ = 30;
	static const int margin_ = 15;
	string tip_text_;
public:
	NetWaitingInterface();
	~NetWaitingInterface();

	void set_tip_text(const char* text)
	{
		tip_text_ = text;
	}

	enum ACTION_TYPE
	{
		ACTION_NONE,
		ACTION_CANCEL
	};

	ACTION_TYPE action_judge(int x, int y);
	void on_mouse_move(ACTION_TYPE type);
	void on_mouse_click(ACTION_TYPE type);
	void show_interface();
};

