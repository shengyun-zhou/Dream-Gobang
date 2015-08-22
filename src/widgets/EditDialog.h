#pragma once
#include "Dialog.h"
#include "Button.h"
#include "EditText.h"
class EditDialog : public Dialog
{
private:
	static PIMAGE dialog_icon_;
	static Button* ok_button_;
	static Button* cancel_button_;
	EditText* edit_text_;
	
	static const int icon_margin_ = 35;
	static const int padding_ = 15;
	static const int edit_margin_bottom_ = 10;
	static const int text_margin_ = 10;
	std::string text_;
	RECT text_rect_;

	virtual void on_dialog_init();
	virtual void on_dialog_close();
	virtual void on_create_message();
	std::string input_text_;
public:
	enum ResponseType
	{
		response_none,
		response_ok,
		response_cancel,
		response_close,
	};

	EditDialog(int width, int height);
	virtual ~EditDialog();

	void set_text(const char* text)
	{
		text_ = text;
	}

	void set_input_max_len(int length)
	{
		edit_text_->set_max_len(length);
	}

	void set_input_text(const char* text)
	{
		input_text_ = text;
	}

	std::string get_input_text()
	{
		return input_text_;
	}

	ResponseType get_response_type()
	{
		return response_type_;
	}

private:
	ResponseType response_type_;

	class ButtonListener : public Button::OnClickListener
	{
	private:
		EditDialog* dialog_;
		ResponseType type_;
	public:
		ButtonListener(EditDialog* dialog, EditDialog::ResponseType type)
		{
			dialog_ = dialog;
			type_ = type;
		}
		void on_click();
	};

	class EditListener :public EditText::OnActivateListener
	{
	private:
		EditDialog* dialog_;
	public:
		EditListener(EditDialog* dialog)
		{
			dialog_ = dialog;
		}
		void on_activate();
	};

	ButtonListener ok_button_listener_;
	ButtonListener cancel_button_listener_;
	EditListener edit_listener_;
};

