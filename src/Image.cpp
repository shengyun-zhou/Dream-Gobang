#include "Image.h"

Image::Image(const char* file_path)
{
	image_ = newimage();
	getimage(image_, file_path);
}

Image::~Image()
{
	delimage(image_);
}

void Image::show_image(int x, int y)
{
	putimage(x, y, image_);
}

void Image::show_image_with_alpha(int x, int y, double alaph)
{
	putimage_withalpha(NULL, image_, alaph, x, y);
}
