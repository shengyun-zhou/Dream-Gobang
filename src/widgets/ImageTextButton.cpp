#include "ImageTextButton.h"

ImageTextButton::ImageTextButton() : ImageButton()
{
	normal_text_.font_size = hover_text_.font_size = press_text_.font_size = -1;
}

ImageTextButton::~ImageTextButton()
{
}

void ImageTextButton::show_text(TextInfo& text_info)
{
	if (text_info.font_size <= 0 || text_info.text.length() <= 0)
		return;
	int target_x, target_y;
	Gobang::set_font(text_info.font_family.c_str(), text_info.font_size);
	setcolor(text_info.text_color);
	setbkmode(TRANSPARENT);
	if (text_info.padding_left < 0)
	{
		int text_width = textwidth(text_info.text.c_str());
		target_x = pos_x_ + get_width() / 2 - text_width / 2;
	}
	else
		target_x = pos_x_ + text_info.padding_left;
	if (text_info.padding_top < 0)
	{
		int text_height = textheight(text_info.text.c_str());
		target_y = pos_y_ + get_height() / 2 - text_height / 2;
	}
	else
		target_y = pos_y_ + text_info.padding_top;
	outtextxy(target_x, target_y, text_info.text.c_str());
}

void ImageTextButton::set_text_info(TextInfo& info, const char* text, int font_size, int padding_left, int padding_top, const char* font_family, color_t text_color)
{
	info.text = text;
	info.font_size = font_size;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	info.font_family = font_family;
	info.text_color = text_color;
}

void ImageTextButton::set_normal_text(const char* text, int font_size, int padding_left, int padding_top, const char* font_family, color_t text_color)
{
	set_text_info(normal_text_, text, font_size, padding_left, padding_top, font_family, text_color);
}

void ImageTextButton::set_hover_text(const char* text, int font_size, int padding_left, int padding_top, const char* font_family, color_t text_color)
{
	set_text_info(hover_text_, text, font_size, padding_left, padding_top, font_family, text_color);
}

void ImageTextButton::set_press_text(const char* text, int font_size, int padding_left, int padding_top, const char* font_family, color_t text_color)
{
	set_text_info(press_text_, text, font_size, padding_left, padding_top, font_family, text_color);
}

void ImageTextButton::show()
{
	ImageButton::show();
	show_text(normal_text_);
}

void ImageTextButton::on_mouse_hover()
{
	if (!get_hover_status())
	{
		ImageButton::on_mouse_hover();
		show_text(hover_text_);
	}
}

void ImageTextButton::on_mouse_click()
{
	if (get_hover_status())
	{
		ImageButton::on_mouse_click();
		show_text(press_text_);
	}
}