#pragma once
class BaseButton
{
protected:
	bool button_hover_flag_;
	int pos_x_;
	int pos_y_;
public:
	class OnClickListener
	{
	public:
		virtual void on_click() = 0;
	};

	BaseButton()
	{
		button_hover_flag_ = false;
		pos_x_ = pos_y_ = 0;
	}
	virtual ~BaseButton(){}

	void set_position(int x, int y)
	{
		pos_x_ = x;
		pos_y_ = y;
	}

	inline bool get_hover_status()
	{
		return button_hover_flag_;
	}

	virtual void show() = 0;
	virtual void on_mouse_hover() = 0;
	virtual void on_mouse_click() = 0;
};

