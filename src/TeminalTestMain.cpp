#include "Chess.h"
#include <stdio.h>
int main()
{
	Chess chess;
	int i;
	for (i = Chess::SIZE - 1; i >= 0; i--)
	{
		chess.set_point(0, i, Chess::BLACK);
		chess.set_point(1, i, Chess::BLACK);
	}
	for (i = 0; i < Chess::SIZE; i++)
	{
		printf("Num of Max Linked Chess(%d,%d):%d(Row)\n", 0, i, chess.judge(0, i, Chess::ROW));
		printf("Num of Max Linked Chess(%d,%d):%d(Col)\n", 0, i, chess.judge(0, i, Chess::COL));
		printf("Num of Max Linked Chess(%d,%d):%d(Left-Right)\n", 0, i, chess.judge(0, i, Chess::LEFT_RIGHT));
		printf("Num of Max Linked Chess(%d,%d):%d(Right-Left)\n", 0, i, chess.judge(0, i, Chess::RIGHT_LEFT));
		printf("\n");
	}
	return 0;
}
