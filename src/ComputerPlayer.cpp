#include "ComputerPlayer.h"
#include <string.h>
#include <stdio.h>

const double ComputerPlayer::linked_grid_score_[] = {0.0, 1.0, 5.0, 21.0, 85.0, 341.0, 1365.0, 5461.0, 21845.0, 87381.0};
const double ComputerPlayer::empty_grid_discount_weight_[] = {1.0, 0.8, 0.512, 0.21, 0.044, 0.0016};
const double ComputerPlayer::linked_grid_weight_[] = {0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
int ComputerPlayer::max_search_step = 4;
const double ComputerPlayer::INFINITY_MAX = 10000000.0;

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
  bool link_grid_invalid_flag = false;
  bool empty_grid_flag = false;

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
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
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
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      if(link_grid_invalid_flag == false)
        grid_score.row_score = linked_grid_score_[result] * discount_weight + extra_score;
      else
        grid_score.row_score = linked_grid_score_[result];
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
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
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
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row_pos, col) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      if(link_grid_invalid_flag == false)
        grid_score.col_score = linked_grid_score_[result] * discount_weight + extra_score;
      else
        grid_score.col_score = linked_grid_score_[result];
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
        else if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row_pos, col_pos) == self_type)
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
        else if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      if(link_grid_invalid_flag == false)
        grid_score.left_right_score = linked_grid_score_[result] * discount_weight + extra_score;
      else
        grid_score.left_right_score = linked_grid_score_[result];
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
        else if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row_pos, col_pos) == self_type)
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
        else if(chess_ -> get_point(row_pos, col_pos) == oposite_type)
        {
          if(empty_grid_num == 0 && max_linked_piece_num < 5 && link_grid_invalid_flag == false)
          {
            link_grid_invalid_flag = true;
            extra_score = 0.0;
            result--;
          }
          break;
        }
        else if(chess_ -> get_point(row_pos, col_pos) == self_type)
        {
          result++;
          discount_weight *= empty_grid_discount_weight_[empty_grid_num];
          empty_grid_num = 0;
        }
      }
      if(link_grid_invalid_flag == false)
        grid_score.right_left_score = linked_grid_score_[result] * discount_weight + extra_score;
      else
        grid_score.right_left_score = linked_grid_score_[result];
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
  double max_computer_score = computer_score_[0].sum_score;
  double max_person_score = 0;
  printf("computer_max_score:%.0lf\n", max_computer_score);
  int i;
  for(i = 0; i < Chess::SIZE * Chess::SIZE && computer_score_[i].sum_score == max_computer_score; i++)
  {
    if(chess_ -> get_point(computer_score_[i].row, computer_score_[i].col) != Chess::EMPTY)
      continue;
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
  double max_computer_score = 0;
  double max_person_score = person_score_[0].sum_score;
  printf("person_max_score:%.0lf\n", max_person_score);
  int i;
  for(i = 0; i < Chess::SIZE * Chess::SIZE && person_score_[i].sum_score == max_person_score; i++)
  {
    if(chess_ -> get_point(person_score_[i].row, person_score_[i].col) != Chess::EMPTY)
      continue;
    if(computer_score_table_[person_score_[i].row][person_score_[i].col].sum_score > max_computer_score)
    {
      max_computer_score = computer_score_table_[person_score_[i].row][person_score_[i].col].sum_score;
      target_row = person_score_[i].row;
      target_col = person_score_[i].col;
    }
  }
}

double abs(double a, double b)
{
  if(a - b < 0.0)
    return b - a;
  return a - b;
}

void ComputerPlayer::calc_score_table()
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

  //printf("Computer Score:\n");
  //show_score_tabel(computer_score_table_);
  //printf("Person Score:\n");
  //show_score_tabel(person_score_table_);

  quicksort(0, Chess::SIZE * Chess::SIZE - 1, computer_score_);
  quicksort(0, Chess::SIZE * Chess::SIZE - 1, person_score_);
}

