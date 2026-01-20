#ifndef LEVEL1_H
#define LEVEL1_H

#include "computer.h"

// Level 1 computer player
// Makes random legal moves
class Level1 : public Computer {

  public:
    // Constructor
    Level1(Board *board, bool side);

    // Destructor
    ~Level1();

    // Generates a level 1 move
    virtual Move generateMove() const override;
};


#endif

