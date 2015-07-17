#include "Interface.h"
#include "Music.h"
#include "Settings.h"
#include <stdio.h>

const int WIDTH = 1280;
const int HEIGHT = 720;
Music* game_bgm = NULL;
Settings* game_settings = NULL;

void action_judge();

void play_button_click_audio()
{
	static Music audio_button_click("res/button-click.wav", 1);
	if (game_settings && game_settings->is_audio_on())
		audio_button_click.start();
}

int main()
{
	initgraph(WIDTH, HEIGHT);
	setbkcolor(WHITE);
	setcaption("梦幻五子棋");
	
	Interface interphase;
	interphase.animation();

	game_bgm = new Music("res/game-bgm.wma", -1, 2);
	game_bgm->set_volume(0.85);

	game_settings = new Settings();
	if (game_settings->read_settings() == false)
		game_settings->write_settings();
	if (game_settings->is_audio_on())
		game_bgm->start();

	interphase.welcome();
	action_judge();
	return 0;
}


void action_judge()
{
ag:
	mouse_msg msg_;
	int k;
	Interface interphase;
	Interface::action_type welcome_action;

	while(is_run())
	{
		msg_ = getmouse();
		welcome_action = interphase.action_judge(msg_.x, msg_.y);
		interphase.on_mouse_move(welcome_action);
		if (msg_.is_down() && msg_.is_left())
			interphase.on_mouse_click(welcome_action);
		k = ((msg_.is_up() && msg_.is_left() == 1) ? 1 : 0);
		if (k == 1)
		{
			switch (welcome_action)
			{
				case Interface::ACTION_ENTER_GAME:
				{
					int n, m;
					mouse_msg msg;
					play_button_click_audio();
				again:
					Chess c;
					c.set_man_picetype(game_settings->get_piece_color());
					PlayChess* p = new PlayChess(&c);
					ComputerPlayer cp(c.get_computer_piecetype(), &c);
					p->show_chessboard();
					//p -> show_last_game();
					int row, col;
					if (c.get_computer_piecetype() == Chess::BLACK)
					{
						cp.calc_next_step(row, col);
						p->play_chess_by_computer(row, col, c.get_computer_piecetype());
						c.set_point(row, col, c.get_computer_piecetype());
					}
					while (is_run())							//循环接收鼠标坐标
					{
						msg = getmouse();
						n = (int)p->action_judge(msg.x, msg.y);
						m = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);

						if (n == 1 && m == 1 && (p->show_outcome(c) == false))								//下棋
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
							c.show_chess();											//调试输出
						}
						else if (n == 2 && m == 1)						//重新开始 
						{
							play_button_click_audio();
							p->update_windows(c);
							delete p;
							goto again;
						}
						else if (n == 3 && m == 1)						//退出游戏 
						{
							play_button_click_audio();
							Save save;
							//p -> save_last_game(save, c);
							interphase.welcome();
							goto ag;
						}
					}
					break;
				}
				case Interface::ACTION_GAME_SETTINGS:
				{
					play_button_click_audio();
					mouse_msg msg;
					int k;

					interphase.set_welcome(game_settings);
					while (is_run())
					{
						msg = getmouse();
						k = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);
						if (k == 1)
						{
							const int offset = 10;
							Chess c;

							//1.设置黑子
							if (msg.x > 30 - offset && msg.x < 180 + offset && msg.y>350 - offset && msg.y < 450 + offset)
							{
								play_button_click_audio();
								game_settings->set_piece_color(Chess::BLACK);
								interphase.set_welcome(game_settings);
							}

							//2.选择白子
							if (msg.x > 260 - offset && msg.x < 410 + offset && msg.y>350 - offset && msg.y < 450 + offset)
							{
								play_button_click_audio();
								game_settings->set_piece_color(Chess::WHITE);
								interphase.set_welcome(game_settings);
							}

							//3.设置音效
							if (msg.x > 30 - offset && msg.x < 180 - offset && msg.y>520 - offset && msg.y < 600 + offset)
							{
								if (game_settings->is_audio_on())
								{
									game_settings->set_audio(false);
									game_bgm->stop();
								}
								else
								{
									game_settings->set_audio(true);
									game_bgm->start();
								}
								play_button_click_audio();
								interphase.set_welcome(game_settings);
							}

							//4.返回
							if (msg.x > 260 - offset && msg.x < 410 - offset && msg.y>500 - offset && msg.y < 600 + offset)
							{
								play_button_click_audio();
								game_settings->write_settings();
								interphase.welcome();
								goto ag;
							}
						}
					}
					break;
				}
				case Interface::ACTION_EXIT_GAME:
					closegraph();
					exit(0);
				case Interface::ACTION_SPECIAL_THANKS:
				{
					play_button_click_audio();
					mouse_msg msg;
					int k;
					const int offset = 10;

					interphase.Curtain();
					while (is_run())
					{
						msg = getmouse();
						k = (msg.is_up() && msg.is_left() ? 1 : 0);
						if (k == 1 && msg.x > 800 - offset && msg.x < 950 - offset && msg.y > 600 - offset && msg.y < 650 + offset)
						{
							play_button_click_audio();
							interphase.welcome();
							goto ag;
						}
					}
					break;
				}
				default:
					break;
			}
		}
	}
}

