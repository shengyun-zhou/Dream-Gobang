#pragma once
#define SHOW_CONSOLE
#include "graphics.h"
#include <windef.h>
#include <windows.h>
class Music
{
private:
	static HWND win32_handle_;
	int n_play_;
	int interval_time_;
	HANDLE thread_handle_;
	bool is_running_;
	const char* file_path_;
	float volume_;
	bool pause_mission_;
	bool stop_mission_;
	bool resume_mission_;

	static DWORD WINAPI on_player_running(LPVOID music_player);
public:
  /**
  * @brief 从指定的音乐文件创建一个Music对象。
  * @param file_path 指定的文件路径。
  * @param n_play 播放次数，-1为无限次播放。
  * @param interval_time 一次播放完到下一次播放开始的间隔时间，单位为秒。
  * @note 建议音乐文件使用wma格式，MP3格式开头有爆音，wav格式无法调音量。
  * @attention 对象被释放的时候，会自动停止音乐播放并终止监控线程。
  */
	Music(const char* file_path, int n_play, int interval_time = 0);
	~Music();

  /**
  * @brief 设置间隔时间。
  * @param time_second 间隔时间，单位为秒。
  */
	void set_interval_time(int time_second);

	/**
	* @brief 设置音乐音量。
	* @param volume 当前系统音量百分比，范围为0.0 - 1.0。
	* @attention 设置的音量需要到下一次播放开始时才会生效。
	*/
	void set_volume(float volume);

	/**
	* @brief 启动音乐播放，并启动监控线程。
	*/
	void start();

	/**
	* @brief 停止音乐播放，并终止监控线程。
	* @attention 停止后，该对象的已播放次数将重置为0。
	*/
	void stop();

	/**
	* @brief 暂停音乐播放，并挂起监控线程。
	*/
	void pause();

	/**
	* @brief 从暂停状态恢复音乐播放，并恢复监控线程。
	*/
	void resume();
};

