#pragma once
#include <windows.h>
#include <string>
#include "graphics.h"
class EditText
{
protected:
	int pos_x_;
	int pos_y_;
	int width_;
	int height_;
	static WNDCLASSEX edit_frame_class_;
	static bool register_flag_;
	HWND edit_frame_handle_;
	HWND edit_handle_;
	std::string text_;
	std::string input_text_;
	int max_len_;
	bool init_flag_;
	bool focus_flag_;

	LOGFONT font_;
	PIMAGE edit_frame_image_;
	HDC edit_frame_dc_;
	RECT edit_frame_rect_;

	static const int padding_top_ = 7;
	static const int padding_left_ = 10;

	//由于origin_proc函数指针指向的函数存在于Windows系统的DLL中，因此必须加上WINAPI(也就是__stdcall)
	//标识来告诉编译器该函数的调用方式
	static LRESULT(WINAPI *origin_proc)(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK edit_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK edit_frame_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void on_normal();
	virtual void on_focus();
public:
	EditText(int width);
	~EditText();

	void set_position(int x, int y)
	{
		pos_x_ = x;
		pos_y_ = y;
	}
	
	int get_height()
	{
		return height_;
	}

	virtual void show(HWND parent_window);
	void set_max_len(int len);

	void set_text(const char* text);
	void get_text(std::string& text);
	void set_font(int size, LPCSTR font_family);
	void set_focus();

	class OnActivateListener
	{
	public:
		virtual void on_activate() = 0;
	};

	void set_on_activate_listener(OnActivateListener* listener)
	{
		listener_ = listener;
	}
protected:
	OnActivateListener* listener_;
};

