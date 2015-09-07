#include "Gobang.h"
#include <windows.h>
#include <string.h>
#include "image_game_welcome_bg.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace Gobang
{
	void load_font_res()
	{
		int i;
		for (i = 0; i < sizeof(font_res); i++)
			AddFontResource(font_res[i]);
	}

	void remove_font_res()
	{
		int i;
		for (i = 0; i < sizeof(font_res); i++)
			RemoveFontResource(font_res[i]);
	}

	void set_font(const char* font_family, int font_size, bool has_underline, bool is_bold, PIMAGE pimg)
	{
		LOGFONT font;
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &font, NULL);
		strcpy(font.lfFaceName, font_family);
		font.lfHeight = -font_size;
		font.lfWidth = 0;
		font.lfUnderline = has_underline;
		if (is_bold)
			font.lfWeight = 700;
		else
			font.lfWeight = 400;
		if (font_size >= 20)
			font.lfQuality = ANTIALIASED_QUALITY;
		else
			font.lfQuality = DEFAULT_QUALITY;
		setfont(&font, pimg);
	}
}