#include "EditText.h"
#include "../Gobang.h"
#include <string.h>

LRESULT(WINAPI *EditText::origin_proc)(HWND, UINT, WPARAM, LPARAM) = NULL;
WNDCLASSEX EditText::edit_frame_class_ = { 0 };
bool EditText::register_flag_ = false;

EditText::EditText(int width)
{
	if (!register_flag_)
	{
		edit_frame_class_.cbSize = sizeof(edit_frame_class_);
		edit_frame_class_.style = CS_VREDRAW | CS_HREDRAW;
		edit_frame_class_.lpfnWndProc = EditText::edit_frame_proc;
		edit_frame_class_.cbClsExtra = 0;
		edit_frame_class_.cbWndExtra = 0;
		edit_frame_class_.hInstance = NULL;
		edit_frame_class_.hIcon = NULL;
		edit_frame_class_.hCursor = NULL;
		edit_frame_class_.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		edit_frame_class_.lpszMenuName = NULL;
		edit_frame_class_.lpszClassName = "Dream-Gobang-Win32EditFrame";
		RegisterClassEx(&edit_frame_class_);
		register_flag_ = true;
	}

	width_ = width;
	pos_x_ = pos_y_ = 10;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &font_, NULL);
	font_.lfHeight = -15;

	LOGFONT temp_font;
	getfont(&temp_font);
	setfont(&font_);
	int text_height = textheight('A');
	setfont(&temp_font);
	height_ = text_height + 2 * padding_top_;

	edit_handle_ = NULL;
	edit_frame_image_ = NULL;
	max_len_ = 1024;
	listener_ = NULL;
	focus_flag_ = false;
}

EditText::~EditText()
{
	if (edit_frame_image_)
		delimage(edit_frame_image_);
}

void EditText::set_font(int size, LPCSTR font_family)
{
#ifdef _MSC_VER
	strcpy_s(font_.lfFaceName, sizeof(font_.lfFaceName), font_family);
#else
	strcpy(font_.lfFaceName, font_family);
#endif
	font_.lfHeight = size * -1;
	LOGFONT temp_font;
	getfont(&temp_font);
	setfont(&font_);
	int text_height = textheight('A');
	setfont(&temp_font);
	height_ = text_height + 2 * padding_top_;
}

void EditText::set_max_len(int len)
{
	max_len_ = len;
	if (max_len_ <= 0)
		max_len_ = 1;
	else if (max_len_ > 1024)
		max_len_ = 1024;
}

void EditText::set_focus()
{
	focus_flag_ = true;
}

