#include "Button.h"

bool Button::register_flag_ = false;
WNDCLASSEX Button::button_class_ = { 0 };

Button::Button(int width, int height) : BaseButton()
{
	if (register_flag_ == false)
	{
		button_class_.cbSize = sizeof(button_class_);
		button_class_.style = CS_VREDRAW | CS_HREDRAW;
		button_class_.lpfnWndProc = Button::button_proc;
		button_class_.cbClsExtra = 0;
		button_class_.cbWndExtra = 0;
		button_class_.hInstance = NULL;
		button_class_.hIcon = NULL;
		button_class_.hCursor = NULL;
		button_class_.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		button_class_.lpszMenuName = NULL;
		button_class_.lpszClassName = "Dream-Gobang-Win32Button";
		RegisterClassEx(&button_class_);
		register_flag_ = true;
	}
	width_ = width;
	height_ = height;
	icon_image_ = NULL;
	text_ = "Button";
	button_handle_ = NULL;
	button_dc_ = NULL;
	button_image_ = NULL;

	LOGFONT system_font;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &system_font, NULL);
	font_family_ = system_font.lfFaceName;
	font_size_ = -15;
	on_click_listener_ = NULL;
}

Button::~Button()
{
	if (button_image_)
		delimage(button_image_);
}

LRESULT CALLBACK Button::button_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Button* button_data = (Button*)GetWindowLong(hWnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
			return 0;
		case WM_SETCURSOR:
			if (getcursorstyle() != NULL)
				SetCursor(getcursorstyle());
			return 0;
		case WM_PAINT:
		{
			if (button_data && button_data->button_image_)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				BitBlt(hdc, 0, 0, button_data->width_, button_data->height_, button_data->button_dc_, 0, 0, SRCCOPY);
				EndPaint(hWnd, &ps);
			}
			return 0;
		}
		case WM_MOUSELEAVE:
			if (button_data && button_data->button_image_)
			{
				button_data->button_hover_flag_ = false;
				if (button_data->is_focus_)
					button_data->on_focus();
				else
					button_data->show();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return 0;
		case WM_MOUSEMOVE:
			static TRACKMOUSEEVENT mouse_event;
			mouse_event.cbSize = sizeof(mouse_event);
			mouse_event.dwFlags = TME_LEAVE;
			mouse_event.hwndTrack = hWnd;
			mouse_event.dwHoverTime = 0;
			TrackMouseEvent(&mouse_event);
			if (button_data && button_data->button_image_)
			{
				button_data->button_hover_flag_ = true;
				button_data->on_mouse_hover();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return 0;
		case WM_LBUTTONDOWN:
			if (button_data && button_data->button_image_)
			{
				SetFocus(hWnd);
				button_data->on_mouse_click();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW|RDW_INVALIDATE);
			}
			return 0;
		case WM_LBUTTONUP:
			if (button_data && button_data->button_image_)
			{
				button_data->on_mouse_hover();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			if (button_data && button_data->on_click_listener_)
				button_data->on_click_listener_->on_click();
			return 0;
		case WM_KEYDOWN:
			if (wParam == VK_RETURN)
			{
				if (button_data && button_data->button_image_)
				{
					button_data->on_mouse_click();
					RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
				}
				if (button_data && button_data->on_click_listener_)
					button_data->on_click_listener_->on_click();
			}
			return 0;
		case WM_SETFOCUS:
			button_data->is_focus_ = true;
			if (button_data && button_data->button_image_)
			{
				button_data->on_focus();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return 0;
		case WM_KILLFOCUS:
			button_data->is_focus_ = false;
			if (button_data && button_data->button_image_)
			{
				button_data->show();
				RedrawWindow(hWnd, &button_data->button_rect_, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			}
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void Button::show_button_icon_text(color_t text_color)
{
	UINT text_format = DT_SINGLELINE | DT_WORD_ELLIPSIS;
	text_rect_.top = padding_;
	text_rect_.bottom = height_ - padding_;
	text_rect_.right = width_ - padding_;
	if (icon_image_)
	{
		int text_width = textwidth(text_.c_str(), button_image_);
		int width_sum = text_width + text_margin_ + getwidth(icon_image_);
		putimage_withalpha(button_image_, icon_image_, (width_ - width_sum) / 2, (height_ - getheight(icon_image_)) / 2);
		text_rect_.left = (width_ - width_sum) / 2 + getwidth(icon_image_) + text_margin_;
	}
	else
	{
		text_rect_.left = padding_;
		text_format |= DT_CENTER | DT_TOP;
	}

	setcolor(text_color, button_image_);
	setfont(font_size_, 0, font_family_.c_str(), button_image_);
	setbkmode(TRANSPARENT, button_image_);
	int text_height = DrawText(button_dc_, text_.c_str(), -1, &text_rect_, text_format | DT_CALCRECT);
	text_rect_.top = (height_ - text_height) / 2;
	text_rect_.right = width_ - padding_;
	//setlinestyle(SOLID_LINE, NULL, 1, button_image_);
	//rectangle(text_rect_.left, text_rect_.top, text_rect_.right, text_rect_.bottom, button_image_);
	DrawText(button_dc_, text_.c_str(), -1, &text_rect_, text_format);
}

void Button::show(HWND parent_window)
{
	is_focus_ = false;
	button_handle_ = CreateWindow(button_class_.lpszClassName, "", WS_CHILD, pos_x_, pos_y_,
																width_, height_, parent_window, NULL, NULL, NULL);
	
	button_rect_.left = button_rect_.top = 0;
	button_rect_.bottom = height_ - 1;
	button_rect_.right = width_ - 1;
	SetWindowLong(button_handle_, GWL_USERDATA, (LONG)this);
	ShowWindow(button_handle_, SW_SHOW);

	if (button_image_)
		delimage(button_image_);
	button_image_ = newimage(button_handle_, width_, height_);
	button_dc_ = getimage_dc(button_image_);
	this->show();
}

void Button::show()
{
	setfillcolor(WHITE, button_image_);
	bar(0, 0, width_, height_, button_image_);
	setcolor(LIGHTGRAY, button_image_);
	setlinestyle(SOLID_LINE, NULL, 2, button_image_);
	rectangle(1, 1, width_, height_, button_image_);
	show_button_icon_text(BLACK);
}

void Button::on_mouse_hover()
{
	setfillcolor(EGERGB(144, 164, 174), button_image_);
	bar(0, 0, width_, height_, button_image_);
	setcolor(EGERGB(69, 90, 100), button_image_);
	setlinestyle(SOLID_LINE, NULL, 2, button_image_);
	rectangle(1, 1, width_ - 1, height_ - 1, button_image_);
	show_button_icon_text(BLACK);
}

void Button::on_mouse_click()
{
	setfillcolor(EGERGB(96, 125, 139), button_image_);
	bar(0, 0, width_, height_, button_image_);
	setcolor(EGERGB(69, 90, 100), button_image_);
	setlinestyle(SOLID_LINE, NULL, 2, button_image_);
	rectangle(1, 1, width_ - 1, height_ - 1, button_image_);
	show_button_icon_text(WHITE);
}

void Button::on_focus()
{
	setfillcolor(WHITE, button_image_);
	bar(0, 0, width_, height_, button_image_);
	setcolor(EGERGB(69, 90, 100), button_image_);
	setlinestyle(SOLID_LINE, NULL, 3, button_image_);
	rectangle(1, 1, width_ - 2, height_ - 2, button_image_);
	show_button_icon_text(BLACK);
}