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

string Settings::get_user_name()
{
	string value;
	if (file_parser_.get_value("user-name", value))
		return value;
	else
		return string();
}

void Settings::set_user_name(const string& name)
{
	if (name.size() > 0)
		file_parser_.set_value("user-name", name);
}

string Settings::get_client_connect_IP_addr()
{
	string value;
	if (file_parser_.get_value("client-connect-IP", value))
		return value;
	else
	{
		file_parser_.set_value("client-connect-IP", "127.0.0.1");
		return "127.0.0.1";
	}
}

void Settings::set_client_connect_IP_addr(const string& addr)
{
	if (addr.size() > 0)
		file_parser_.set_value("client-connect-IP", addr);
}

string Settings::get_client_connect_port()
{
	string value;
	if (file_parser_.get_value("client-connect-port", value))
		return value;
	else
	{
		file_parser_.set_value("client-connect-port", "10086");
		return "10086";
	}
}

void Settings::set_client_connect_port(const string& port)
{
	if (port.size() > 0)
		file_parser_.set_value("client-connect-port", port);
}

string Settings::get_server_port()
{
	string value;
	if (file_parser_.get_value("server-port", value))
		return value;
	else
	{
		file_parser_.set_value("server-port", "10086");
		return "10086";
	}
}

void Settings::set_server_port(const string& port)
{
	if (port.size() > 0)
		file_parser_.set_value("server-port", port);
}

bool Settings::read_settings()
{
	return file_parser_.parse(file_name_);
}

void Settings::write_settings()
{
	file_parser_.write(file_name_);
}
