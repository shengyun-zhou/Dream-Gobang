#include "IniSimpleParser.h"
#include <iostream>
#include <exception>
#pragma warning(disable:4018)

IniSimpleParser::IniSimpleParser()
{
}

IniSimpleParser::~IniSimpleParser()
{
}

bool IniSimpleParser::is_content_available(string& content)
{
	if (content.length() <= 0)
		return false;
	bool flag = false;
	bool first_flag = false;
	bool seperator_flag = false;
	bool value_flag = false;
	int i;
	for (i = 0; i < content.length(); i++)
	{
		if (content[i] == ' ')
			continue;
		if ((content[i] == '[' || content[i] == '#' || content[i] == '=') && !first_flag)
			return false;
		else if (!first_flag)
		{
			first_flag = true;
			continue;
		}
		
		if (first_flag && content[i] == '=' && !seperator_flag)
		{
			seperator_flag = true;
			continue;
		}

		if (seperator_flag)
			value_flag = true;

		if (first_flag && seperator_flag && value_flag)
			break;
	}
	if (first_flag && seperator_flag && value_flag)
		return true;
	return false;
}

string IniSimpleParser::parse_key(string& content, int& seperator_pos)
{
	int i, end_pos = -1;
	for (i = 0; content[i] == ' '; i++);
	int start_pos = i;
	bool space_flag = false;
	for (; content[i] != '='; i++)
	{
		if (content[i] != ' ' && space_flag)
			space_flag = false;
		else if (content[i] == ' ' && !space_flag)
		{
			end_pos = i;
			space_flag = true;
		}
	}
	if (end_pos < 0)
		end_pos = i;
	seperator_pos = i;
	return content.substr(start_pos, end_pos - start_pos);
}

string IniSimpleParser::parse_value(string& content, int start_pos)
{
	int i, result_end_pos = -1;
	for (i  = start_pos; content[i] == ' '; i++);
	int result_start_pos = i;
	bool space_flag = false;
	for (; i < content.length(); i++)
	{
		if (content[i] != ' ' && space_flag)
			space_flag = false;
		else if (content[i] == ' ' && !space_flag)
		{
			result_end_pos = i;
			space_flag = true;
		}
	}
	if (result_end_pos < 0)
		result_end_pos = i;
	return content.substr(result_start_pos, result_end_pos - result_start_pos);
}

bool IniSimpleParser::parse(const char* file_path)
{
	input_file_.open(file_path, ios::in);
	if (input_file_.fail())
		return false;
	settings_pairs_ = map<string, string>();
	string line_content;
	while (input_file_.eof() == false)
	{
		try{
			getline(input_file_, line_content);
		}catch (exception e){
			cout << e.what() << endl;
			input_file_.close();
			return false;
		}
		if (is_content_available(line_content) == false)
			continue;
		int seperator_pos;
		string key = parse_key(line_content, seperator_pos);
		string value = parse_value(line_content, seperator_pos + 1);
		settings_pairs_.insert(pair<string, string>(key, value));
	}
	input_file_.close();
	return true;
}

bool IniSimpleParser::get_value(const string& key, string& value)
{
	map<string, string>::iterator it = settings_pairs_.find(key);
	if (it == settings_pairs_.end())
		return false;
	value = it->second;
	return true;
}

void IniSimpleParser::set_value(const string& key, const string& value)
{
	map<string, string>::iterator it = settings_pairs_.find(key);
	if (it == settings_pairs_.end())
		settings_pairs_.insert(pair<string, string>(key, value));
	else
		settings_pairs_[key] = value;
}

bool IniSimpleParser::write(const char* file_path)
{
	output_file_.open(file_path, ios::out);
	if (output_file_.fail())
		return false;
	map<string, string>::iterator it;
	for (it = settings_pairs_.begin(); it != settings_pairs_.end(); ++it)
	{
		try{
			output_file_ << it->first << " = " << it->second << endl;
		}catch (exception e){
			cout << e.what() << endl;
			output_file_.close();
			return false;
		}
	}
	output_file_.close();
	return true;
}

void IniSimpleParser::lower_convert(string& str)
{
	int i;
	for (i = 0; i < str.length(); i++)
	{
		if ('A' <= str[i] && str[i] <= 'Z')
			str[i] += 'a' - 'A';
	}
}
