#include "Settings.h"
#include <string>

const char* Settings::file_name_ = "game-settings.ini";

Settings::Settings()
{
}

Chess::PieceType Settings::get_piece_color()
{
	string value;
	if (file_parser_.get_value("piece-type", value))
	{
		IniSimpleParser::lower_convert(value);
		if (value == "white")
			return Chess::WHITE;
		else
			return Chess::BLACK;
	}
	else
	{
		file_parser_.set_value("piece-type", "black");
		return Chess::BLACK;
	}
}

void Settings::set_piece_color(Chess::PieceType color)
{
	if (color == Chess::BLACK)
		file_parser_.set_value("piece-type", "black");
	else if (color == Chess::WHITE)
		file_parser_.set_value("piece-type", "white");
}

bool Settings::is_audio_on()
{
	string value;
	if (file_parser_.get_value("audio", value))
	{
		IniSimpleParser::lower_convert(value);
		if (value == "off")
			return false;
		else
			return true;
	}
	else
	{
		file_parser_.set_value("audio", "on");
		return true;
	}
}

void Settings::set_audio(bool is_on)
{
	if (is_on)
		file_parser_.set_value("audio", "on");
	else
		file_parser_.set_value("audio", "off");
}

bool Settings::read_settings()
{
	return file_parser_.parse(file_name_);
}


void Settings::write_settings()
{
	file_parser_.write(file_name_);
}
