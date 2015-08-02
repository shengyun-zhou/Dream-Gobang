#pragma once
#include "Dialog.h"
#include "Button.h"
class MessageDialog : public Dialog
{
protected:
	virtual void on_dialog_init();
	virtual void on_create_message();

	class OkButtonListener : public BaseButton::OnClickListener
	{
	private:
		HWND window_handle_;
	public:
		OkButtonListener(HWND hwnd)
		{
			window_handle_ = hwnd;
		}

		void on_click();
	};

	OkButtonListener ok_listener_;

	RECT text_rect_;
	std::string text_;
	static Button* ok_button_;

	static PIMAGE icon_infomation_;

	static const int icon_margin_ = 35;
	static const int text_margin_ = 15;
	static const int button_width_ = 110;
	static const int button_height_ = 35;
	static const int button_margin_ = 8;
	static const int button_area_height_ = button_height_ + button_margin_ * 2;
public:
	MessageDialog(int width, int height);
	virtual ~MessageDialog();

	void set_text(const char* text)
	{
		text_ = text;
	}
};

