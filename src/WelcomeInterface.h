#pragma once
#include "PlayChess.h"
#include "ComputerPlayer.h"
#include "Settings.h"
#include "tools/Image.h"
#include "widgets/ImageButton.h"
#include "widgets/ImageTextButton.h"

class WelcomeInterface
{
private:
	static const int button_text_size_ = 28;
	static const int text_x_offset_ = 75;

	static Image* game_welcome_bg_;
	static ImageTextButton* button_enter_game_;
	static ImageTextButton* button_game_settings_;
	static ImageTextButton* button_exit_game_;
	static ImageButton* button_special_thanks_;
	static ImageButton* button_net_play_;

public:
	enum action_type{
		ACTION_NONE,
		ACTION_ENTER_GAME,
		ACTION_GAME_SETTINGS,
		ACTION_EXIT_GAME,
		ACTION_SPECIAL_THANKS,
		ACTION_NET_PLAY
	};

	WelcomeInterface();
	static void opening_animation();
	void enter_interface();
	void show_welcome();
	static void curtain();

	action_type action_judge(int x, int y);

	void on_mouse_move(action_type action);
	void on_mouse_click(action_type action);
};