LRESULT CALLBACK EditText::edit_frame_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	EditText* edit_data = (EditText*)GetWindowLong(hWnd, GWL_USERDATA);
	switch (message)
	{
		case WM_SETCURSOR:
			if (getcursorstyle() != NULL)
			{
				SetCursor(getcursorstyle());
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_PAINT:
			if (edit_data && edit_data->init_flag_ == false)
			{
				edit_data->edit_handle_ = CreateWindow("EDIT", "", WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
					EditText::padding_left_, EditText::padding_top_, 
					edit_data->width_ - 2 * EditText::padding_left_, edit_data->height_ - 2 * EditText::padding_top_,
					hWnd, NULL, NULL, NULL);

				if (!EditText::origin_proc)
					EditText::origin_proc = (LRESULT(WINAPI *)(HWND, UINT, WPARAM, LPARAM))GetWindowLong(edit_data->edit_handle_, GWL_WNDPROC);

				SetWindowLong(edit_data->edit_handle_, GWL_USERDATA, (LONG)edit_data);
				SetWindowLong(edit_data->edit_handle_, GWL_WNDPROC, (LONG)EditText::edit_proc);

				HFONT hfont = CreateFontIndirect(&edit_data->font_);
				if (hfont)
					SendMessage(edit_data->edit_handle_, WM_SETFONT, (WPARAM)hfont, NULL);
				SendMessage(edit_data->edit_handle_, EM_LIMITTEXT, (WPARAM)edit_data->max_len_, NULL);
				ShowWindow(edit_data->edit_handle_, SW_SHOW);
				edit_data->init_flag_ = true;
			}
			if (edit_data && edit_data->edit_frame_image_)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				BitBlt(hdc, 0, 0, edit_data->width_, edit_data->height_, edit_data->edit_frame_dc_, 0, 0, SRCCOPY);
				EndPaint(hWnd, &ps);
			}
			if (edit_data->focus_flag_)
			{
				SetFocus(edit_data->edit_handle_);
				edit_data->focus_flag_ = false;
			}
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK EditText::edit_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	EditText* edit_data = (EditText*)GetWindowLong(hWnd, GWL_USERDATA);
	switch (message)
	{
		case WM_SETCURSOR:
			if (getcursorstyle() != NULL)
			{
				SetCursor(getcursorstyle());
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_ERASEBKGND:
			return 0;
		case WM_SETFOCUS:
			if (edit_data && edit_data->edit_frame_image_)
			{
				edit_data->on_focus();
				RedrawWindow(edit_data->edit_frame_handle_, &edit_data->edit_frame_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return EditText::origin_proc(hWnd, message, wParam, lParam);
		case WM_KILLFOCUS:
			if (edit_data && edit_data->edit_frame_image_)
			{
				edit_data->on_normal();
				RedrawWindow(edit_data->edit_frame_handle_, &edit_data->edit_frame_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return EditText::origin_proc(hWnd, message, wParam, lParam);
		case WM_KEYDOWN:
			if (wParam == VK_RETURN)
			{
				if (edit_data && edit_data->listener_)
				{
					edit_data->listener_->on_activate();
					return 0;
				}
			}
			return EditText::origin_proc(hWnd, message, wParam, lParam);
		default:
			if (EditText::origin_proc)
				return EditText::origin_proc(hWnd, message, wParam, lParam);
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void EditText::show(HWND parent_window)
{
	init_flag_ = false;
	edit_frame_handle_ = CreateWindow(edit_frame_class_.lpszClassName, "", WS_CHILD,
		pos_x_, pos_y_, width_, height_, parent_window, NULL, NULL, NULL);

	edit_frame_rect_.left = edit_frame_rect_.top = 0;
	edit_frame_rect_.bottom = height_ - 1;
	edit_frame_rect_.right = width_ - 1;

	SetWindowLong(edit_frame_handle_, GWL_USERDATA, (LONG)this);

	if (edit_frame_image_)
		delimage(edit_frame_image_);
	edit_frame_image_ = newimage(edit_frame_handle_, width_, height_);
	edit_frame_dc_ = getimage_dc(edit_frame_image_);
	on_normal();

	ShowWindow(edit_frame_handle_, SW_SHOW);
}

void EditText::set_text(const char* text)
{
	SendMessage(edit_handle_, WM_SETTEXT, (WPARAM)0, (LPARAM)text);
}

void EditText::get_text(std::string& str)
{
	char buf[1024];
	SendMessage(edit_handle_, WM_GETTEXT, (WPARAM)(max_len_ + 1), (LPARAM)buf);
	str = buf;
}

void EditText::on_normal()
{
	setbkcolor(EGERGB(255, 255, 255), edit_frame_image_);
	cleardevice(edit_frame_image_);
	setcolor(LIGHTGRAY, edit_frame_image_);
	setlinestyle(SOLID_LINE, NULL, 2, edit_frame_image_);
	rectangle(1, 1, width_, height_, edit_frame_image_);
}

void EditText::on_focus()
{
	setbkcolor(EGERGB(255, 255, 255), edit_frame_image_);
	cleardevice(edit_frame_image_);
	setcolor(EGERGB(69, 90, 100), edit_frame_image_);
	setlinestyle(SOLID_LINE, NULL, 3, edit_frame_image_);
	rectangle(1, 1, width_ - 1, height_ - 1, edit_frame_image_);
}
