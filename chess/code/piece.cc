#include <vector>
#include "piece.h"
#include <iostream>
#include <utility>

// Piece

// Constructor
Piece::Piece(bool side, char code, int value) : side{side}, code{code}, value{value} {}
// Destructor
Piece::~Piece() {}

char Piece::getCode() const {
  return code;
}

bool Piece::getSide() const {
  return side;
}

int Piece::getValue() const {
  return value;
}






// Rook
Rook::Rook(bool side, bool hasMoved) : Piece(side, 'R', 5), hasMoved{hasMoved} {}

void Rook::setMoved() {
  hasMoved = true;
}

bool Rook::getMoved() const {
  return hasMoved;
}

void Rook::setEnPassant(bool b) {}
bool Rook::getEnPassant() const {}

Piece *Rook::clone() {
  return new Rook(side, hasMoved);
}




// King
King::King(bool side, bool hasMoved) : Piece(side, 'K', 0), hasMoved{hasMoved} {}

void King::setMoved() {
  hasMoved = true;
}

bool King::getMoved() const {
  return hasMoved;
}

void King::setEnPassant(bool b) {}
bool King::getEnPassant() const {}

Piece *King::clone() {
  return new King(side, hasMoved);
}





// Pawn
Pawn::Pawn(bool side, bool hasMoved, bool enPassant) : Piece(side, 'P', 1), hasMoved{hasMoved}, enPassant{enPassant} {}

void Pawn::setMoved() {
  hasMoved = true;
}

bool Pawn::getMoved() const {
  return hasMoved;
}

void Pawn::setEnPassant(bool b) {
  enPassant = b;
}

bool Pawn::getEnPassant() const {
  return enPassant;
}

Piece *Pawn::clone() {
  return new Pawn(side, hasMoved, enPassant);
}





// Queen
Queen::Queen(bool side) : Piece(side, 'Q', 9) {}
void Queen::setMoved(){}
bool Queen::getMoved() const {}
void Queen::setEnPassant(bool b) {}
bool Queen::getEnPassant() const {}

Piece *Queen::clone() {
  return new Queen(side);
}






// Knight
Knight::Knight(bool side) : Piece(side, 'N', 3) {}

void Knight::setMoved() {}
bool Knight::getMoved() const {}
void Knight::setEnPassant(bool b) {}
bool Knight::getEnPassant() const {}

Piece *Knight::clone() {
  return new Knight(side);
}





// Bishop
Bishop::Bishop(bool side) : Piece(side, 'B', 3) {}

void Bishop::setMoved() {}
bool Bishop::getMoved() const {}
void Bishop::setEnPassant(bool b) {}
bool Bishop::getEnPassant() const {}

Piece *Bishop::clone() {
  return new Bishop(side);
}


