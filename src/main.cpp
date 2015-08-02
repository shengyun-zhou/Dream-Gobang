#include "WelcomeInterface.h"
#include "SettingsInterface.h"
#include "Music.h"
#include "Settings.h"
#include "MessageDialog.h"
#include <stdlib.h>
#include <direct.h>
#include <winuser.h>

const int WIDTH = 1280;
const int HEIGHT = 720;
Music* game_bgm = NULL;
Settings* game_settings = NULL;

void main_loop();
void play_chess_interface();
void game_settings_interface();
void game_curtain();

void play_button_click_audio()
{
	static Music audio_button_click("res/button-click.wav", 1);
	if (game_settings && game_settings->is_audio_on())
		audio_button_click.start();
}

int main()
{
	char cwd[1024];
	GetModuleFileName(NULL, cwd, sizeof(cwd));				//获取当前进程所在的完整路径
	strrchr(cwd, '\\')[0] = '\0';											//去掉路径中的EXE进程文件名
	chdir(cwd);																				//切换工作目录至进程文件所在目录

	//本函数为修改EGE库后新增的函数
	//载入鼠标样式文件并设置鼠标样式，需要在窗口初始化前调用。
	setcursorstyle("res/cursor.ani");

	initgraph(WIDTH, HEIGHT);

	//MessageDialog dialog(500,150);
	//dialog.set_text("Dream-Gobang游戏启动成功。");
	//dialog.show();

	setbkcolor(WHITE);
	setcaption("梦幻五子棋");

	WelcomeInterface::opening_animation();

	game_bgm = new Music("res/game-bgm.wma", -1, 2);
	game_bgm->set_volume(0.85);

	game_settings = new Settings();
	if (game_settings->read_settings() == false)
		game_settings->write_settings();
	if (game_settings->is_audio_on())
		game_bgm->start();

	main_loop();
	return 0;
}


void main_loop()
{
	mouse_msg msg_;
	int k;
	bool welcome_flag;
	WelcomeInterface welcome_interface;
	WelcomeInterface::action_type welcome_action;

	while (is_run())
	{
		welcome_interface.show_welcome();
		welcome_flag = false;
		while (is_run())
		{
			msg_ = getmouse();
			welcome_action = welcome_interface.action_judge(msg_.x, msg_.y);
			welcome_interface.on_mouse_move(welcome_action);
			if (msg_.is_down() && msg_.is_left())
				welcome_interface.on_mouse_click(welcome_action);
			k = ((msg_.is_up() && msg_.is_left() == 1) ? 1 : 0);
			if (k == 1)
			{
				switch (welcome_action)
				{
					case WelcomeInterface::ACTION_ENTER_GAME:
					{
						play_button_click_audio();
						play_chess_interface();
						welcome_flag = true;
						break;
					}
					case WelcomeInterface::ACTION_GAME_SETTINGS:
					{
						play_button_click_audio();
						game_settings_interface();
						welcome_flag = true;
						break;
					}
					case WelcomeInterface::ACTION_EXIT_GAME:
						closegraph();
						exit(0);
					case WelcomeInterface::ACTION_SPECIAL_THANKS:
					{
						play_button_click_audio();
						game_curtain();
						welcome_flag = true;
						break;
					}
					default:
						break;
				}
				if (welcome_flag)
					break;
			}
		}
	}
}

void play_chess_interface()
{
	PlayChess::ACTION_TYPE n;
	int m;
	mouse_msg msg;
	while (is_run())
	{
		Chess c;
		c.set_man_picetype(game_settings->get_piece_color());
		PlayChess* p = new PlayChess(&c);
		ComputerPlayer cp(c.get_computer_piecetype(), &c);
		p->show_chessboard();
		//p -> show_last_game();
		int row, col;
		if (c.get_computer_piecetype() == Chess::BLACK)												//电脑执黑子先下
		{
			cp.calc_next_step(row, col);
			p->play_chess_by_computer(row, col, c.get_computer_piecetype());
			c.set_point(row, col, c.get_computer_piecetype());
		}
		while (is_run())																											//循环接收鼠标坐标
		{
			msg = getmouse();
			n = p->action_judge(msg.x, msg.y);
			m = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);
      p->on_mouse_move(n);
      if(msg.is_down() && msg.is_left())
        p->on_mouse_click(n);
			if (n == PlayChess::ACTION_PLAY && m == 1 && (p->show_outcome(c) == false))								//下棋
			{
				play_button_click_audio();
				p->play_chess_by_man(msg.x, msg.y, c.get_man_piecetype());
				c.set_point(p->mouse_to_row(msg.x, msg.y), p->mouse_to_col(msg.x, msg.y), c.get_man_piecetype());
				p->show_outcome(c);
				cp.calc_next_step(row, col);
				printf("computer_row:%d, computer_col:%d\n", row, col);
				p->play_chess_by_computer(row, col, c.get_computer_piecetype());
				c.set_point(row, col, c.get_computer_piecetype());
				p->show_outcome(c);
				c.show_chess();																						//调试输出
			}
			else if (n == PlayChess::ACTION_REPLAY && m == 1)						//重新开始
			{
				play_button_click_audio();
				p->update_windows(c);
				delete p;
				break;
			}
			else if (n == PlayChess::ACTION_QUIT && m == 1)						//退出游戏
			{
				play_button_click_audio();
				//Save save;
				//p -> save_last_game(save, c);
				return;
			}
		}
	}
}

void game_settings_interface()
{
	mouse_msg msg;
	int k;
	SettingsInterface settings_interface(game_settings);
	SettingsInterface::action_type settings_action;

	settings_interface.update_interface();
	while (is_run())
	{
		msg = getmouse();
		k = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);
		if (k == 1)
		{
			settings_action = settings_interface.action_judge(msg.x, msg.y);
			switch (settings_action)
			{
				case SettingsInterface::ACTION_SELECT_BLACK:
					play_button_click_audio();
					game_settings->set_piece_color(Chess::BLACK);
					break;
				case SettingsInterface::ACTION_SELECT_WHITE:
					play_button_click_audio();
					game_settings->set_piece_color(Chess::WHITE);
					break;
				case SettingsInterface::ACTION_SWITCH_AUDIO:
					if (game_settings->is_audio_on())
					{
						game_settings->set_audio(false);
						game_bgm->stop();
					}
					else
					{
						game_settings->set_audio(true);
						play_button_click_audio();
						game_bgm->start();
					}
					break;
				case SettingsInterface::ACTION_BACK:
					game_settings->write_settings();
					play_button_click_audio();
					return;
			}
			settings_interface.update_interface();
		}
	}
}

void game_curtain()
{
	mouse_msg msg;
	int k;
	const int offset = 10;

	WelcomeInterface::curtain();
	while (is_run())
	{
		msg = getmouse();
		k = (msg.is_up() && msg.is_left() ? 1 : 0);
		if (k == 1 && msg.x > 800 - offset && msg.x < 950 - offset && msg.y > 600 - offset && msg.y < 650 + offset)
		{
			play_button_click_audio();
			return;
		}
	}
}

