#pragma once
#include "Save.h"
#include "PlayChess.h"
#include "ComputerPlayer.h"
#include "Settings.h"
#include "Image.h"

class Interface
{
private:
	static const int text_x_offset = 65;
	static const int text_y_offset = 12;
	static const double button_alpha;

	static Image* button_enter_game;
	static Image* button_hover_enter_game;
	static Image* button_press_enter_game;
	static Image*	text_enter_game;
	static Image* button_game_settings;
	static Image* button_hover_game_settings;
	static Image* button_press_game_settings;
	static Image* text_game_settings;
	static Image* button_exit_game;
	static Image* button_hover_exit_game;
	static Image* button_press_exit_game;
	static Image* text_exit_game;
	static Image* button_special_thanks;
	static Image* game_welcome_bg;

	bool button_enter_game_flag_;
	bool button_game_settings_flag_;
	bool button_exit_game_flag_;

public:
	enum action_type{
		ACTION_NONE,
		ACTION_ENTER_GAME,
		ACTION_GAME_SETTINGS,
		ACTION_EXIT_GAME,
		ACTION_SPECIAL_THANKS
	};

	Interface();
	void animation();
	void welcome();
	void set_welcome(Settings* settings);
	void Curtain();

	action_type action_judge(int x, int y);
	void on_mouse_move(action_type action);
	void on_mouse_click(action_type action);
};
