#include "ImageButton.h"
#ifdef _MSC_VER
#pragma warning(disable:4018)
#endif

ImageButton::ImageButton() : BaseButton()
{
	normal_images_ = vector<ImageInfo>();
	hover_images_ = vector<ImageInfo>();
	press_images_ = vector<ImageInfo>();
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
}

void ImageButton::add_hover_image(Image* image, int padding_left, int padding_top)
{
	ImageInfo info;
	info.image = image;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	hover_images_.push_back(info);
}

void ImageButton::add_press_image(Image* image, int padding_left, int padding_top)
{
	ImageInfo info;
	info.image = image;
	info.padding_left = padding_left;
	info.padding_top = padding_top;
	press_images_.push_back(info);
}

void ImageButton::show()
{
	int i;
	ImageInfo info;
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
		for (i = 0; i < hover_images_.size(); i++)
		{
			info = hover_images_.at(i);
			info.image->show_image_with_alpha(pos_x_ + info.padding_left, pos_y_ + info.padding_top, 1.0);
		}
	}
}

void ImageButton::on_mouse_click()
{
	int i;
	ImageInfo info;
	for (i = 0; i < press_images_.size(); i++)
	{
		info = press_images_.at(i);
		info.image->show_image_with_alpha(pos_x_ + info.padding_left, pos_y_ + info.padding_top, 1.0);
	}
}

