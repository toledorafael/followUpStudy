#include "level1.h"
#include <cstdlib>
#include <chrono>

// Implementation for Level 1 computer player

// Constructor
Level1::Level1(Board *board, bool side) : Computer(board, side) {}

// Destructor
Level1::~Level1() {}

// Generates a move
// Level 1: Produces a random legal move
Move Level1::generateMove() const {

  std::vector<int> pieces = board->getPieces(side);

  // Find pieces that have legal moves
  std::vector<int> canMove;
  for (size_t i = 0; i < pieces.size(); i++) {
    if (!board->legalMoves(pieces.at(i)).empty()) {
      canMove.push_back(pieces.at(i));
    }
  }

  
  // Get random piece, random move for that piece, and random promotion value
  // Seed based on current time in milliseconds (seconds is too slow)
  srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
  
  int rand1 = rand() % canMove.size();
  int randPiece = canMove.at(rand1);
  
  int rand2 = rand() % board->legalMoves(randPiece).size();
  Move randMove = board->legalMoves(randPiece).at(rand2);

  return randMove;
}


