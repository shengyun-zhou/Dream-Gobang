#include "EditDialog.h"
#include "../Gobang.h"
#include "image_dialog_button_cancel.h"
#include "image_dialog_button_ok.h"
#include "image_dialog_icon_edit.h"

PIMAGE EditDialog::dialog_icon_ = NULL;
Button* EditDialog::ok_button_ = NULL;
Button* EditDialog::cancel_button_ = NULL;

EditDialog::EditDialog(int width, int height) : Dialog(width, height),
ok_button_listener_(this, response_ok), cancel_button_listener_(this, response_cancel),
edit_listener_(this)
{
	if (!ok_button_)
	{
		ok_button_ = new Button(button_width_, button_height_);
		ok_button_->set_text("确定");
		static PIMAGE icon_ok = newimage();
		getimage_mem(icon_ok, binary_dialog_button_ok_png, sizeof(binary_dialog_button_ok_png));
		ok_button_->set_icon(icon_ok);
	}
	if (!cancel_button_)
	{
		cancel_button_ = new Button(button_width_, button_height_);
		cancel_button_->set_text("取消");
		static PIMAGE icon_cancel = newimage();
		getimage_mem(icon_cancel, binary_dialog_button_cancel_png, sizeof(binary_dialog_button_cancel_png));
		cancel_button_->set_icon(icon_cancel);
	}
	if (!dialog_icon_)
	{
		dialog_icon_ = newimage();
		getimage_mem(dialog_icon_, binary_dialog_icon_edit_png, sizeof(binary_dialog_icon_edit_png));
	}

	cancel_button_->set_position(width_ - button_margin_ - button_width_, height_ - button_margin_ - button_height_);
	ok_button_->set_position(width_ - button_margin_ * 2 - button_width_ * 2, height_ - button_margin_ - button_height_);

	edit_text_ = new EditText(width_ - padding_ * 2);
	text_ = "An editing dialog.";
}

EditDialog::~EditDialog()
{
	delete edit_text_;
}

bool EditDialog::on_dialog_close()
{
	response_type_ = response_close;
	DestroyWindow(dialog_handle_);
	return true;
}

void EditDialog::on_dialog_init()
{
	Gobang::set_font(font_family_.c_str(), font_size_, false, false, dialog_image_);
	setcolor(BLACK, dialog_image_);
	static UINT text_format = DT_LEFT | DT_TOP | DT_EDITCONTROL | DT_WORDBREAK | DT_WORD_ELLIPSIS;
	//rectangle(text_rect_.left, text_rect_.top, text_rect_.right, text_rect_.bottom, dialog_image_);
	//计算文本高度
	text_rect_.top = padding_;
	text_rect_.left = icon_margin_ + getwidth(dialog_icon_) + text_margin_;
	text_rect_.bottom = height_ - button_area_height_;
	text_rect_.right = width_ - padding_;
	int text_height = DrawText(dialog_dc_, text_.c_str(), -1, &text_rect_, text_format | DT_CALCRECT);
	text_rect_.top = (padding_ + (height_ - button_area_height_ - edit_margin_bottom_ - edit_text_->get_height() - text_margin_)) / 2 - text_height / 2;
	text_rect_.bottom = text_rect_.top + text_height + 1;
	DrawText(dialog_dc_, text_.c_str(), -1, &text_rect_, text_format);

	putimage_withalpha(dialog_image_, dialog_icon_,
		icon_margin_,
		(text_rect_.top + text_rect_.bottom - getheight(dialog_icon_)) / 2);

	setfillcolor(EGERGB(230, 230, 230), dialog_image_);
	bar(0, height_ - button_area_height_, width_, height_, dialog_image_);
	edit_text_->set_position(padding_, height_ - button_area_height_ - edit_margin_bottom_ - edit_text_->get_height());

	response_type_ = response_none;
	ok_button_->set_on_click_listener(&ok_button_listener_);
	cancel_button_->set_on_click_listener(&cancel_button_listener_);
	edit_text_->set_on_activate_listener(&edit_listener_);
}

void EditDialog::on_create_message()
{
	edit_text_->show(dialog_handle_);
	edit_text_->set_text(input_text_.c_str());
	edit_text_->set_focus();
	ok_button_->show(dialog_handle_);
	cancel_button_->show(dialog_handle_);
}

void EditDialog::ButtonListener::on_click()
{
	dialog_->response_type_ = type_;
	if (type_ == response_ok)
		dialog_->edit_text_->get_text(dialog_->input_text_);
	DestroyWindow(dialog_->dialog_handle_);
}

void EditDialog::EditListener::on_activate()
{
	dialog_->response_type_ = response_ok;
	dialog_->edit_text_->get_text(dialog_->input_text_);
	DestroyWindow(dialog_->dialog_handle_);
}
