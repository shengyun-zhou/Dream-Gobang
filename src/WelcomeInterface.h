#pragma once
#include "PlayChess.h"
#include "ComputerPlayer.h"
#include "Settings.h"
#include "tools/Image.h"
#include "widgets/ImageButton.h"

class WelcomeInterface
{
private:
	static const int text_x_offset_ = 65;
	static const int text_y_offset_ = 12;

	static Image* game_welcome_bg_;
	static ImageButton* button_enter_game_;
	static ImageButton* button_game_settings_;
	static ImageButton* button_exit_game_;
	static ImageButton* button_special_thanks_;

public:
	enum action_type{
		ACTION_NONE,
		ACTION_ENTER_GAME,
		ACTION_GAME_SETTINGS,
		ACTION_EXIT_GAME,
		ACTION_SPECIAL_THANKS
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
