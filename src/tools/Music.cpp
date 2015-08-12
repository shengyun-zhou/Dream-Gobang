#include "Music.h"
#include <time.h>

HWND Music::win32_handle_ = NULL;

Music::Music(const char* file_path, int n_play, int interval_time)
{
	file_path_ = file_path;
	n_play_ = n_play;
	interval_time_ = interval_time;
	thread_handle_ = NULL;
	is_running_ = false;
	volume_ = 1.0;
	pause_mission_ = resume_mission_ = stop_mission_ = false;
	if (win32_handle_ == NULL)
		win32_handle_ = getHWnd();
}

Music::~Music()
{
	stop();
}

DWORD WINAPI Music::on_player_running(LPVOID data)
{
	Music* music_data = (Music*)data;
	MUSIC music_player;
	int num_play = music_data -> n_play_;
	bool is_window_iconic = false;
	BOOL window_iconic_status;
	clock_t start_time;
	HWND active_window;
	while (num_play == -1 || num_play > 0)
	{
		//printf("start_playing %d times...\n", num_play);
		music_player.OpenFile(music_data->file_path_);
		music_player.SetVolume(music_data->volume_);
		music_player.Play();
		while (music_player.GetPlayStatus() != MUSIC_MODE_STOP)
		{
			window_iconic_status = IsIconic(Music::win32_handle_);
			active_window = GetForegroundWindow();
			if ((window_iconic_status || active_window != Music::win32_handle_) && is_window_iconic == false)
			{
				is_window_iconic = true;
				//printf("on_window_iconic.\n");
				music_player.Pause();
			}
			else if ((!window_iconic_status && active_window == Music::win32_handle_) && is_window_iconic)
			{
				music_player.Play();
				is_window_iconic = false;
			}
			if (music_data->pause_mission_)
			{
				music_player.Pause();
				music_data->pause_mission_ = false;
			}
			if (music_data->resume_mission_)
			{
				if (!window_iconic_status && active_window == Music::win32_handle_)
					music_player.Play();
				music_data->resume_mission_ = false;
			}
			if (music_data->stop_mission_)
			{
				music_player.Stop();
				music_data->is_running_ = false;
				music_data->stop_mission_ = false;
				return 0;
			}
			api_sleep(250);
		}
		if (num_play > 0)
			num_play--;
		if (num_play == 0)
			break;
		start_time = clock();
		while ((clock() - start_time) / CLOCKS_PER_SEC < music_data->interval_time_)
			api_sleep(250);
	}
	music_data->is_running_ = false;
	return 0;
}

void Music::set_interval_time(int time_second)
{
	interval_time_ = time_second;
}

void Music::set_volume(float volume)
{
	volume_ = volume;
}

void Music::start()
{
	if (is_running_ == false)
	{
		thread_handle_ = CreateThread(NULL, 0, on_player_running, this, 0, NULL);
	}
}

void Music::stop()
{
	if (is_running_)
	{
		stop_mission_ = true;
		while (stop_mission_)
			api_sleep(50);
		CloseHandle(thread_handle_);
	}
}

void Music::pause()
{
	if (is_running_)
	{
		pause_mission_ = true;
		while (pause_mission_)
			api_sleep(50);
		SuspendThread(thread_handle_);
	}
}

void Music::resume()
{
	if (is_running_)
	{
		resume_mission_ = true;
		ResumeThread(thread_handle_);
		while (resume_mission_)
			api_sleep(50);
	}
}

