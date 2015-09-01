#pragma once
#include "BaseButton.h"
#include "../tools/Image.h"
#include <vector>
using namespace std;
class ImageButton : public BaseButton
{
protected:
	struct ImageInfo
	{
		Image* image;
		int padding_left;
		int padding_top;
	};

	vector<ImageInfo> normal_images_;
	vector<ImageInfo> hover_images_;
	vector<ImageInfo> press_images_;
	int button_width_;
	int button_height_;
private:
	struct BufferImage
	{
		int x;
		int y;
		PIMAGE img;
	};
	BufferImage buffer_img_;
	bool recovery_flag_;
public:
	ImageButton();
	virtual ~ImageButton();

	virtual void add_normal_image(Image* image, int padding_left, int padding_top);
	virtual void add_hover_image(Image* image, int padding_left, int padding_top);
	virtual void add_press_image(Image* image, int padding_left, int padding_top);

	virtual void show();
	virtual bool is_mouse_in_button(int x, int y);
	virtual void on_mouse_hover();
	virtual void on_mouse_click();

	virtual int get_width()
	{
		return button_width_;
	}

	virtual int get_height()
	{
		return button_height_;
	}

	void set_recovery_flag(bool recovery_flag)
	{
		recovery_flag_ = recovery_flag;
	}
};

