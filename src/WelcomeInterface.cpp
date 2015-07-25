#include "WelcomeInterface.h"

ImageButton* WelcomeInterface::button_enter_game_ = NULL;
ImageButton* WelcomeInterface::button_game_settings_ = NULL;
ImageButton* WelcomeInterface::button_exit_game_ = NULL;
ImageButton* WelcomeInterface::button_special_thanks_ = NULL;
Image* WelcomeInterface::game_welcome_bg_ = NULL;

WelcomeInterface::WelcomeInterface()
{
	//加载背景图片
	if (!game_welcome_bg_)
		game_welcome_bg_ = new Image("res/game-welcom-bg.jpg");

	//加载所有按钮
	if (!button_enter_game_)
	{
		button_enter_game_ = new ImageButton();
		Image* text_enter_game = new Image("res/text-enter-game.png");
		button_enter_game_->add_normal_image(new Image("res/button-enter-game.png"), 0, 0);
		button_enter_game_->add_normal_image(text_enter_game, text_x_offset_, text_y_offset_);
		button_enter_game_->add_hover_image(new Image("res/button-hover-enter-game.png"), 0, 0);
		button_enter_game_->add_hover_image(text_enter_game, text_x_offset_, text_y_offset_);
		button_enter_game_->add_press_image(new Image("res/button-press-enter-game.png"), 0, 0);
		button_enter_game_->add_press_image(text_enter_game, text_x_offset_, text_y_offset_);
	}

	if (!button_game_settings_)
	{
		button_game_settings_ = new ImageButton();
		Image* text_game_settings = new Image("res/text-game-settings.png");
		button_game_settings_->add_normal_image(new Image("res/button-game-settings.png"), 0, 0);
		button_game_settings_->add_normal_image(text_game_settings, text_x_offset_, text_y_offset_);
		button_game_settings_->add_hover_image(new Image("res/button-hover-game-settings.png"), 0, 0);
		button_game_settings_->add_hover_image(text_game_settings, text_x_offset_, text_y_offset_);
		button_game_settings_->add_press_image(new Image("res/button-press-game-settings.png"), 0, 0);
		button_game_settings_->add_press_image(text_game_settings, text_x_offset_, text_y_offset_);
	}

	if (!button_exit_game_)
	{
		button_exit_game_ = new ImageButton();
		Image* text_exit_game = new Image("res/text-exit-game.png");
		button_exit_game_->add_normal_image(new Image("res/button-exit-game.png"), 0, 0);
		button_exit_game_->add_normal_image(text_exit_game, text_x_offset_, text_y_offset_);
		button_exit_game_->add_hover_image(new Image("res/button-hover-exit-game.png"), 0, 0);
		button_exit_game_->add_hover_image(text_exit_game, text_x_offset_, text_y_offset_);
		button_exit_game_->add_press_image(new Image("res/button-press-exit-game.png"), 0, 0);
		button_exit_game_->add_press_image(text_exit_game, text_x_offset_, text_y_offset_);
	}
	
	if (!button_special_thanks_)
	{
		button_special_thanks_ = new ImageButton();
		button_special_thanks_->add_normal_image(new Image("res/button-special-thanks.png"), 0, 0);
	}
}

void WelcomeInterface::opening_animation()												//显示开场动画
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
	

void WelcomeInterface::show_welcome()						
{
	game_welcome_bg_->show_image(0, 0);

	setrendermode(RENDER_MANUAL);

	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10) 
	{
		game_welcome_bg_->show_image(0, 0);
		button_enter_game_->set_position(x, 350);
		button_enter_game_->show();
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10) 
	{
		game_welcome_bg_->show_image(0, 0);
		button_enter_game_->show();
		button_game_settings_->set_position(x, 450);
		button_game_settings_->show();
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10) 
	{
		game_welcome_bg_->show_image(0, 0);
		button_enter_game_->show();
		button_game_settings_->show();
		button_exit_game_->set_position(x, 550);
		button_exit_game_->show();
	}
	
	button_special_thanks_->set_position(1000, 10);
	button_special_thanks_->show();
}

void WelcomeInterface::curtain()
{
	setbkmode(TRANSPARENT);
	static Image game_thanks_bg("res/game-special-thanks-bg.png");
	game_thanks_bg.show_image(0, 0);
	ege_enable_aa(true);
	AddFontResource("res/font-llt.ttc");									//加载字体文件
	setfont(-40, 0, "萝莉体 第二版");
	static LOGFONTA current_font;
	getfont(&current_font);
	current_font.lfUnderline = 1;													//设置字体带下划线
	current_font.lfQuality = ANTIALIASED_QUALITY;					//开启字体抗锯齿
	setfont(&current_font);

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

	RemoveFontResource("res/font-llt.ttc");
}

WelcomeInterface::action_type WelcomeInterface::action_judge(int x, int y)
{
	const int offset = 10;
	if (x > 80 - offset && x < 320 + offset && y > 350 - offset && y < 410 + offset)
		return ACTION_ENTER_GAME;
	else if (x > 80 - offset && x < 320 + offset && y >450 - offset && y < 510 + offset)
		return ACTION_GAME_SETTINGS;
	else if (x  > 80 - offset && x < 320 + offset && y>550 - offset &&  y < 610 + offset)
		return ACTION_EXIT_GAME;
	else if (x > 1000 - offset && x < 1200 + offset && y > 30 - offset && y < 230 + offset)
		return ACTION_SPECIAL_THANKS;
	else
		return ACTION_NONE;
}

void WelcomeInterface::on_mouse_move(WelcomeInterface::action_type action)
{
	switch (action)
	{
		case WelcomeInterface::ACTION_NONE:
			if (button_enter_game_->get_hover_status())
				button_enter_game_->show();
			if (button_game_settings_->get_hover_status())
				button_game_settings_->show();
			if (button_exit_game_->get_hover_status())
				button_exit_game_->show();
			break;
		case WelcomeInterface::ACTION_ENTER_GAME:
			button_enter_game_->on_mouse_hover();
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			button_game_settings_->on_mouse_hover();
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			button_exit_game_->on_mouse_hover();
			break;
		default:
			break;
	}
}

void WelcomeInterface::on_mouse_click(WelcomeInterface::action_type action)
{
	switch (action)
	{
		case WelcomeInterface::ACTION_ENTER_GAME:
			button_enter_game_->on_mouse_click();
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			button_game_settings_->on_mouse_click();
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			button_exit_game_->on_mouse_click();
			break;
		default:
			break;
	}
}
