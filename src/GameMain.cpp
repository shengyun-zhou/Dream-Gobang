#define SHOW_CONSOLE
#include <graphics.h>
#include "Gobang.h"
#include "Image.h"
#include "Music.h"

using namespace Gobang;

void show_animation()																	//显示开场动画
{
	Image game_logo("game-logo.png");
	const int image_width = 500;
	const int image_height = 222;
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
	for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i > 255)
			i = 255;
		setbkcolor_f(EGERGB(color, color, color));												//由黑到白逐渐改变背景色
		cleardevice();
		/*由完全透明到不透明渲染Logo图片*/
		game_logo.show_image_with_alpha(WINDOW_WIDTH / 2 - image_width / 2, WINDOW_HEIGHT / 2 - image_height / 2, (double)i / 255.0);
	}

	delay_ms(1500);
	deltaAlpha *= -1;
	deltaColor *= -1;
	/*下面的动画为上面动画的逆过程*/
	for (color = 255, i = 255 + (-1)*deltaAlpha; color >= 0; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i<0)
			i = 0;
		setbkcolor_f(EGERGB(color, color, color));
		cleardevice();
		game_logo.show_image_with_alpha(WINDOW_WIDTH / 2 - image_width / 2, WINDOW_HEIGHT / 2 - image_height / 2, (double)i / 255.0);
	}
	delay_ms(100);
}

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	show_animation();
	Music bg_music("TheNewWorld.mp3", 2);
	bg_music.set_volume(0.5);
	bg_music.start();

	//Music bg_music2("TheNewWorld.wav", -1);
	//bg_music2.set_volume(0.8);
	//delay_ms(5000);
	//bg_music2.start();

	/*delay_ms(3000);
	printf("pause..\n");
	bg_music.pause();
	delay_ms(3000);
	printf("resume..\n");
	bg_music.resume();*/
	getch();
	return 0;
}
