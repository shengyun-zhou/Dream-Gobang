#pragma once
#include "graphics.h"
namespace Gobang
{
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const int USER_NAME_MAX_LEN = 20;

	static const char* message_ready = "Ready";
	static const char* message_quit = "Quit";
	static const char* message_received = "Received";
	static const char* message_head_piece_type = "Piece-Type:";
	static const char* message_head_chess_pos = "Chess-Pos:";
	static const char* message_head_user_name = "User-Name:";
	static const char* text_black_piece = "black";
	static const char* text_white_piece = "white";

	static const char* font_res[] = { "res/font-llt.ttc", "res/font-yahei-mono.ttf" };
	static const char* font_default = "Microsoft Yahei Mono";
	static const char* font_llt = "ÂÜÀòÌå µÚ¶þ°æ";

	void load_font_res();
	void remove_font_res();
	void set_font(const char* font_family, int font_size, bool has_underline = false, bool is_bold = false, PIMAGE img = NULL);
}
