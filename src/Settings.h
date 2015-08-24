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
	bool set_user_name(const string& name);
	string get_client_connect_IP_addr();
	bool set_client_connect_IP_addr(const string& addr);
	string get_client_connect_port();
	bool set_client_connect_port(const string& port);
	string get_server_port();
	bool set_server_port(const string& port);

	bool read_settings();
	void write_settings();
private:
	static const char* file_name_;
	IniSimpleParser file_parser_;

	int str_to_int(const string& str, int start_pos, int end_pos);
	string generate_IP_addr(int num[4]);
	bool check_user_name(const string& name);
	bool check_IP(const string& IP_addr, string& convert_IP);
	bool check_port(const string& port, string& convert_port);
};
