#include "Chess.h"
#include "ComputerPlayer.h"
#include <stdio.h>

void draw_chess(Chess& chess)
{
  int i,j;
  printf(" %2s", " ");
  for(i = 0; i < Chess::SIZE; i++)
    printf(" %2d", i);
  printf("\n\n");
  for(i = 0; i < Chess::SIZE; i++)
  {
    printf(" %2d", i);
    for(j = 0; j < Chess::SIZE; j++)
    {
      if(chess.get_point(i, j) == Chess::EMPTY)
        printf(" %2s", "*");
      else if(chess.get_point(i, j) == Chess::WHITE)
        printf(" %2s", "W");
      else if(chess.get_point(i, j) == Chess::BLACK)
        printf(" %2s", "B");
    }
    printf("\n\n");
  }
}

int main()
{
	Chess chess;
	chess.set_point(Chess::SIZE / 2, Chess::SIZE / 2, Chess::WHITE);
	chess.set_point(Chess::SIZE / 2, Chess::SIZE / 2 + 1, Chess::BLACK);
	chess.set_point(Chess::SIZE / 2 + 1, Chess::SIZE / 2, Chess::BLACK);
	chess.set_point(Chess::SIZE / 2 + 1, Chess::SIZE / 2 + 1, Chess::WHITE);
	draw_chess(chess);
	const Chess::PieceType player_type = Chess::BLACK;
	const Chess::PieceType computer_type = Chess::WHITE;
  ComputerPlayer computer(computer_type, &chess);
  int row, col;
	while(!chess.is_chess_full() && chess.judge_win() == Chess::EMPTY)
  {
    printf("The player holds Black piece.\n");
    printf("Input the position of the next step (row,col):");
    scanf("%d%d", &row, &col);
    if(chess.get_point(row, col) != Chess::EMPTY)
      continue;
    chess.set_point(row, col, player_type);
    draw_chess(chess);
    computer.calc_next_step(row, col);
    chess.set_point(row, col, computer_type);
    draw_chess(chess);
    printf("computer step:(%d,%d)\n",row, col);
  }
  if(chess.judge_win() == Chess::BLACK)
    printf("The player win!\n");
  else if(chess.judge_win() == Chess::WHITE)
    printf("The computer win!\n");
  else
    printf("Tie!\n");
  return 0;
}
