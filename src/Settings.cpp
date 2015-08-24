#include "Settings.h"
#include <string>
#ifdef _MSC_VER
#pragma warning(disable:4018)
#endif

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

bool Settings::check_user_name(const string& name)
{
	int i;
	for (i = 0; i < name.length(); i++)
	{
		if (name[i] == ' ')
			return false;
	}
	return true;
}

bool Settings::set_user_name(const string& name)
{
	if (name.length() > 0)
	{
		if (check_user_name(name))
		{
			file_parser_.set_value("user-name", name);
			return true;
		}
		return false;
	}
	return true;
}

int Settings::str_to_int(const string& str, int start_pos, int end_pos)
{
	if (end_pos < start_pos)
		return -1;
	int i, k = 1;
	int result = 0;
	for (i = end_pos; i >= start_pos; i--, k *= 10)
		result += (str[i] - '0') * k;
	return result;
}

string Settings::generate_IP_addr(int num[4])
{
	int i;
	string result;
	char temp[20];
	for (i = 0; i < 4; i++)
	{
		sprintf(temp, "%d", num[i]);
		result += temp;
		result += '.';
	}
	return result.substr(0, result.length() - 1);
}

bool Settings::check_IP(const string& IP_addr, string& convert_IP)
{
	int point_num = 0;
	int pos1 = 0, pos2 = 0;
	int i, temp;
	int num[4] = { 0 };
	if (IP_addr.length() <= 0 || IP_addr.length() > 15)
		return false;
	for (i = 0; i < IP_addr.length(); i++)
	{
		if ('0' <= IP_addr[i] && IP_addr[i] <= '9')
			continue;
		else if (IP_addr[i] == '.')
		{
			point_num++;
			pos2 = i - 1;
			temp = str_to_int(IP_addr, pos1, pos2);
			if (temp < 0 || temp>255)
				return false;
			num[point_num - 1] = temp;
			pos1 = i + 1;
		}
		else
			return false;
	}
	if (point_num != 3)
		return false;
	temp = str_to_int(IP_addr, pos1, i - 1);
	if (temp < 0 || temp>255)
		return false;
	num[3] = temp;
	convert_IP = generate_IP_addr(num);
	return true;
}

string Settings::get_client_connect_IP_addr()
{
	string value;
	if (file_parser_.get_value("client-connect-IP", value))
	{
		if (check_IP(value, value))
			return value;
		else
		{
			file_parser_.set_value("client-connect-IP", "127.0.0.1");
			return "127.0.0.1";
		}
	}
	else
	{
		file_parser_.set_value("client-connect-IP", "127.0.0.1");
		return "127.0.0.1";
	}
}

bool Settings::set_client_connect_IP_addr(const string& addr)
{
	if (addr.length() > 0)
	{
		string temp;
		if (check_IP(addr, temp))
		{
			printf("temp == %s\n", temp.c_str());
			file_parser_.set_value("client-connect-IP", temp);
			return true;
		}
		return false;
	}
	return true;
}

bool Settings::check_port(const string& port, string& convert_port)
{
	int i, temp;
	char num[10];
	if (port.length() <= 0 || port.length() > 5)
		return false;
	for (i = 0; i < port.length(); i++)
	{
		if ('0' <= port[i] && port[i] <= '9')
			continue;
		else
			return false;
	}
	temp = str_to_int(port, 0, port.length() - 1);
	if (temp < 0 || temp > 65535)
		return false;
	sprintf(num, "%d", temp);
	convert_port = num;
	return true;
}

string Settings::get_client_connect_port()
{
	string value;
	if (file_parser_.get_value("client-connect-port", value))
	{
		if (check_port(value, value))
			return value;
		else
		{
			file_parser_.set_value("client-connect-port", "10086");
			return "10086";
		}
	}
	else
	{
		file_parser_.set_value("client-connect-port", "10086");
		return "10086";
	}
}

bool Settings::set_client_connect_port(const string& port)
{
	if (port.length() > 0)
	{
		string temp;
		if (check_port(port, temp))
		{
			file_parser_.set_value("client-connect-port", temp);
			return true;
		}
		return false;
	}
	return true;
}

string Settings::get_server_port()
{
	string value;
	if (file_parser_.get_value("server-port", value))
	{
		if (check_port(value, value))
			return value;
		else
		{
			file_parser_.set_value("server-port", "10086");
			return "10086";
		}
	}
	else
	{
		file_parser_.set_value("server-port", "10086");
		return "10086";
	}
}

bool Settings::set_server_port(const string& port)
{
	if (port.length() > 0)
	{
		string temp;
		if (check_port(port, temp))
		{
			file_parser_.set_value("server-port", temp);
			return true;
		}
		return false;
	}
	return true;
}

bool Settings::read_settings()
{
	return file_parser_.parse(file_name_);
}

void Settings::write_settings()
{
	file_parser_.write(file_name_);
}
