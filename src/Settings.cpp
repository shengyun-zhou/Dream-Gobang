#include"Settings.h"
#include<fstream>
static string fill_name="name.txt";
enum color{BLACK=1,WHITE=-1};
Settings::Settings()
{
	color_=BLACK;
}
Settings::Settings(color color_)
{
	this->color_=color_;
}
Settings::color Settings::getter(){
	return color_;
}
void Settings::setter(color color_){
	this->color_=color_;
}
bool Settings::readfine(){
	ifstream input;
	input.open("name.txt");
	int temp;
	if(input.eof()!=1)
	{
		input>>temp;
		if(temp == BLACK)
			color_= BLACK;
		else if(temp == WHITE)
			color_= WHITE;
		else
			return 0;
		input.close();
		return 1;
	}
	else
	{
		input.close();
		return 0;
	}
	}
void Settings::writefine(color color_)
{
	ofstream output;
	output.open("name.txt");
	output<<color_<<" ";
	output.close();
}