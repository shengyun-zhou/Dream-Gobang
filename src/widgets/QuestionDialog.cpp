#include "QuestionDialog.h"
#include "../Gobang.h"
#include "image_dialog_button_cancel.h"
#include "image_dialog_button_ok.h"
#include "image_dialog_icon_question.h"
#include "image_dialog_icon_warning.h"

PIMAGE QuestionDialog::icon_question_ = NULL;
PIMAGE QuestionDialog::icon_warning_ = NULL;
Button* QuestionDialog::yes_button_ = NULL;
Button* QuestionDialog::no_button_ = NULL;

QuestionDialog::QuestionDialog(int width, int height, QuestionDialog::IconType type) : Dialog(width, height),
yes_button_listener_(NULL, NULL, response_none), no_button_listener_(NULL, NULL, response_none)
{
	if (!icon_question_)
	{
		icon_question_ = newimage();
		getimage_mem(icon_question_, binary_dialog_icon_question_png, sizeof(binary_dialog_icon_question_png));
	}
	if (!icon_warning_)
	{
		icon_warning_ = newimage();
		getimage_mem(icon_warning_, binary_dialog_icon_warning_png, sizeof(binary_dialog_icon_warning_png));
	}
	switch (type)
	{
		case QuestionDialog::icon_question:
			dialog_icon_ = icon_question_;
			break;
		case QuestionDialog::icon_warning:
			dialog_icon_ = icon_warning_;
			break;
	}

	if (!yes_button_)
	{
		yes_button_ = new Button(button_width_, button_height_);
		yes_button_->set_text("确定");
		static PIMAGE icon_ok = newimage();
		getimage_mem(icon_ok, binary_dialog_button_ok_png, sizeof(binary_dialog_button_ok_png));
		yes_button_->set_icon(icon_ok);
	}
	if (!no_button_)
	{
		no_button_ = new Button(button_width_, button_height_);
		no_button_->set_text("取消");
		static PIMAGE icon_cancel = newimage();
		getimage_mem(icon_cancel, binary_dialog_button_cancel_png, sizeof(binary_dialog_button_cancel_png));
		no_button_->set_icon(icon_cancel);
	}

	no_button_->set_position(width_ - button_margin_ - button_width_, height_ - button_margin_ - button_height_);
	yes_button_->set_position(width_ - button_margin_ * 2 - button_width_ * 2, height_ - button_margin_ - button_height_);

	text_rect_.left = icon_margin_ + getwidth(dialog_icon_) + text_margin_;
	text_rect_.top = text_margin_;
	text_rect_.right = width - text_margin_;
	text_rect_.bottom = height_ - button_area_height_ - text_margin_;
	text_ = "A question message.";
}


QuestionDialog::~QuestionDialog()
{
}

void QuestionDialog::on_dialog_init()
{
	Gobang::set_font(font_family_.c_str(), font_size_, false, false, dialog_image_);
	putimage_withalpha(dialog_image_, dialog_icon_,
		icon_margin_,
		(height_ - button_area_height_ - getheight(dialog_icon_)) / 2);
	setcolor(BLACK, dialog_image_);
	static UINT text_format = DT_LEFT | DT_TOP | DT_EDITCONTROL | DT_WORDBREAK | DT_WORD_ELLIPSIS;
	//rectangle(text_rect_.left, text_rect_.top, text_rect_.right, text_rect_.bottom, dialog_image_);
	//计算文本高度
	int text_height = DrawText(dialog_dc_, text_.c_str(), -1, &text_rect_, text_format | DT_CALCRECT);
	text_rect_.top = (height_ - button_area_height_ - text_height) / 2;
	text_rect_.bottom = height_ - button_area_height_ - text_margin_;
	DrawText(dialog_dc_, text_.c_str(), -1, &text_rect_, text_format);

	setfillcolor(EGERGB(230, 230, 230), dialog_image_);
	bar(0, height_ - button_area_height_, width_, height_, dialog_image_);

	response_result_ = response_none;
	yes_button_listener_ = ButtonListener(dialog_handle_, &response_result_, response_yes);
	yes_button_->set_on_click_listener(&yes_button_listener_);
	no_button_listener_ = ButtonListener(dialog_handle_, &response_result_, response_no);
	no_button_->set_on_click_listener(&no_button_listener_);
}

void QuestionDialog::on_create_message()
{
	yes_button_->show(dialog_handle_);
	no_button_->show(dialog_handle_);
	no_button_->set_focus();
}

bool QuestionDialog::on_dialog_close()
{
	response_result_ = response_close;
	DestroyWindow(dialog_handle_);
	return true;
}

void QuestionDialog::ButtonListener::on_click()
{
	*response_value_ = type_;
	DestroyWindow(parent_window_);
}
