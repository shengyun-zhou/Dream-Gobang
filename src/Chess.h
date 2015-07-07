#pragma once
class Chess
{
public:
	Chess();

	enum PieceType
	{
	  BLACK=1,
	  WHITE=-1,
	  EMPTY=0
  };
	enum Direction
	{
	  COL=1,
	  ROW=2,
	  LEFT_RIGHT=3,
	  RIGHT_LEFT
  };

	static const int SIZE = 15;

  PieceType get_player_type();
	void set_point(int x, int y, PieceType value);
	PieceType get_point(int x, int y);
	int judge(int x, int y, Direction dire);          //1�������ң�2�������£�3�������ϵ����£�4�������ϵ�����
private:
	PieceType chess_array_[SIZE] [SIZE];
};