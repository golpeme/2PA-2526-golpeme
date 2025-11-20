
//Parchis 26/09/2025

#include "parcheesi.h"
#include <time.h>
#include <stdlib.h>

const int Parcheesi::safePoint[Parcheesi::kSafePoints] = {5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
const int Parcheesi::startPoint[Parcheesi::kMaxPlayers] = {5, 22, 39, 56};
const int Parcheesi::finishPoint[Parcheesi::kMaxPlayers] = {68, 17, 34, 51};

int RollDice(){
  return (rand() % 6) + 1;
}

Parcheesi::Parcheesi(int num_players)
  : num_players_{num_players}
  , current_turn_{0}
  , repeated_sixes_{0}
  , current_player_{kColor_Yellow}
  , player_{kColor_Yellow,kColor_Blue,kColor_Red,kColor_Green} {
}

int Parcheesi::CountPiecesOnBox(int box){
  int occupants = 0;
  for(int i = 0; i < num_players_; i++){
    occupants += player_[i].CountPiecesOnBox(box);
  }
  return occupants;
}

void Parcheesi::NextPlayer(){
  int num_player = static_cast<int>(current_player_);
  num_player = (num_player + 1) % num_players_;
  current_player_ = static_cast<Colors>(num_player);
}

void Parcheesi::PlayTurn(){
  int dice_num;
  dice_num = RollDice();
  for(int i = 0; i < Player::kPieceNum; i++){
    if(dice_num == 5 && player_[current_player_].piece_[i].position == 0 &&
        CountPiecesOnBox(startPoint[current_player_]) < 2){
      player_[current_player_].piece_[i].position = startPoint[current_player_];
      NextPlayer();
      return;
    }
  }
}

MovementOutput Parcheesi::ApplyMovement(int piece_idx, int roll_dice){

  Player& turn_player = player_[current_player_];
  Piece& turn_piece = turn_player.piece_[piece_idx];

  if(turn_piece.position == 0){
    if(roll_dice != 5){
      return kMovementOutput_NoMove;
    }else{
      if(CountPiecesOnBox(startPoint[current_player_]) >= 2){
        return kMovementOutput_NoMove;
      } else {
        turn_piece.position = startPoint[current_player_];
      }
    }
  }

  if (roll_dice == 6)
  {
    Parcheesi::repeated_sixes_++;
    if (repeated_sixes_ == 3)
    {
      turn_piece.position = 0;
      return kMovementOutput_IllegalMove; 
    }
  }
  

  //check bridges
  for (int i = 1; i <= roll_dice; i++)
  {
    if(CountPiecesOnBox(turn_piece.position + i) >= 2) {
      return kMovementOutput_NoMove;
    }
  }
  
  //check if enters final runway
  if(turn_piece.position + roll_dice == finishPoint[current_player_]){
    return kMovementOutput_End;
  }

  if(turn_piece.position + roll_dice > kBoxNum){
    int full_cycle = turn_piece.position - kBoxNum;
    turn_piece.position = full_cycle;
  }
  
  bool is_safe = false;
  for (int i = 0; i < kSafePoints; i++)
  {
    if(safePoint[i] == turn_piece.position) is_safe = true;
  }
  
  bool did_eat = false;
  if(!is_safe){
    if(CountPiecesOnBox(turn_piece.position + roll_dice) == 1){
      for(int i = 0; i < num_players_; i++){
        if(i == static_cast<int>(current_player_)) continue;

        if(player_[i].CountPiecesOnBox(turn_piece.position + roll_dice) == 1){
          for (int j = 0; j < Player::kPieceNum; j++)
          {
            if (player_[i].piece_[j].position == turn_piece.position + roll_dice)
            {
              player_[i].piece_[j].position = 0;
              did_eat = true;
            }
          }
        }
      }
    }
  }
  turn_piece.position += roll_dice;
  
  if (Parcheesi::repeated_sixes_ < 3 && Parcheesi::repeated_sixes_ > 0)
  {
    return kMovementOutput_RollAgain;
  }else{
    Parcheesi::repeated_sixes_ = 0;
  }
  
  if (did_eat)
  {
    return kMovementOutput_Eat;
  }

  return kMovementOutput_BasicMove;
}