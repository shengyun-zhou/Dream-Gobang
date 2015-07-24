#include "SettingsInterface.h"
#include "Image.h"
#include "Chess.h"

SettingsInterface::SettingsInterface(Settings* settings)
{
	this->settings_ = settings;
}

void SettingsInterface::update_interface()
{
	static Image object_select("res/object-select.png");
	static Image game_settings_bg("res/game-welcom-bg.jpg");

	game_settings_bg.show_image(0, 0);

	static Image settings_select_black("res/game-settings-select-black.png");
	settings_select_black.show_image_with_alpha(30, 350, 0.9);
	if (settings_->get_piece_color() == Chess::BLACK)
		object_select.show_image_with_alpha(90, 480, 1.0);

	static Image settings_select_white("res/game-settings-select-white.png");
	settings_select_white.show_image_with_alpha(260, 350, 0.9);
	if (settings_->get_piece_color() == Chess::WHITE)
		object_select.show_image_with_alpha(320, 480, 1.0);

	static Image settings_audio("res/game-settings-audio.png");
	settings_audio.show_image_with_alpha(30, 520, 0.9);
	if (settings_->is_audio_on())
		object_select.show_image_with_alpha(90, 630, 1.0);

	static Image settings_back("res/game-settings-back.png");
	settings_back.show_image_with_alpha(260, 500, 0.9);
}

SettingsInterface::action_type SettingsInterface::action_judge(int x, int y)
{
	const int offset = 10;
	//1.设置黑子
	if (x > 30 - offset && x < 180 + offset && y > 350 - offset && y < 450 + offset)
		return ACTION_SELECT_BLACK;

	//2.选择白子
	if (x > 260 - offset && x < 410 + offset && y>350 - offset && y < 450 + offset)
		return ACTION_SELECT_WHITE;

	//3.设置音效
	if (x > 30 - offset && x < 180 - offset && y>520 - offset && y < 600 + offset)
		return ACTION_SWITCH_AUDIO;

	//4.返回
	if (x > 260 - offset && x < 410 - offset && y>500 - offset && y < 600 + offset)
		return ACTION_BACK;
	return ACTION_NONE;
}
