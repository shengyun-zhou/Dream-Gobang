#pragma once
#include "BaseButton.h"
#include "Image.h"
#include <vector>
using namespace std;
class ImageButton : public BaseButton
{
private:
	struct ImageInfo
	{
		Image* image;
		int padding_left;
		int padding_top;
	};

	vector<ImageInfo> normal_images_;
	vector<ImageInfo> hover_images_;
	vector<ImageInfo> press_images_;
public:
	ImageButton();
	virtual ~ImageButton();

	virtual void add_normal_image(Image* image, int padding_left, int padding_top);
	virtual void add_hover_image(Image* image, int padding_left, int padding_top);
	virtual void add_press_image(Image* image, int padding_left, int padding_top);

	virtual void show();
	virtual void on_mouse_hover();
	virtual void on_mouse_click();
};

