#pragma once
#include "ImageButton.h"
#include "../Gobang.h"
#include <string>

using namespace std;

class ImageTextButton : public ImageButton
{
protected:
	struct TextInfo
	{
		int padding_left;
		int padding_top;
		string text;
		string font_family;
		int font_size;
		color_t text_color;
	};
	TextInfo normal_text_;
	TextInfo hover_text_;
	TextInfo press_text_;
private:
	void show_text(TextInfo& text_info);
	void set_text_info(TextInfo& info, const char* text, int font_size, int padding_left, int padding_right, const char* font_family, color_t text_color);
public:
	ImageTextButton();
	virtual ~ImageTextButton();

	virtual void set_normal_text(const char* text, int font_size, int padding_left = -1, int padding_top = -1,
															 const char* font_family = Gobang::font_default, color_t text_color = WHITE);
	virtual void set_hover_text(const char* text, int font_size, int padding_left = -1, int padding_top = -1,
		const char* font_family = Gobang::font_default, color_t text_color = WHITE);
	virtual void set_press_text(const char* text, int font_size, int padding_left = -1, int padding_top = -1,
		const char* font_family = Gobang::font_default, color_t text_color = WHITE);

	virtual void show();
	virtual void on_mouse_hover();
	virtual void on_mouse_click();
};

