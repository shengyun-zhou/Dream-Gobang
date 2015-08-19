#include "GradientAnimation.h"
#include "graphics.h"
#include "../Gobang.h"

int GCD(int a, int b)
{
	if (a % b == 0)
		return b;
	else
		return GCD(b, a % b);
}

void GradientAnimation::transition_ease_in()
{
	setrendermode(RENDER_MANUAL);
	PIMAGE screen_image = newimage(Gobang::WINDOW_WIDTH, Gobang::WINDOW_HEIGHT);
	putimage(screen_image, 0, 0, NULL);
	int gcd = GCD(Gobang::WINDOW_WIDTH, Gobang::WINDOW_HEIGHT);
	PIMAGE black_bg = newimage(Gobang::WINDOW_WIDTH / gcd, Gobang::WINDOW_HEIGHT / gcd);
	setbkcolor(BLACK, black_bg);
	cleardevice(black_bg);
	int alpha;
	for (alpha = 0; alpha <= 255; alpha += 10, delay_fps(60))
	{
		putimage(0, 0, screen_image);
		putimage_rotatezoom(NULL, black_bg, 0, 0, 0, 0, 0, (float)gcd, 0, alpha);
	}
	delimage(screen_image);
	delimage(black_bg);
	setrendermode(RENDER_AUTO);
}

void GradientAnimation::transition_ease_out(const Image* window_bg_img)
{
	setrendermode(RENDER_MANUAL);
	int gcd = GCD(Gobang::WINDOW_WIDTH, Gobang::WINDOW_HEIGHT);
	PIMAGE black_bg = newimage(Gobang::WINDOW_WIDTH / gcd, Gobang::WINDOW_HEIGHT / gcd);
	setbkcolor(BLACK, black_bg);
	cleardevice(black_bg);
	int alpha;
	for (alpha = 255; alpha >= 0; alpha -= 10, delay_fps(60))
	{
		window_bg_img->show_image(0, 0);
		putimage_rotatezoom(NULL, black_bg, 0, 0, 0, 0, 0, (float)gcd, 0, alpha);
	}
	window_bg_img->show_image(0, 0);
	delimage(black_bg);
	setrendermode(RENDER_AUTO);
}