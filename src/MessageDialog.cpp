#include "MessageDialog.h"

PIMAGE MessageDialog::icon_infomation_ = NULL;
Button* MessageDialog::ok_button_ = NULL;

MessageDialog::MessageDialog(int width, int height) : Dialog(width, height), ok_listener_(NULL)
{
	if (width_ < 200)
		width_ = 200;
	if (height_ < 100)
		height_ = 100;

	if (!icon_infomation_)
	{
		icon_infomation_ = newimage();
		getimage(icon_infomation_, "res/dialog/dialog-icon-information.png");
	}
	if (!ok_button_)
	{
		ok_button_ = new Button(button_width_, button_height_);
		ok_button_->set_text("确定");
		static PIMAGE icon_ok = newimage();
		getimage(icon_ok, "res/dialog/dialog-button-ok.png");
		ok_button_->set_icon(icon_ok);
	}

	ok_button_->set_position(width_ - button_margin_ - button_width_, height_ - button_margin_ - button_height_);

	text_rect_.left = icon_margin_ + getwidth(icon_infomation_) + text_margin_;
	text_rect_.top = text_margin_;
	text_rect_.right = width - text_margin_;
	text_rect_.bottom = height_ - button_area_height_ - text_margin_;
	text_ = "A message.";

}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::on_dialog_init()
{
	putimage_withalpha(dialog_image_, icon_infomation_,
										 icon_margin_,
										 (height_ - button_area_height_ - getheight(icon_infomation_)) / 2);

	setfont(font_size_, 0, font_family_.c_str(), dialog_image_);
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

	ok_listener_ = OkButtonListener(dialog_handle_);
	ok_button_->set_on_click_listener(&ok_listener_);

}

void MessageDialog::on_create_message()
{
	ok_button_->show(dialog_handle_);
}

void MessageDialog::OkButtonListener::on_click()
{
	DestroyWindow(this->window_handle_);
}
