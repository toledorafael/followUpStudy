#ifndef MOVE_H
#define MOVE_H


// Move object class
class Move {
  private:
    int start;       // Starting square
    int dest;        // Ending square
    char promotion;  // Promotion piece: Q, K, N, B, or 0 for no promotion

  public:
    // Constructors
    Move(int start, int dest);
    Move(int start, int dest, char promotion);

    // Destructor
    ~Move();

    // Getters
    int getStart() const;
    int getDest() const;
    int getPromotion() const;

    // Setter
    void setPromotion(char piece);

    // Operators
    friend bool operator==(const Move &move1, const Move &move2);
    friend bool operator!=(const Move &move1, const Move &move2);
    friend bool operator<(const Move &move1, const Move &move2);
};


#endif