double ComputerPlayer::calc_chess_score(Chess::PieceType self_type, int min_max_flag)
{
  Chess::PieceType oposite_type;
  if(self_type == Chess::BLACK)
    oposite_type = Chess::WHITE;
  else
    oposite_type = Chess::BLACK;
  double self_score = 0.0;
  GridScore self_grid_score_temp;
  double oposite_score = 0.0;
  GridScore oposite_grid_score_temp;
  int i, j;
  for(i = 0; i < Chess::SIZE; i++)
  {
    for(j = 0; j < Chess::SIZE; j++)
    {
      if(chess_->get_point(i, j) == self_type)
      {
        self_grid_score_temp.row = i;
        self_grid_score_temp.col = j;
        calc_grid_score(self_grid_score_temp, self_type);
        if(self_grid_score_temp.sum_score > self_score)
          self_score = self_grid_score_temp.sum_score;
        chess_->set_point(i, j, self_type);
      }
      else if(chess_->get_point(i, j) == oposite_type)
      {
        oposite_grid_score_temp.row = i;
        oposite_grid_score_temp.col = j;
        calc_grid_score(oposite_grid_score_temp, oposite_type);
        if(oposite_grid_score_temp.sum_score > oposite_score)
          oposite_score  = oposite_grid_score_temp.sum_score;
        chess_->set_point(i, j, oposite_type);
      }
    }
  }
  self_score *= min_max_flag;
  oposite_score *= min_max_flag;
  return self_score - oposite_score;
}

