#include "SettingsInterface.h"
#include "tools/Image.h"
#include "Chess.h"
#include "image_game_welcome_bg.h"
#include "image_game_settings_audio.h"
#include "image_game_settings_back.h"
#include "image_game_settings_select_black.h"
#include "image_game_settings_select_white.h"
#include "image_object_select.h"

SettingsInterface::SettingsInterface(Settings* settings)
{
	this->settings_ = settings;
}

void SettingsInterface::update_interface()
{
	static Image object_select(binary_object_select_png, sizeof(binary_object_select_png));
	static Image game_settings_bg(binary_game_welcome_bg_jpg, sizeof(binary_game_welcome_bg_jpg));

	game_settings_bg.show_image(0, 0);

	static Image settings_select_black(binary_game_settings_select_black_png, sizeof(binary_game_settings_select_black_png));
	settings_select_black.show_image_with_alpha(30, 350, 0.9);
	if (settings_->get_piece_color() == Chess::BLACK)
		object_select.show_image_with_alpha(90, 480, 1.0);

	static Image settings_select_white(binary_game_settings_select_white_png, sizeof(binary_game_settings_select_white_png));
	settings_select_white.show_image_with_alpha(260, 350, 0.9);
	if (settings_->get_piece_color() == Chess::WHITE)
		object_select.show_image_with_alpha(320, 480, 1.0);

	static Image settings_audio(binary_game_settings_audio_png, sizeof(binary_game_settings_audio_png));
	settings_audio.show_image_with_alpha(30, 520, 0.9);
	if (settings_->is_audio_on())
		object_select.show_image_with_alpha(90, 630, 1.0);

	static Image settings_back(binary_game_settings_back_png, sizeof(binary_game_settings_back_png));
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
