#include "ComputerPlayer.h"
#include <string.h>
#include <stdio.h>

const double ComputerPlayer::linked_grid_score_[] = {0.0, 1.0, 5.0, 21.0, 85.0, 341.0, 1365.0, 5461.0, 21845.0, 87381.0};
const double ComputerPlayer::empty_grid_discount_weight_[] = {1.0, 0.8, 0.512, 0.21, 0.044, 0.0016};
const double ComputerPlayer::linked_grid_weight_[] = {0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

ComputerPlayer::ComputerPlayer(Chess::PieceType computer_type, Chess* c)
{
  computer_piece_type_ = computer_type;
  if(computer_piece_type_ == Chess::BLACK)
    person_piece_type_ = Chess::WHITE;
  else if(computer_piece_type_ == Chess::WHITE)
    person_piece_type_ = Chess::BLACK;
  chess_ = c;
}

bool ComputerPlayer::is_direction_valid(int row, int col, Chess::Direction dire, Chess::PieceType oposite_type)
{
  int k1, k2;
  int row_pos, col_pos;
  int row_max, col_max, row_min, col_min;
  row_max = (row + 4 < Chess::SIZE) ? row + 4 : Chess::SIZE - 1;
  row_min = (row - 4 >= 0) ? row - 4 : 0;
  col_max = (col + 4 < Chess::SIZE) ? col + 4 : Chess::SIZE - 1;
  col_min = (col - 4 >= 0) ? col - 4 : 0;
  switch(dire)
  {
    case Chess::ROW:
      for(col_pos = col, k1 = 0; col_pos <= col_max; col_pos++, k1++)
      {
        if(chess_ -> get_point(row, col_pos) == oposite_type)
          break;
      }
      k1--;
      for(col_pos = col, k2 = 0; col_pos >= col_min; col_pos--, k2--)
      {
        if(chess_ -> get_point(row, col_pos) == oposite_type)
          break;
      }
      k2++;
      break;
    case Chess::COL:
      for(row_pos = row, k1 = 0; row_pos <= row_max; row_pos++, k1++)
      {
        if(chess_ -> get_point(row_pos, col) == oposite_type)
          break;
      }
      k1--;
      for(row_pos = row, k2 = 0; row_pos >= row_min; row_pos--, k2--)
      {
        if(chess_ -> get_point(row_pos, col) == oposite_type)
          break;
      }
      k2++;
      break;
    case Chess::LEFT_RIGHT:
      for(row_pos = row, col_pos = col, k1 = 0; row_pos <= row_max && col_pos <= col_max; row_pos++, col_pos++, k1++)
      {
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
      }
      k1--;
      for(row_pos = row, col_pos = col, k2 = 0; row_pos >= row_min && col_pos >= col_min; row_pos--, col_pos--, k2--)
      {
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
      }
      k2++;
      break;
    case Chess::RIGHT_LEFT:
      for(row_pos = row, col_pos = col, k1 = 0; row_pos <= row_max && col_pos >= col_min; row_pos++, col_pos--, k1++)
      {
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
      }
      k1--;
      for(row_pos = row, col_pos = col, k2 = 0; row_pos >= row_min && col_pos <= col_max; row_pos--, col_pos++, k2--)
      {
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
      }
      k2++;
      break;
  }
  if(k1 - k2 + 1 < 5)
    return false;
  return true;
}

void ComputerPlayer::calc_grid_score_direction(int row, int col, Chess::Direction dire,
                                               Chess::PieceType self_type, GridScore& grid_score)
{
  int row_pos, col_pos;
  int row_max, col_max, row_min, col_min;
  int result = 1;
  int empty_grid_num = 0;
  double discount_weight = 1.0;
  double extra_score = 0;
  int max_linked_piece_num;

  row_max = (row + 4 < Chess::SIZE) ? row + 4 : Chess::SIZE - 1;
  row_min = (row - 4 >= 0) ? row - 4 : 0;
  col_max = (col + 4 < Chess::SIZE) ? col + 4 : Chess::SIZE - 1;
  col_min = (col - 4 >= 0) ? col - 4 : 0;

  chess_ -> set_point(row, col, self_type);
  max_linked_piece_num = chess_ -> judge(row, col, dire);
  extra_score = linked_grid_score_[max_linked_piece_num] * linked_grid_weight_[max_linked_piece_num];
  chess_ -> set_point(row, col, Chess::EMPTY);

  Chess::PieceType oposite_type;
  if(self_type == Chess::BLACK)
    oposite_type = Chess::WHITE;
  else if(self_type == Chess::WHITE)
    oposite_type = Chess::BLACK;

  switch(dire)
  {
    case Chess::ROW:
      grid_score.row_score = 0;
      if(is_direction_valid(row, col, dire, oposite_type) == false)
      {
        grid_score.row_score = 0;
        return;
      }
      for(col_pos = col + 1; col_pos <= col_max; col_pos++)
      {
        if(chess_ -> get_point(row, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        else if(chess_ -> get_point(row, col_pos) == oposite_type)
          break;
        else if(chess_ -> get_point(row, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      empty_grid_num = 0;
      for(col_pos = col - 1; col_pos >= col_min; col_pos--)
      {
        if(chess_ -> get_point(row, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        else if(chess_ -> get_point(row, col_pos) == oposite_type)
          break;
        else if(chess_ -> get_point(row, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      grid_score.row_score = linked_grid_score_[result] * discount_weight + extra_score;
      return;
    case Chess::COL:
      grid_score.col_score = 0;
      if(is_direction_valid(row, col, dire, oposite_type) == false)
      {
        grid_score.col_score = 0;
        return;
      }
      for(row_pos = row + 1; row_pos <= row_max; row_pos++)
      {
        if(chess_ -> get_point(row_pos, col) == Chess::EMPTY)
          empty_grid_num++;
        else if(chess_ -> get_point(row_pos, col) == oposite_type)
          break;
        else if(chess_ -> get_point(row_pos, col) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      empty_grid_num = 0;
      for(row_pos = row - 1; row_pos >= row_min; row_pos--)
      {
        if(chess_ -> get_point(row_pos, col) == Chess::EMPTY)
          empty_grid_num++;
        else if(chess_ -> get_point(row_pos, col) == oposite_type)
          break;
        else if(chess_ -> get_point(row_pos, col) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      grid_score.col_score = linked_grid_score_[result] * discount_weight + extra_score;
      return;
    case Chess::LEFT_RIGHT:
      grid_score.left_right_score = 0;
      if(is_direction_valid(row, col, dire, oposite_type) == false)
      {
        grid_score.left_right_score = 0;
        return;
      }
      for(col_pos = col + 1, row_pos = row + 1; col_pos <= col_max && row_pos <= row_max; col_pos++, row_pos++)
      {
        if(chess_ -> get_point(row_pos, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
        if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      empty_grid_num = 0;
      for(col_pos = col - 1, row_pos = row - 1; col_pos >= col_min && row_pos >= row_min; col_pos--, row_pos--)
      {
        if(chess_ -> get_point(row_pos, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
        if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      grid_score.left_right_score = linked_grid_score_[result] * discount_weight + extra_score;
      return;
    case Chess::RIGHT_LEFT:
      grid_score.right_left_score = 0;
      if(is_direction_valid(row, col, dire, oposite_type) == false)
      {
        grid_score.right_left_score = 0;
        return;
      }
      for(col_pos = col + 1, row_pos = row - 1; col_pos <= col_max && row_pos >= row_min; col_pos++, row_pos--)
      {
        if(chess_ -> get_point(row_pos, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
        if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      empty_grid_num = 0;
      for(col_pos = col - 1, row_pos = row + 1; col_pos >= col_min && row_pos <= row_max; col_pos--, row_pos++)
      {
        if(chess_ -> get_point(row_pos, col_pos) == Chess::EMPTY)
          empty_grid_num++;
        if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
          break;
        if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      grid_score.right_left_score = linked_grid_score_[result] * discount_weight + extra_score;
      return;
  }
}

void ComputerPlayer::calc_grid_score(GridScore& grid_score, Chess::PieceType self_type)
{
  calc_grid_score_direction(grid_score.row, grid_score.col, Chess::ROW, self_type, grid_score);
  calc_grid_score_direction(grid_score.row, grid_score.col, Chess::COL, self_type, grid_score);
  calc_grid_score_direction(grid_score.row, grid_score.col, Chess::LEFT_RIGHT, self_type, grid_score);
  calc_grid_score_direction(grid_score.row, grid_score.col, Chess::RIGHT_LEFT, self_type, grid_score);
  grid_score.sum_score = grid_score.row_score + grid_score.col_score +
                         grid_score.left_right_score + grid_score.right_left_score;
}

int ComputerPlayer::quicksort_division(int left, int right, GridScore* array)
{
  //printf("left == %d, right == %d\n", left, right);
  GridScore base = array[left];
  while(left < right)
  {
    while(left < right && array[right].sum_score <= base.sum_score)
      right--;
    array[left] = array[right];
    while(left < right && array[left].sum_score >= base.sum_score)
      left++;
    array[right] = array[left];
  }
  array[left] = base;
  return left;
}

void ComputerPlayer::quicksort(int left, int right, GridScore* array)
{
  if(left < right)
  {
    int m = quicksort_division(left, right, array);
    quicksort(left, m - 1, array);
    quicksort(m + 1, right, array);
  }
}

void ComputerPlayer::attack(int& target_row, int& target_col)
{
  int max_computer_score = computer_score_[0].sum_score;
  int max_person_score = 0;
  int i;
  for(i = 0; i < Chess::SIZE * Chess::SIZE && computer_score_[i].sum_score == max_computer_score; i++)
  {
    if(person_score_table_[computer_score_[i].row][computer_score_[i].col].sum_score > max_person_score)
    {
      max_person_score = person_score_table_[computer_score_[i].row][computer_score_[i].col].sum_score;
      target_row = computer_score_[i].row;
      target_col = computer_score_[i].col;
    }
  }
}

void ComputerPlayer::defend(int& target_row, int& target_col)
{
  int max_computer_score = 0;
  int max_person_score = person_score_[0].sum_score;
  int i;
  for(i = 0; i < Chess::SIZE * Chess::SIZE && person_score_[i].sum_score == max_person_score; i++)
  {
    if(computer_score_table_[person_score_[i].row][person_score_[i].col].sum_score > max_computer_score)
    {
      max_computer_score = computer_score_table_[person_score_[i].row][person_score_[i].col].sum_score;
      target_row = person_score_[i].row;
      target_col = person_score_[i].col;
    }
  }
}

void ComputerPlayer::calc_next_step(int& target_row, int& target_col)
{
  memset(computer_score_, 0, sizeof(computer_score_));
  memset(person_score_, 0, sizeof(person_score_));
  int i, j, count = 0;
  for(i = 0; i < Chess::SIZE; i++)
  {
    for(j = 0; j < Chess::SIZE; j++)
    {
      if(chess_ -> get_point(i, j) != Chess::EMPTY)
        continue;
      computer_score_table_[i][j].row = i;
      computer_score_table_[i][j].col = j;
      person_score_table_[i][j].row = i;
      person_score_table_[i][j].col = j;
      calc_grid_score(computer_score_table_[i][j], computer_piece_type_);
      calc_grid_score(person_score_table_[i][j], person_piece_type_);
      computer_score_[count] = computer_score_table_[i][j];
      person_score_[count++] = person_score_table_[i][j];
    }
  }

  quicksort(0, Chess::SIZE * Chess::SIZE - 1, computer_score_);
  quicksort(0, Chess::SIZE * Chess::SIZE - 1, person_score_);

  if(computer_score_[0].sum_score >= person_score_[0].sum_score)
    attack(target_row, target_col);
  else
    defend(target_row, target_col);
}
