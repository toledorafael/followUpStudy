#ifndef _BOARD_H_
#define _BOARD_H_
#include "piece.h"
#include "move.h"
#include <iostream>
#include <vector>

class Board {
  Piece *arr[64];
  int whiteKing = -1;
  int blackKing = -1;

  // Piece position vectors
  vector<int> whitePieces;
  vector<int> blackPieces;

  // pawnMoves: Returns possible pawn moves
  vector<int> pawnMoves(int coord, bool side);

  // kingMoves: Returns possible king moves
  vector<int> kingMoves(int coord, bool side);

  // knightMoves: Returns possible knight moves
  vector<int> knightMoves(int coord, bool side);

  // legalSlidingMoves: Returns all legal moves for slidingpieces (rook, queen, bishop)
  std::vector<int> legalSlidingMoves(int coord, char p, bool side);

  // distanceToEdge: Helper function for legalSlidingMoves,
  // Returns distance to each edge of the board {left, right, up down} used by rooks and queens
  std::vector<int> distanceToEdge(int coord);

  // helper function for legalSlidingMoves,
  //   returns distance on each diagonal to the edges of board
  //   {upleft, upright, downright, downleft} used by bishops and queens
  std::vector<int> bishopDist(int coord);

  // removeFromVector: Remove coord from piece position vector
  void removeFromVector(int coord);

  // promotionMove: Moves pawn and promotes it
  void promotionMove(Move move);

  // isPinned: Checks if a piece is pinned by temporarily moving it and
  //   checking if this results in a self-check
  bool isPinned(int start, int dest);

  // Returns true if side has sufficient material to checkmate
  bool sufficientMaterial(bool side);

public:
  // Constructors and Destructor
  Board();
  ~Board();
  void swap(Board &other);
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);

  // at: getter for arr[]
  Piece *at(int coord) const;

  // getPieces: Get the white or black piece position vector
  // White is 1, black is 0
  vector<int> getPieces(bool side) const;

  // placePiece: Places piece at coord, overwrites anything at coord
  void placePiece(int coord, Piece *piece);

  // removePiece: Removes piece at coord
  bool removePiece(int coord);

  // movePiece: Moves piece
  void movePiece(Move move);

  // isPromotionMove: Returns true if a move's start and dest indicate a promotion move
  bool isPromotionMove(Move move);

  // isCheck: Checks if move results in a check on the opponent
  bool isCheck(Move move);

  // legalMoves: Gets all legal moves for the piece at coord
  // Wrapper function for all _Moves
  std::vector<Move> legalMoves(int coord);

  // noMoves: Checks if side has no legal moves in the current position
  bool noMoves(bool side);

  //kingExists: Checks if ther is a king of side color on the board
  bool kingExists(bool side) const;

  // isUnderAttackMove: returns true if coord is attacked by the side opposite move
  bool isUnderAttackMove(int coord, Move move);

  // isUnderAttack: returns the coord of the highest-value attacking piece if coord is attacked by !side, false otherwise
  int isUnderAttack(int coord, bool side);

  // isInCheck: Checks if side is in check in the current position
  bool isInCheck(bool side);

  // enPassant: enPassant checker, sets all enPassant flags for !side to be
  // false, called after each side's turn
  void enPassant(bool side);

  // Returns true if both sides have insufficient material to checkmate
  bool insufficientMaterial();

  // getBoard: Gets a copy of the board as characters
  vector<char> getBoard() const;

  // Output stream operator for board class
  friend std::ostream &operator<<(std::ostream &out, const Board &b);
};

#endif

