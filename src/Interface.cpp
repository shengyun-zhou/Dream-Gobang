#include "Interface.h"

void Interface::animation()																	//显示开场动画
{
	//显示开场动画
	static Image game_logo("res/game-logo.png");
	const int image_width = 500;
	const int image_height = 222;
	int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
	for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i > 255)
			i = 255;
		setbkcolor_f(EGERGB(color, color, color));										//由黑到白逐渐改变背景色
		cleardevice();
		/*由完全透明到不透明渲染Logo图片*/
		game_logo.show_image_with_alpha(640 - image_width / 2, 320 - image_height / 2, (double)i / 255.0);
	}

	delay_ms(1500);
	deltaAlpha *= -1;
	deltaColor *= -1;
	/*下面的动画为上面动画的逆过程*/
	for (color = 255, i = 255 + (-1) * deltaAlpha; color >= 0; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i < 0)
			i = 0;
		setbkcolor_f(EGERGB(color, color, color));
		cleardevice();
		game_logo.show_image_with_alpha(640 - image_width / 2, 320 - image_height / 2, (double)i / 255.0);
	}
	delay_ms(100);
}
	

void Interface::welcome()						
{
	static Image game_welcom_bg("res/game-welcom-bg.jpg");
	game_welcom_bg.show_image(0, 0);
	static Image button_enter_game("res/button-enter-game.png");
	static Image button_game_settings("res/button-game-settings.png");
	static Image button_exit_game("res/button-exit-game.png");
	static Image button_special_thanks("res/button-special-thanks.png");

	setrendermode(RENDER_MANUAL);

	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(x, 350, 0.85);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(80, 350, 0.85);
		button_game_settings.show_image_with_alpha(x, 450, 0.85);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(80, 350, 0.85);
		button_game_settings.show_image_with_alpha(80, 450, 0.85);
		button_exit_game.show_image_with_alpha(x, 550, 0.85);
		x += 10;
	}
	
	button_special_thanks.show_image_with_alpha(1000, 10, 1.0);
}


void Interface::set_welcome()
{
	static Image game_welcom_bg("res/game-welcom-bg.jpg");
	game_welcom_bg.show_image(0, 0);

	static Image settings_select_black("res/game-settings-select-black.png");
	settings_select_black.show_image_with_alpha(30, 350, 0.9);

	static Image settings_select_white("res/game-settings-select-white.png");
	settings_select_white.show_image_with_alpha(260, 350, 0.9);

	static Image settings_audio("res/game-settings-audio.png");
	settings_audio.show_image_with_alpha(30, 520, 0.9);

	static Image settings_back("res/game-settings-back.png");
	settings_back.show_image_with_alpha(260, 500, 0.9);
}


void Interface::Curtain()
{
	setbkmode(TRANSPARENT);
	static Image game_thanks_bg("res/game-special-thanks-bg.png");
	game_thanks_bg.show_image(0, 0);
	//setfont(40, 30, "方正喵呜体");
	setfont(-40, 0, "微软雅黑");
	setcolor(WHITE);

	xyprintf(10, 80, "特别鸣谢");

	delay_ms(1000);

	outtextxy(30, 180, "周圣云 = 大神带你飞");

	delay_ms(1000);

	outtextxy(30, 300, "陆爽秋 = 球球的编码新时代");

	delay_ms(1000);

	outtextxy(30, 400, "桂欢 = 欢神帮帮忙");

	delay_ms(1000);

	outtextxy(30, 500, "苏月 = 速度君");

	delay_ms(1000);

	outtextxy(30, 600, "何宇 = 图标小达人");
	delay_ms(1000);

	outtextxy(800, 600, "返回");

}



