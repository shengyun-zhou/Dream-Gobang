#pragma once
#include <string>
#include "Chess.h"
#include "tools/IniSimpleParser.h"

using namespace std;

class Settings
{
public:
	Settings();
	Chess::PieceType get_piece_color();
	void set_piece_color(Chess::PieceType color);
	bool is_audio_on();
	void set_audio(bool is_on);
	string get_user_name();
	void set_user_name(const string& name);
	string get_client_connect_IP_addr();
	void set_client_connect_IP_addr(const string& addr);
	string get_client_connect_port();
	void set_client_connect_port(const string& port);
	string get_server_port();
	void set_server_port(const string& port);

	bool read_settings();
	void write_settings();
private:
	static const char* file_name_;
	IniSimpleParser file_parser_;
};
