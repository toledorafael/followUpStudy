#include "move.h"

// Implementation for move object class

// Constructors
Move::Move(int start, int dest) : start{start}, dest{dest}, promotion{'0'} {}
Move::Move(int start, int dest, char promotion) : start{start}, dest{dest}, promotion{promotion} {}

// Destructor
Move::~Move() {}

// Getters
int Move::getStart() const {
  return start;
}

int Move::getDest() const {
  return dest;
}

int Move::getPromotion() const {
  return promotion;
}

// Setter
void Move::setPromotion(char piece) {
  promotion = piece;
}


// Operators
bool operator==(const Move &move1, const Move &move2) {
  return move1.start == move2.start && move1.dest == move2.dest && move1.promotion == move2.promotion;
}

bool operator!=(const Move &move1, const Move &move2) {
  return !(move1 == move2);
}

bool operator<(const Move &move1, const Move &move2) {
  return move1.start < move2.start;
}

