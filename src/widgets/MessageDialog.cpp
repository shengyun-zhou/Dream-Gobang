#include "MessageDialog.h"
#include "../Gobang.h"
#include "image_dialog_button_ok.h"
#include "image_dialog_icon_information.h"
#include "image_dialog_icon_error.h"

PIMAGE MessageDialog::icon_infomation_ = NULL;
PIMAGE MessageDialog::icon_error_ = NULL;
Button* MessageDialog::ok_button_ = NULL;

MessageDialog::MessageDialog(int width, int height, MessageDialog::IconType type) : Dialog(width, height), ok_listener_(NULL)
{
	if (!icon_infomation_)
	{
		icon_infomation_ = newimage();
		getimage_mem(icon_infomation_, binary_dialog_icon_information_png, sizeof(binary_dialog_icon_information_png));
	}
	if (!icon_error_)
	{
		icon_error_ = newimage();
		getimage_mem(icon_error_, binary_dialog_icon_error_png, sizeof(binary_dialog_icon_error_png));
	}
	if (!ok_button_)
	{
		ok_button_ = new Button(button_width_, button_height_);
		ok_button_->set_text("确定");
		static PIMAGE icon_ok = newimage();
		getimage_mem(icon_ok, binary_dialog_button_ok_png, sizeof(binary_dialog_button_ok_png));
		ok_button_->set_icon(icon_ok);
	}

	switch (type)
	{
		case icon_infomation:
			dialog_icon_ = icon_infomation_;
			set_title("提示");
			break;
		case icon_error:
			dialog_icon_ = icon_error_;
			set_title("错误");
			break;
	}

	ok_button_->set_position(width_ - button_margin_ - button_width_, height_ - button_margin_ - button_height_);
	text_ = "A message.";
}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::on_dialog_init()
{
	Gobang::set_font(font_family_.c_str(), font_size_, false, false, dialog_image_);
	putimage_withalpha(dialog_image_, dialog_icon_,
										 icon_margin_,
										 (height_ - button_area_height_ - getheight(dialog_icon_)) / 2);

	setcolor(BLACK, dialog_image_);
	static UINT text_format = DT_LEFT | DT_TOP | DT_EDITCONTROL | DT_WORDBREAK | DT_WORD_ELLIPSIS;
	text_rect_.left = icon_margin_ + getwidth(dialog_icon_) + text_margin_;
	text_rect_.top = text_margin_;
	text_rect_.right = width_ - text_margin_;
	text_rect_.bottom = height_ - button_area_height_ - text_margin_;
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
	ok_button_->set_focus();
}

bool MessageDialog::on_key_press(unsigned int key_value)
{
	if (key_value == VK_RETURN)
	{
		DestroyWindow(dialog_handle_);
		return true;
	}
	return false;
}

void MessageDialog::OkButtonListener::on_click()
{
	DestroyWindow(this->window_handle_);
}
