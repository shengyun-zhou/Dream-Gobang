#include "Settings.h"

const char* Settings::file_name = "game-settings.ini";

Settings::Settings()
{
	color_ = Chess::BLACK;
	is_audio_on_ = true;
}


Chess::PieceType Settings::get_piece_color()
{
	return color_;
}

void Settings::set_piece_color(Chess::PieceType color_)
{
	this -> color_ = color_;
}

bool Settings::is_audio_on()
{
	return is_audio_on_;
}

void Settings::set_audio(bool is_on)
{
	this->is_audio_on_ = is_on;
}
bool Settings::read_settings()
{
	ifstream input;
	input.open(file_name, ios_base::in);
	if (input.fail())
		return false;

	bool read_success_flag = true;
	string temp;
	input >> temp;
	if (temp == "piece-type=black")
		color_ = Chess::BLACK;
	else if (temp == "piece-type=white")
		color_ = Chess::WHITE;
	else
	{
		color_ = Chess::BLACK;
		read_success_flag = false;
	}

	input >> temp;
	if (temp == "audio=on")
		is_audio_on_ = true;
	else if (temp == "audio=off")
		is_audio_on_ = false;
	else
	{
		is_audio_on_ = true;
		read_success_flag = false;
	}

	input.close();
	return read_success_flag;
}


void Settings::write_settings()
{
	ofstream output;
	output.open(file_name, ios_base::out);
	if (output.fail())
		return;

	if (color_ == Chess::BLACK)
		output << "piece-type=black" << endl;
	else
		output << "piece-type=white" << endl;

	if (is_audio_on_)
		output << "audio=on" << endl;
	else
		output << "audio=off" << endl;

	output.close();
}
