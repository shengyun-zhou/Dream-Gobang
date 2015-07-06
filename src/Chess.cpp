#include"Chess.h"
Chess::Chess()
{
	int i, j;
	for(i = 0;i < SIZE;i++)
  {
		for(j = 0;j < SIZE;j++)
			chess_array[i][j] = EMPTY;
	}
}
void Chess::set_point(int x, int y, PieceType value)
{
	chess_array[x][y] = value;
}

Chess::PieceType Chess::get_point(int x, int y)
{
	return chess_array[x][y];
}

//1代表左右，2代表上下，3代表左上到右下，4代表右上到左下
int Chess::judge(int x, int y, Direction dire)
{
  PieceType temp = chess_array[x][y];
	int result1, result2;
	result1 = 0;
	result2 = 0;
	int x_min, y_min, x_max, y_max;
	x_min = (x - 4 > 0) ? x - 4 : 0;
	y_min = (y - 4 > 0) ? y - 4 : 0;
	x_max = (x + 4 < SIZE) ? x + 4 : SIZE;
	y_max = (y + 4 < SIZE) ? y + 4 : SIZE;
	switch(dire){
	case 1:
		for(; x_min < x; x_min++)
		{
			if(chess_array[x_min][y] == temp)
				result1++;
			else
				result1 = 0;
		}
		for(; x_max > x; x_max--)
		{
			if(chess_array[x_max][y] == temp)
				result2++;
			else
				result2 = 0;
		}
		break;
	case 2:
		for(; y_min < y; y_min++)
		{
			if(chess_array[x][y_min] == temp)
				result1++;
			else
				result1 = 0;
		}
    for(; y_max > y; y_max--)
		{
			if(chess_array[x][y_max] == temp)
				result2++;
			else
				result2 = 0;
		}
    break;
	case 3:
		int t1;
	  t1 = (x - x_min > y - y_min) ? y - y_min : x - x_min;
		x_min = x-t1;
		y_min = y-t1;
		for(; x_min < x; x_min++, y_min++)
		{
			if(chess_array[x_min][y_min] == temp)
				result1++;
			else
				result1 = 0;
		}
		int t2;
	  t2 = (x_max - x > y_max-y) ? y_max-y : x_max-x;
		x_max = x + t2;
		y_max = y + t2;
		for(; x_max>x; x_max--, y_max--)
		{
			if(chess_array[x_max][y_max] == temp)
				result2++;
			else
				result2 = 0;
		}
		break;
	case 4:
		int t3;
		t3 = (y_max-y > x-x_min) ? x - x_min : y_max - y;
		y_max = y + t3;
		x_min = x - t3;
		for(; x_min < x; x_min++, y_max--)
		{
			if(chess_array[x_min][y_max] == temp)
				result1++;
			else
				result1 = 0;
		}
		int t4;
		t4 = (x_max - x > y - y_min) ? y - y_min : x_max - x;
		x_max = x + t4;
		y_min = y - t4;
		for(; y_min < y; y_min++, x_max--)
		{
			if(chess_array[x_max][y_min] == temp)
				result2++;
			else
				result2 = 0;
		}
	}
  return result1+result2+1;
}
