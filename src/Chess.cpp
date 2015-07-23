#include "Chess.h"
#include <stdio.h>

Chess::PieceType Chess::picetype_ = Chess::BLACK;

Chess::Chess()
{
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
			chess_array_[i][j] = EMPTY;
	}
}


void Chess::set_point(int x, int y, PieceType value)
{
	chess_array_[x][y] = value;
}


Chess::PieceType Chess::get_point(int x, int y)
{
	return chess_array_[x][y];
}


int Chess::get_empty_grid_num()
{
  int i, j;
  int num = 0;
  for(i = 0; i < SIZE; i++)
  {
    for(j = 0; j < SIZE; j++)
    {
      if(chess_array_[i][j] == EMPTY)
        num++;
    }
  }
  return num;
}


//1代表上下 2代表左右 3代表左上到右下 4代表右上到左下
int Chess::judge(int x, int y, Direction dire)
{
	PieceType temp = chess_array_[x][y];
	int result1, result2;
	result1 = 0;
	result2 = 0;
	int x_min, y_min, x_max, y_max;
	x_min = (x - 4 > 0) ? x - 4 : 0;
	y_min = (y - 4 > 0) ? y - 4 : 0;
	x_max = (x + 4 < SIZE) ? x + 4 : SIZE - 1;
	y_max = (y + 4 < SIZE) ? y + 4 : SIZE - 1;
	switch (dire){
	case 1:
		for (; x_min < x; x_min++)
		{
			if (chess_array_[x_min][y] == temp)
				result1++;
			else
				result1 = 0;
		}
		for (; x_max > x; x_max--)
		{
			if (chess_array_[x_max][y] == temp)
				result2++;
			else
				result2 = 0;
		}
		break;
	case 2:
		for (; y_min < y; y_min++)
		{
			if (chess_array_[x][y_min] == temp)
				result1++;
			else
				result1 = 0;
		}
		for (; y_max > y; y_max--)
		{
			if (chess_array_[x][y_max] == temp)
				result2++;
			else
				result2 = 0;
		}
		break;
	case 3:
		int t1;
		t1 = (x - x_min > y - y_min) ? y - y_min : x - x_min;
		x_min = x - t1;
		y_min = y - t1;
		for (; x_min < x; x_min++, y_min++)
		{
			if (chess_array_[x_min][y_min] == temp)
				result1++;
			else
				result1 = 0;
		}
		int t2;
		t2 = (x_max - x > y_max - y) ? y_max - y : x_max - x;
		x_max = x + t2;
		y_max = y + t2;
		for (; x_max>x; x_max--, y_max--)
		{
			if (chess_array_[x_max][y_max] == temp)
				result2++;
			else
				result2 = 0;
		}
		break;
	case 4:
		int t3;
		t3 = (y_max - y > x - x_min) ? x - x_min : y_max - y;
		y_max = y + t3;
		x_min = x - t3;
		for (; x_min < x; x_min++, y_max--)
		{
			if (chess_array_[x_min][y_max] == temp)
				result1++;
			else
				result1 = 0;
		}
		int t4;
		t4 = (x_max - x > y - y_min) ? y - y_min : x_max - x;
		x_max = x + t4;
		y_min = y - t4;
		for (; y_min < y; y_min++, x_max--)
		{
			if (chess_array_[x_max][y_min] == temp)
				result2++;
			else
				result2 = 0;
		}
	}
	return result1 + result2 + 1;
}


bool Chess::is_chess_full()
{
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (chess_array_[i][j] == EMPTY)
				return false;
		}
	}
	return true;
}


Chess::PieceType Chess::judge_win()
{
	int i, j, dire;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (chess_array_[i][j] != EMPTY)
			{
				for (dire = COL; dire <= RIGHT_LEFT; dire++)
				{
					if (judge(i, j, (Direction)dire) >= 5)
						return chess_array_[i][j];
				}
			}
		}
	}
	return EMPTY;
}


void Chess::show_chess()
{
	int i, j;
	printf(" %2s", " ");
	for (i = 0; i < Chess::SIZE; i++)
		printf(" %2d", i);
	printf("\n\n");
	for (i = 0; i < Chess::SIZE; i++)
	{
		printf(" %2d", i);
		for (j = 0; j < Chess::SIZE; j++)
		{
			if (get_point(i, j) == Chess::EMPTY)
				printf(" %2s", "*");
			else if (get_point(i, j) == Chess::WHITE)
				printf(" %2s", "W");
			else if (get_point(i, j) == Chess::BLACK)
				printf(" %2s", "B");
		}
		printf("\n\n");
	}
}


void Chess::set_man_picetype(Chess::PieceType value)
{
	picetype_ = value;
}


Chess::PieceType Chess::get_man_piecetype()
{
	return picetype_;
}


