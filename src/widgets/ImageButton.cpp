#include "ImageButton.h"
#ifdef _MSC_VER
#pragma warning(disable:4018)
#endif
#define INFINITE_MAX 10000000

ImageButton::ImageButton() : BaseButton()
{
	normal_images_ = vector<ImageInfo>();
	hover_images_ = vector<ImageInfo>();
	press_images_ = vector<ImageInfo>();
	button_width_ = button_height_ = 0;
	buffer_img_.x = buffer_img_.y = -INFINITE_MAX;
	buffer_img_.img = NULL;
	recovery_flag_ = true;
}

ImageButton::~ImageButton()
{
}

void ImageButton::add_normal_image(Image* image, int padding_left, int padding_top)
{
	ImageInfo info;
	info.image = image;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	normal_images_.push_back(info);
	if (image->get_width() > button_width_)
		button_width_ = image->get_width();
	if (image->get_height() > button_height_)
		button_height_ = image->get_height();
}

void ImageButton::add_hover_image(Image* image, int padding_left, int padding_top)
{
	ImageInfo info;
	info.image = image;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	hover_images_.push_back(info);
	if (image->get_width() > button_width_)
		button_width_ = image->get_width();
	if (image->get_height() > button_height_)
		button_height_ = image->get_height();
}

void ImageButton::add_press_image(Image* image, int padding_left, int padding_top)
{
	ImageInfo info;
	info.image = image;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	press_images_.push_back(info);
	if (image->get_width() > button_width_)
		button_width_ = image->get_width();
	if (image->get_height() > button_height_)
		button_height_ = image->get_height();
}

void ImageButton::show()
{
	int i;
	ImageInfo info;
	if (pos_x_ != buffer_img_.x || pos_y_ != buffer_img_.y)					//将按钮所占用的屏幕区域在贴图之前缓存下来
	{
		if (buffer_img_.img)
		{
			if (recovery_flag_)
				putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
			delimage(buffer_img_.img);
		}
		int temp_viewport_left, temp_viewport_right, temp_viewport_top, temp_viewport_bottom;
		getviewport(&temp_viewport_left, &temp_viewport_top, &temp_viewport_right, &temp_viewport_bottom);
		setviewport(pos_x_, pos_y_, pos_x_ + button_width_, pos_y_ + button_height_);
		buffer_img_.x = pos_x_;
		buffer_img_.y = pos_y_;
		buffer_img_.img = newimage(button_width_, button_height_);
		putimage(buffer_img_.img, 0, 0, NULL);
		setviewport(temp_viewport_left, temp_viewport_top, temp_viewport_right, temp_viewport_bottom);
	}
	else if (buffer_img_.img)
		putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
	button_hover_flag_ = false;
	for (i = 0; i < normal_images_.size(); i++)
	{
		info = normal_images_.at(i);
		info.image->show_image_with_alpha(pos_x_ + info.padding_left, pos_y_ + info.padding_top, 1.0);
	}
}

void ImageButton::on_mouse_hover()
{
	if (button_hover_flag_ == false)
	{
		int i;
		ImageInfo info;
		button_hover_flag_ = true;
		if (buffer_img_.img)
			putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
		for (i = 0; i < hover_images_.size(); i++)
		{
			info = hover_images_.at(i);
			info.image->show_image_with_alpha(pos_x_ + info.padding_left, pos_y_ + info.padding_top, 1.0);
		}
	}
}

bool ImageButton::is_mouse_in_button(int x, int y)
{
	if (pos_x_ <= x && x <= pos_x_ + button_width_ &&
		pos_y_ <= y && y <= pos_y_ + button_height_)
		return true;
	return false;
}

void ImageButton::on_mouse_click()
{
	int i;
	ImageInfo info;
	if (buffer_img_.img)
		putimage(buffer_img_.x, buffer_img_.y, buffer_img_.img);
	for (i = 0; i < press_images_.size(); i++)
	{
		info = press_images_.at(i);
		info.image->show_image_with_alpha(pos_x_ + info.padding_left, pos_y_ + info.padding_top, 1.0);
	}
}

