#pragma once
#include <string>
#include "Chess.h"
#include "tools/IniSimpleParser.h"

class Settings
{
public:
	Settings();
	Chess::PieceType get_piece_color();
	void set_piece_color(Chess::PieceType color);
	bool is_audio_on();
	void set_audio(bool is_on);

	bool read_settings();
	void write_settings();
private:
	static const char* file_name_;
	IniSimpleParser file_parser_;
};