Chess::PieceType Chess::get_computer_piecetype()
{

	if (picetype_ == BLACK)
		return WHITE;
	else
		return BLACK;
}

int Chess::two_end(int x, int y, Direction dire)
{
	const int i = x;
	const int j = y;
	int k = i;
	int v = j;
	int temp = 0;
	switch (dire)
	{
	case COL:
		if (chess_array_[i][j] != EMPTY)
		{
			for (k = i; k >= 0; k--)
			{
				if (chess_array_[k][j] == EMPTY)
				{
					temp++;
					break;
				}
			}
			for (k = i; k<SIZE; k++)
			{
				if (chess_array_[k][j] == EMPTY)
				{
					temp++;
					break;
				}
			}
			return temp;
		}
		else
			return 0;
		break;
	case ROW:
		if (chess_array_[i][j] != EMPTY)
		{
			for (v = j; v >= 0; v--)
			{
				if (chess_array_[i][v] == EMPTY)
				{
					temp++;
					break;
				}
			}
			for (v = j; v<SIZE; v++)
			{
				if (chess_array_[i][v] == EMPTY)
				{
					temp++;
					break;
				}
			}
			return temp;
		}
		else
			return 0;
		break;
	case LEFT_RIGHT:
		if (chess_array_[i][j] != EMPTY)
		{
			for (k = i, v = j; k >= 0 && v >= 0; k--, v--)
			{
				if (chess_array_[k][v] == EMPTY)
				{
					temp++;
					break;
				}
			}
			for (k = i, v = j; k<SIZE&&v<SIZE; k++, v++)
			{
				if (chess_array_[k][v] == EMPTY)
				{
					temp++;
					break;
				}
			}
			return temp;
		}
		else
			return 0;
		break;
	case RIGHT_LEFT:
		if (chess_array_[i][j] != EMPTY)
		{
			for (k = i, v = j; k >= 0 && v<SIZE; k--, v++)
			{
				if (chess_array_[k][v] == EMPTY)
				{
					temp++;
					break;
				}
			}
			for (k = i, v = j; k<SIZE&&v >= 0; k++, v--)
			{
				if (chess_array_[k][v] == EMPTY)
				{
					temp++;
					break;
				}
				return temp;
			}
		}
		else
			return 0;
		break;
	}
}

bool Chess::judge_win_ex(PieceType piece)
{
	PieceType self_type = piece;
	PieceType oposite_type;
	if (piece == WHITE)
		oposite_type = BLACK;
	else
		oposite_type = WHITE;
	if (self_type == judge_win())
		return 1;
	else if (oposite_type == judge_win())
		return 0;
	else
	{
		int i = 0;
		int j = 0;
		int temp1 = 0;
		int temp2 = 0;
		int temp_ = (int)COL;
		for (; i<SIZE; i++)
		{
			for (; j<SIZE; j++)
			{
				if (chess_array_[i][j] == oposite_type)
				{
					for (temp_ = (int)COL; temp_ <= RIGHT_LEFT; temp_++)
					{
						temp1 = judge(i, j, (Direction)temp_);
						temp2 = two_end(i, j, (Direction)temp_);
						if (temp1 >= 4 && temp2 >= 1)
							return 0;
					}
				}//检查黑子是否一步胜
				if (chess_array_[i][j] == self_type)
				{
					for (temp_ = (int)COL; temp_ <= RIGHT_LEFT; temp_++)
					{
						temp1 = judge(i, j, (Direction)temp_);
						temp2 = two_end(i, j, (Direction)temp_);
						if (temp1 >= 4 && temp2 >= 1)
							return 1;
					}
				}//检查白子是否一步胜
			}
		}
		for (i = 0; i<SIZE; i++)
		{
			for (j = 0; j<SIZE; j++)
			{
				if (chess_array_[i][j] == oposite_type)
				{
					for (temp_ = (int)COL; temp_ <= RIGHT_LEFT; temp_++)
					{
						temp1 = judge(i, j, (Direction)temp_);
						temp2 = two_end(i, j, (Direction)temp_);
						if (temp1 >= 3 && temp2 >= 2)
							return 0;
					}//检查黑棋活3数是否大于等于1
				}
				if (chess_array_[i][j] == self_type)
				{
					for (temp_ = (int)COL; temp_ <= RIGHT_LEFT; temp_++)
					{
						int sum = 0;
						temp1 = judge(i, j, (Direction)temp_);
						temp2 = two_end(i, j, (Direction)temp_);
						if (temp1 >= 3 && temp2 >= 2)
						{
							sum++;
							if (sum >= 2)
								return 1;
						}
					}
				}//检查白子活3数是否大于等于2
			}
		}
		return 0;
	}
}
