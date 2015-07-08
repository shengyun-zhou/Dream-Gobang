#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "Chess.h"

class ComputerPlayer
{
private:
  Chess* chess_;
  Chess::PieceType computer_piece_type_;
  Chess::PieceType person_piece_type_;

  struct GridScore
  {
    int row;
    int col;
    double row_score;
    double col_score;
    double left_right_score;
    double right_left_score;
    double sum_score;
  };

  GridScore computer_score_table_[Chess::SIZE][Chess::SIZE];
  GridScore computer_score_[Chess::SIZE * Chess::SIZE];
  GridScore person_score_table_[Chess::SIZE][Chess::SIZE];
  GridScore person_score_[Chess::SIZE * Chess::SIZE];

  static const double empty_grid_discount_weight_[10];
  static const double linked_grid_score_[10];
  static const double linked_grid_weight_[10];

  bool is_direction_valid(int row, int col, Chess::Direction dire, Chess::PieceType oposite_type);
  void calc_grid_score_direction(int row, int col, Chess::Direction dire, Chess::PieceType self_type, GridScore& grid_score);
  void calc_grid_score(GridScore& grid_score, Chess::PieceType seft_type);
  int quicksort_division(int left, int right, GridScore* array);
  void quicksort(int left, int right, GridScore* array);
  void attack(int& target_row, int& target_col);
  void defend(int& target_row, int& target_col);

  void show_score_tabel(GridScore score[][Chess::SIZE]);
public:
  ComputerPlayer(Chess::PieceType computer_type, Chess* c);
  void calc_next_step(int& target_row, int& target_col);
};

#endif // COMPUTERPLAYER_H
