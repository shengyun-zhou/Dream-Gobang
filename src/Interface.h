#pragma once
#define SHOW_CONSOLE
#include "Save.h"
#include "PlayChess.h"
#include "ComputerPlayer.h"
#include "Settings.h"
#include "Image.h"

class Interface
{
public:
	void animation();
	void welcome();
	void set_welcome(Settings* settings);
	void Curtain();
};
