#pragma once
#include <string>
#include <fstream>
#include "Chess.h"
using namespace std;

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
	static const char* file_name;
	Chess::PieceType color_;
	bool is_audio_on_;
};
