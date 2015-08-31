#include "Dialog.h"
#include "../Gobang.h"

bool Dialog::register_flag_ = false;
WNDCLASSEX Dialog::window_class_ = { 0 };

Dialog::Dialog(int width, int height)
{
	if (register_flag_ == false)
	{
		window_class_.cbSize = sizeof(window_class_);
		window_class_.style = CS_VREDRAW | CS_HREDRAW;
		window_class_.lpfnWndProc = Dialog::WndProc;
		window_class_.cbClsExtra = 0;
		window_class_.cbWndExtra = 0;
		window_class_.hInstance = NULL;
		window_class_.hIcon = NULL;
		window_class_.hCursor = NULL;
		window_class_.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		window_class_.lpszMenuName = NULL;
		window_class_.lpszClassName = "Dream-Gobang-Dialog";
		//注册窗口
		RegisterClassEx(&window_class_);
		register_flag_ = true;
	}
	width_ = width;
	height_ = height;
	if (width_ < 200)
		width_ = 200;
	if (height_ < 100)
		height_ = 100;
	dialog_image_ = NULL;
	title_ = "Dialog";

	font_family_ = Gobang::font_default;
	font_size_ = 15;
}

LRESULT CALLBACK Dialog::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Dialog* dialog_data = (Dialog*)GetWindowLong(hWnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			if (dialog_data)
			{
				if (dialog_data->on_dialog_close())
					return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_SETCURSOR:
			if (getcursorstyle() != NULL)
			{
				SetCursor(getcursorstyle());
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_PAINT:
		{
			if (dialog_data && dialog_data->init_flag_ == false)
			{
				dialog_data->on_create_message();
				dialog_data->init_flag_ = true;
			}
			if (dialog_data && dialog_data->dialog_image_)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				BitBlt(hdc, 0, 0, dialog_data->width_, dialog_data->height_, dialog_data->dialog_dc_, 0, 0, SRCCOPY);
				EndPaint(hWnd, &ps);
			}
			return 0;
		}
		case WM_DESTROY:				//窗口销毁的消息
			PostQuitMessage(0);		//发送退出消息
			return 0;
		case WM_KEYDOWN:
			if (dialog_data)
			{
				if (dialog_data->on_key_press(wParam))
					return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void Dialog::show()
{
	init_flag_ = false;
	
	static DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING;
	static DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	//计算窗口实际大小
	RECT window_rect;
	window_rect.left = window_rect.top = 0;
	window_rect.right = width_;
	window_rect.bottom = height_;
	AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle);

	//创建窗口
	dialog_handle_ = CreateWindowEx(dwExStyle, window_class_.lpszClassName, title_.c_str(), dwStyle,
		(GetSystemMetrics(SM_CXSCREEN) - (window_rect.right - window_rect.left)) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - (window_rect.bottom - window_rect.top)) / 2,
		window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, getHWnd(), NULL, NULL, NULL);
	
	if (dialog_image_)
		delimage(dialog_image_);
	dialog_image_ = newimage(dialog_handle_, width_, height_);
	dialog_dc_ = getimage_dc(dialog_image_);
	setbkcolor(WHITE, dialog_image_);
	setbkmode(TRANSPARENT, dialog_image_);
	cleardevice(dialog_image_);

	on_dialog_init();

	SetWindowLong(dialog_handle_, GWL_USERDATA, (LONG)this);
	SetForegroundWindow(dialog_handle_);
	SetFocus(dialog_handle_);
	ShowWindow(dialog_handle_, SW_SHOW);

	MSG msg = { 0 };
	exit_flag_ = false;
	while (GetMessage(&msg, NULL, 0, 0) && exit_flag_ == false)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Dialog::~Dialog()
{
	if (dialog_image_)
		delimage(dialog_image_);
}

void Dialog::set_font(LPCSTR font_family, int size)
{
	font_family_ = font_family;
	font_size_ = size;
}
