#include"Chess.h"
class Save{
public:
	void Record(Chess &chess_);
	enum object{PLAYER,COMPUTER};
	void set_first_object(object object_);
	Chess::PieceType get_next_piece_color();
	object get_next_object();
private:
	Chess::PieceType next_piece_color;
	object next_object;
	object first;
};