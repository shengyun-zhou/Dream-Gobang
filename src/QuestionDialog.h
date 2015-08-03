#pragma once
#include "Dialog.h"
#include "Button.h"
class QuestionDialog : public Dialog
{
private:
	PIMAGE dialog_icon_;
	static PIMAGE icon_question_;
	static PIMAGE icon_warning_;
	static Button* yes_button_;
	static Button* no_button_;

	static const int icon_margin_ = 35;
	static const int text_margin_ = 15;
	static const int button_width_ = 110;
	static const int button_height_ = 35;
	static const int button_margin_ = 8;
	static const int button_area_height_ = button_height_ + button_margin_ * 2;

	RECT text_rect_;
	std::string text_;
	virtual void on_dialog_init();
	virtual void on_create_message();
	virtual void on_dialog_close();
public:
	enum IconType
	{
		icon_question,
		icon_warning
	};

	enum ResponseType
	{
		response_none,
		response_yes,
		response_no,
		response_close
	};

	QuestionDialog(int width, int height, QuestionDialog::IconType type = icon_question);
	virtual ~QuestionDialog();

	QuestionDialog::ResponseType get_response_result()
	{
		return response_result_;
	}

	void set_text(const char* text)
	{
		text_ = text;
	}
private:
	QuestionDialog::ResponseType response_result_;

	class ButtonListener : public Button::OnClickListener
	{
	private:
		HWND parent_window_;
		QuestionDialog::ResponseType type_;
		QuestionDialog::ResponseType* response_value_;
	public:
		ButtonListener(HWND hWnd, ResponseType* response_pointer, ResponseType type)
		{
			parent_window_ = hWnd;
			response_value_ = response_pointer;
			type_ = type;
		}

		void on_click();
	};

	ButtonListener yes_button_listener_;
	ButtonListener no_button_listener_;
};

