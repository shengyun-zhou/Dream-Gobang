#pragma once
#include "Image.h"
class GradientAnimation
{
public:
	static void transition_ease_in();
	static void transition_ease_out(const Image* window_bg_img);
};

