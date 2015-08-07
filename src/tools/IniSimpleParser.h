#pragma once
#include <fstream>
#include <map>
#include <string>

using namespace std;

class IniSimpleParser
{
private:
	ifstream input_file_;
	ofstream output_file_;
	map<string, string> settings_pairs_;

	bool is_content_available(string& content);
	string parse_key(string& content, int& seperator_pos);
	string parse_value(string& content, int start_pos);
public:
	IniSimpleParser();
	~IniSimpleParser();

	bool parse(const char* file_path);
	bool get_value(const string& key, string& value);
	void set_value(const string& key, const string& value);
	bool write(const char* file_path);

	static void lower_convert(string& str);
};

