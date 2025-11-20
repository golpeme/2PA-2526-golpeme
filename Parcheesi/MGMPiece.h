#pragma once
#ifndef PIECE_H
#define PIECE_H

class Piece{
  private:
    int position_;
    int turn_;
	public:
		Piece() : position_{0}, turn_{0} {}
		void SetPosition(int pos){position_ = pos;};
		int GetPosition() const{return position_;};
		
		void SetTurn(int turn){turn_ = turn;};
		int GetTurn() const  {return turn_;};

		~Piece() = default;
};
#endif // PIECE_H