#pragma once
#include "BaseButton.h"
#include "graphics.h"
#include <windows.h>
#include <string>
class Button : public BaseButton
{
protected:
	int width_;
	int height_;
	std::string text_;

	HWND button_handle_;
	HDC button_dc_;
	PIMAGE button_image_;

	std::string font_family_;
	int font_size_;
	PIMAGE icon_image_;
	RECT text_rect_;
	RECT button_rect_;
	bool is_focus_;
	BaseButton::OnClickListener* on_click_listener_;								//按钮被点击后运行的函数

	static bool register_flag_;
	static WNDCLASSEX button_class_;

	static const int padding_ = 5;
	static const int text_margin_ = 3;

	static LRESULT CALLBACK button_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void show_button_icon_text(color_t text_color);
	void on_focus();
public:
	Button(int width, int height);

	void set_icon(PIMAGE icon)
	{
		icon_image_ = icon;
	}

	virtual void set_font(LPCSTR font_family, int size)
	{
		font_family_ = font_family;
		font_size_ = size;
	}

	void set_text(const char* text)
	{
		text_ = text;
	}

	void set_on_click_listener(BaseButton::OnClickListener* listener)
	{
		on_click_listener_ = listener;
	}

	virtual ~Button();
	virtual void show(HWND parent_window);

	virtual void show();										//请勿手动调用
	virtual void on_mouse_hover();					//请勿手动调用
	virtual void on_mouse_click();					//请勿手动调用
};

