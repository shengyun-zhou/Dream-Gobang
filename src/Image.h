#pragma once
#include "graphics.h"
class Image
{
	private:
		PIMAGE image_;
	public:
		/**
		* @brief 从指定的文件构造Image对象
		* @param file_path 指定文件的路径
		*/
		Image(const char* file_path);
		~Image();

		/**
		 * @brief 将图像显示在屏幕上
		 * @param x 图像显示的目标区域的左上角x坐标
		 * @param y 图像显示的目标区域的左上角y坐标
		 */
		void show_image(int x, int y);

    /**
		 * @brief 将图像以指定的透明度显示在屏幕上
		 * @param x 图像显示的目标区域的左上角x坐标
		 * @param y 图像显示的目标区域的左上角y坐标
		 * @param alpha 透明度，范围为0.0 - 1.0，0.0为完全透明，1.0为完全不透明
		 * @attention 非PNG格式的图片请勿使用此函数
		 */
		void show_image_with_alpha(int x, int y, double alpha);
};

