#pragma once
#include "Settings.h"
class SettingsInterface
{
private:
	Settings* settings_;
public:
	enum action_type{
		ACTION_NONE,
		ACTION_SELECT_BLACK,
		ACTION_SELECT_WHITE,
		ACTION_SWITCH_AUDIO,
		ACTION_BACK
	};


	SettingsInterface(Settings* settings);
	action_type action_judge(int x, int y);

	void update_interface();

};