double ComputerPlayer::DFS(int nth_step, double ex_value, int min_max_flag, int& target_row, int& target_col)
{
  calc_score_table();
  int c, i = 0, j = 0;
  double current_ex_value;
  double next_step_value;
  double temp_value;
  if(min_max_flag == MAX_FLAG)
    current_ex_value = -1.0 * INFINITY_MAX - 1.0;
  else
    current_ex_value = INFINITY_MAX + 1.0;
  for(c = 0; c < max_select_grid; c++)
  {
    while(chess_->get_point(computer_score_[i].row, computer_score_[i].col) != Chess::EMPTY)
    {
      i++;
      if(i >= Chess::SIZE * Chess::SIZE)
        return current_ex_value;
    }
    while(chess_->get_point(person_score_[j].row, person_score_[j].col) != Chess::EMPTY)
    {
      j++;
      if(j >= Chess::SIZE * Chess::SIZE)
        return current_ex_value;
    }

    if(computer_score_[i].sum_score > person_score_[j].sum_score)     //attack
    {
      chess_->set_point(computer_score_[i].row, computer_score_[i].col, computer_piece_type_);
      if(chess_->judge_win_ex(computer_piece_type_))
      {
        if(nth_step == 1)
        {
          target_row = computer_score_[i].row;
          target_col = computer_score_[i].col;
        }
        chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
        return INFINITY_MAX * min_max_flag;
      }

      if(nth_step == max_search_step)
      {
        if(min_max_flag == MAX_FLAG)
        {
          temp_value = calc_chess_score(computer_piece_type_, min_max_flag);
          if(temp_value > current_ex_value)
          {
            current_ex_value = temp_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = computer_score_[i].row;
              target_col = computer_score_[i].col;
            }
          }
          if(current_ex_value > ex_value)      //当前最大值大于上一层已知的最小值
          {
            chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
            return current_ex_value;
          }
        }
        else
        {
          temp_value = calc_chess_score(computer_piece_type_, min_max_flag);
          if(temp_value < current_ex_value)
          {
            current_ex_value = temp_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = computer_score_[i].row;
              target_col = computer_score_[i].col;
            }
          }
          if(current_ex_value < ex_value)      //当前最小值小于上一层已知的最大值
          {
            chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
            return current_ex_value;
          }
        }
      }
      else
      {
        ComputerPlayer next_com(person_piece_type_, chess_);
        next_step_value = next_com.DFS(nth_step + 1, current_ex_value, min_max_flag * -1, target_row, target_col);
        if(min_max_flag == MAX_FLAG)
        {
          if(next_step_value > current_ex_value)
          {
            current_ex_value = next_step_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = computer_score_[i].row;
              target_col = computer_score_[i].col;
            }
            if(current_ex_value > ex_value)      //当前最大值大于上一层已知的最小值
            {
              chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
              return current_ex_value;
            }
          }
        }
        else
        {
          if(next_step_value < current_ex_value)
          {
            current_ex_value = next_step_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = computer_score_[i].row;
              target_col = computer_score_[i].col;
            }
            if(current_ex_value < ex_value)      //当前最小值小于上一层已知的最大值
            {
              chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
              return current_ex_value;
            }
          }
        }
      }
      chess_->set_point(computer_score_[i].row, computer_score_[i].col, Chess::EMPTY);
      i++;
    }
    else                                                              //defend
    {
      //printf("Current pos:(%d,%d)\n", person_score_[j].row, person_score_[j].col);
      //getchar();
      chess_->set_point(person_score_[j].row, person_score_[j].col, computer_piece_type_);
			if (chess_->judge_win_ex(computer_piece_type_))
      {
        if(nth_step == 1)
        {
          target_row = person_score_[j].row;
          target_col = person_score_[j].col;
        }
        chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
        return INFINITY_MAX * min_max_flag;
      }

      if(nth_step == max_search_step)
      {
        if(min_max_flag == MAX_FLAG)
        {
          temp_value = calc_chess_score(computer_piece_type_, min_max_flag);
          if(temp_value > current_ex_value)
          {
            current_ex_value = temp_value = calc_chess_score(computer_piece_type_, min_max_flag);;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = person_score_[j].row;
              target_col = person_score_[j].col;
            }
          }
          if(current_ex_value > ex_value)      //当前最大值大于上一层已知的最小值
          {
            chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
            return current_ex_value;
          }
        }
        else
        {
          temp_value = calc_chess_score(computer_piece_type_, min_max_flag);
          if(temp_value < current_ex_value)
          {
            current_ex_value = temp_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = person_score_[j].row;
              target_col = person_score_[j].col;
            }
          }
          if(current_ex_value < ex_value)      //当前最小值小于上一层已知的最大值
          {
            chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
            return current_ex_value;
          }
        }
      }
      else
      {
        ComputerPlayer next_com(person_piece_type_, chess_);
        next_step_value = next_com.DFS(nth_step + 1, current_ex_value, min_max_flag * -1, target_row, target_col);
        if(min_max_flag == MAX_FLAG)
        {
          if(next_step_value > current_ex_value)
          {
            current_ex_value = next_step_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = person_score_[j].row;
              target_col = person_score_[j].col;
            }
            if(current_ex_value > ex_value)      //当前最大值大于上一层已知的最小值
            {
              chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
              return current_ex_value;
            }
          }
        }
        else
        {
          if(next_step_value < current_ex_value)
          {
            current_ex_value = next_step_value;
            //printf("step:%d,current_ex_value:%.2lf\n",nth_step, current_ex_value);
            if(nth_step == 1)
            {
              target_row = person_score_[j].row;
              target_col = person_score_[j].col;
            }
            if(current_ex_value < ex_value)      //当前最小值小于上一层已知的最大值
            {
              chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
              return current_ex_value;
            }
          }
        }
      }
      chess_->set_point(person_score_[j].row, person_score_[j].col, Chess::EMPTY);
      j++;
    }
  }
  return current_ex_value;
}

void ComputerPlayer::calc_next_step(int& target_row, int& target_col)
{
  if(chess_->get_empty_grid_num() == Chess::SIZE * Chess::SIZE)
  {
    target_row = Chess::SIZE / 2;
    target_col = Chess::SIZE / 2;
    return;
  }
  /*memset(computer_score_, 0, sizeof(computer_score_));
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

  if(computer_score_[0].sum_score > person_score_[0].sum_score || abs(computer_score_[0].sum_score, person_score_[0].sum_score) < 0.1)
    attack(target_row, target_col);
  else
    defend(target_row, target_col);*/
  if(chess_->get_empty_grid_num() < max_search_step)
  {
    max_search_step = max_search_step / 2 * 2;
    if(max_search_step < 1)
      max_search_step = 1;
  }
  DFS(1, INFINITY_MAX, MAX_FLAG, target_row, target_col);
}

void ComputerPlayer::show_score_tabel(GridScore score[][Chess::SIZE])
{
  int i, j;
  for(i = 0; i < Chess::SIZE; i++)
  {
    for(j = 0; j < Chess::SIZE; j++)
    {
      printf("%4.0f ", score[i][j].sum_score);
    }
    printf("\n");
  }
}
