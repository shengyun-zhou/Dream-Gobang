#pragma once
#include <windows.h>
#include "graphics.h"
#include <string>

class Dialog
{
protected:
	int width_;
	int height_;
	std::string title_;
	HWND dialog_handle_;
	HDC dialog_dc_;
	PIMAGE dialog_image_;
	bool exit_flag_;
	bool init_flag_;

	std::string font_family_;
	int font_size_;

	static bool register_flag_;
	static WNDCLASSEX window_class_;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void on_dialog_init(){}
	virtual void on_create_message(){}
	virtual void on_dialog_close();
public:
	Dialog(int width, int height);
	virtual ~Dialog();
	void show();

	void set_font(LPCSTR font_family, int size);

	void set_title(const char* title)
	{
		title_ = title;
	}
};

