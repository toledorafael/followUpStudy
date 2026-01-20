#ifndef _PIECE_H_
#define _PIECE_H_
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Piece {
protected:
  bool side; // stores color. White is 0. Black is 1
  char code; // stores the code of what piece it is P,K,N,Q,B,R
  int value; // stores piece value, used for computer4 calculation
public:
  // Constructors and destructor
  Piece(bool side, char code, int value);
  virtual ~Piece();

  char getCode() const;
  bool getSide() const;
  int getValue() const;

  virtual void setMoved() = 0;
  virtual bool getMoved() const = 0;
  virtual void setEnPassant(bool b) = 0;
  virtual bool getEnPassant() const = 0;

  virtual Piece *clone() = 0;
};





class Rook : public Piece {
private:
  bool hasMoved;

public:
  Rook(bool side, bool hm);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

class King : public Piece {
private:
  bool hasMoved;

public:
  King(bool side, bool hm);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

class Pawn : public Piece {
private:
  bool hasMoved;
  bool enPassant;

public:
  Pawn(bool side, bool hm, bool ep);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

class Queen : public Piece {
public:
  Queen(bool side);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

class Knight : public Piece {
public:
  Knight(bool side);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

class Bishop : public Piece {
public:
  Bishop(bool side);
  void setMoved() override;
  bool getMoved() const override;
  void setEnPassant(bool b) override;
  bool getEnPassant() const override;

  virtual Piece *clone() override;
};

#endif

