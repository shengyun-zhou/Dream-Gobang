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

		/**
		* @brief 从当前的窗口图像构造Image对象
		*/
		Image();

		/**
		* @brief 从内存中的图像数据构造Image对象
		* @param mem_data 存储图像数据的数组
		* @param data_size 数组数据的总大小
		*/
		Image(const void* mem_data, long data_size);
		~Image();

		/**
		 * @brief 将图像显示在屏幕上
		 * @param x 图像显示的目标区域的左上角x坐标
		 * @param y 图像显示的目标区域的左上角y坐标
		 */
		void show_image(int x, int y) const;

    /**
		 * @brief 将图像以指定的透明度显示在屏幕上
		 * @param x 图像显示的目标区域的左上角x坐标
		 * @param y 图像显示的目标区域的左上角y坐标
		 * @param alpha 透明度，范围为0.0 - 1.0，0.0为完全透明，1.0为完全不透明
		 * @attention 非PNG格式的图片请勿使用此函数,PNG图片若要正常显示请使用此函数
		 */
		void show_image_with_alpha(int x, int y, double alpha) const;

		int get_width()
		{
			return getwidth(image_);
		}

		int get_height()
		{
			return getheight(image_);
		}
};
