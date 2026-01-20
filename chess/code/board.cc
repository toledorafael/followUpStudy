#include "board.h"
#include <utility>

using namespace std;

// Constructor
Board::Board() : whiteKing{-1}, blackKing{-1} {
    for (int i = 0; i < 64; i++) {
        arr[i] = nullptr;
    }
}
// Destructor
Board::~Board() {
  for (int i = 0; i < 64; i++) {
    delete arr[i];
  }
}
// Swap function
void Board::swap(Board &other) {
  std::swap(arr, other.arr);
  std::swap(whiteKing, other.whiteKing);
  std::swap(blackKing, other.blackKing);
  std::swap(whitePieces, other.whitePieces);
  std::swap(blackPieces, other.blackPieces);
}
// Copy Constructor
Board::Board(const Board &other) : whiteKing{other.whiteKing}, blackKing{other.blackKing}, whitePieces{other.whitePieces}, blackPieces{other.blackPieces} {

  for (int i = 0; i < 64; i++) {
    if (other.arr[i] != nullptr) arr[i] = other.arr[i]->clone();
    else arr[i] = nullptr;
  }
}
// Move Constructor
Board::Board(Board &&other) : whiteKing{other.whiteKing}, blackKing{other.blackKing}, whitePieces{other.whitePieces}, blackPieces{other.blackPieces} {
  
  for (int i = 0; i < 64; i++) {
    arr[i] = other.arr[i];
    other.arr[i] = nullptr;
  }
}
// Copy Assignment
Board &Board::operator=(const Board &other) {
  Board temp{other};
  swap(temp);
  return *this;
}
// Move Assignment
Board &Board::operator=(Board &&other) {
  swap(other);
  return *this;
}


// at: getter for arr[]
Piece* Board::at(int coord) const{
    return arr[coord];
}

// getPieces: Get the white or black piece position vector
// White is 1, black is 0
vector<int> Board::getPieces(bool side) const {
  if (side) return whitePieces;
  else return blackPieces;
}

// removeFromVector: Remove coord from piece position vector
void Board::removeFromVector(int coord) {
  if (arr[coord]->getSide()) {
    for (size_t i = 0; i < whitePieces.size(); i++) {
      if (whitePieces.at(i) == coord) whitePieces.erase(whitePieces.begin() + i);
    }
  } else {
    for (size_t i = 0; i < blackPieces.size(); i++) {
      if (blackPieces.at(i) == coord) blackPieces.erase(blackPieces.begin() + i);
    }
  }
}

// placePiece: Places piece at coord, overwrites anything at coord
void Board::placePiece(int coord, Piece *piece) {
    arr[coord] = piece;
    if (piece->getCode() == 'K') {
        if (piece->getSide()) whiteKing = coord;
        else {
          blackKing = coord;
        }
    }
    // Add to piece position vector
    if (piece->getSide()) whitePieces.push_back(coord);
    else blackPieces.push_back(coord);
}

// removePiece: Removes piece at coord
bool Board::removePiece(int coord) {
  if (arr[coord] == nullptr) return false;
  if (arr[coord]->getCode() == 'K') {
    if (arr[coord]->getSide()) {
      whiteKing = -1;
    } else {
      blackKing = -1;
    }
  }
  removeFromVector(coord);
  
  delete arr[coord];
  arr[coord] = nullptr;
  return true;
}

// movePiece: Moves piece
void Board::movePiece(Move move) {
  int start = move.getStart();
  int dest = move.getDest();
  char promotion = move.getPromotion();

  if (promotion != '0') {
    promotionMove(move);
    return;
  }
  
    if (arr[start]->getCode() == 'P' && ((abs(dest - start) == 7 || abs(dest - start) == 9)) && arr[dest] == nullptr) {
        placePiece(dest, arr[start]);
        removeFromVector(start);
        arr[start] = nullptr;
      
        if (dest - start == 7 || dest - start == -9) {
            removePiece(start - 1);
            arr[start - 1] = nullptr;
        } else if (dest - start == -7 || dest - start == 9) {
            removePiece(start + 1);
            arr[start + 1] = nullptr;
        }
    } else if (arr[start]->getCode() == 'K' && abs(dest - start) >= 2 && dest < 8 && start < 8) {
        placePiece(dest, arr[start]);
        removeFromVector(start);
        arr[start] = nullptr;
        if (dest == 6) {
            placePiece(dest - 1, arr[7]);
            removeFromVector(7);
            arr[7] = nullptr;
            arr[dest - 1]->setMoved();
        } else {
            placePiece(dest + 1 , arr[0]);
            removeFromVector(0);
            arr[0] = nullptr;
            arr[dest + 1]->setMoved();
        }
    } else if (arr[start]->getCode() == 'K' && abs(dest - start) >= 2 && 56 < dest && start > 56) {
        placePiece(dest, arr[start]);
        removeFromVector(start);
        arr[start] = nullptr;
        if (dest == 62) {
            placePiece(dest - 1, arr[63]);
            removeFromVector(63);
            arr[63] = nullptr;
            arr[dest - 1]->setMoved();
        } else {
            placePiece(dest + 1, arr[56]);
            removeFromVector(56);
            arr[56] = nullptr;
            arr[dest + 1]->setMoved();
        }

    } else {
        if (arr[dest] != nullptr) {
            removePiece(dest);
        }
        placePiece(dest, arr[start]);
        removeFromVector(start);
        arr[start] = nullptr;
    }
  if (arr[dest]->getCode() == 'P' && abs(dest - start) == 16) {
    arr[dest]->setEnPassant(true);
  }
    arr[dest]->setMoved();
}


// isPromotionMove: Returns true if a move's dest indicate a promotion move
bool Board::isPromotionMove(Move move) {
  int start = move.getStart();
  int dest = move.getDest();
  
  if (arr[start]->getCode() == 'P' && (dest > 55 || dest < 8)) return true;
  return false;
}

// promotionMove: moves pawn from start to dest and promotes it
void Board::promotionMove(Move move) {
  int start = move.getStart();
  int dest = move.getDest();
  char promotion = move.getPromotion();
  bool side = arr[start]->getSide();

  if (isPromotionMove(move) && 
    (promotion == 'Q' || promotion == 'B' || promotion == 'R' || promotion == 'N')) {
    
    removePiece(start);

    if (arr[dest] != nullptr) {
      removePiece(dest);
    }
    
    if (promotion == 'Q') placePiece(dest, new Queen(side));
    else if (promotion == 'B') placePiece(dest, new Bishop(side));
    else if (promotion == 'R') placePiece(dest, new Rook(side, 0));
    else if (promotion == 'N') placePiece(dest, new Knight(side));
  }
}

// isCheck: Checks if move results in a check on the opponent
bool Board::isCheck(Move move) {

  int start = move.getStart();
  bool side = arr[start]->getSide();
  Board boardCopy = *this;
  boardCopy.movePiece(move);
  if (boardCopy.isInCheck(!side)) {
    return true;
  }
  return false;
  
}

// isPinned: Checks if a piece is pinned by temporarily moving it and
//   checking if this results in a self-check
bool Board::isPinned(int start, int dest) {
  
  bool side = arr[start]->getSide();
  Board boardCopy = *this;
  Move move{start, dest};
  boardCopy.movePiece(move);
  if (boardCopy.isInCheck(side)) {
    return false;
  }
  return true;
}

// legalMoves: Gets all legal moves for the piece at coord
// Wrapper function for all _Moves
vector<Move> Board::legalMoves(int coord) {
    vector<Move> ret;
    vector<int> moves;

    if (!arr[coord]) {
        ret = {};
        return ret;
    }
    
    if (arr[coord]->getCode() == 'P') moves = pawnMoves(coord, arr[coord]->getSide());
    else if (arr[coord]->getCode() == 'K') moves = kingMoves(coord, arr[coord]->getSide());
    else if (arr[coord]->getCode() == 'N') moves = knightMoves(coord, arr[coord]->getSide());
    else moves = legalSlidingMoves(coord, arr[coord]->getCode(), arr[coord]->getSide());

    for (size_t i = 0; i < moves.size(); i++) {
      
      // Add all possible promotions
      if (arr[coord]->getCode() == 'P' && (moves.at(i) < 8 || moves.at(i) > 55)) {
        Move move1{coord, moves.at(i), 'R'};
        ret.push_back(move1);
        Move move2{coord, moves.at(i), 'N'};
        ret.push_back(move2);
        Move move3{coord, moves.at(i), 'B'};
        ret.push_back(move3);
        Move move4{coord, moves.at(i), 'Q'};
        ret.push_back(move4);
        
      } else {
        Move move{coord, moves.at(i)};
        ret.push_back(move);
      }
    }

    return ret;
}

// noMoves: Checks if side has no legal moves in the current position
bool Board::noMoves(bool side) {
  
	for (size_t i = 0; i < getPieces(side).size(); i++) {
		if (legalMoves(getPieces(side).at(i)).size() > 0) {
			return false;
		}
	}
  return true;
}

// pawnMoves: Returns possible pawn moves
vector<int> Board::pawnMoves(int coord, bool side) {
    vector<int> ret;
    vector<int> dir;
  
    if (side) {
        dir = {8};
        if (arr[coord]->getMoved() == false) {
            dir.push_back(16);
        }
        if (coord % 8 != 0 && arr[coord + 7] && arr[coord + 7]->getSide() == !side) {
            if(isPinned(coord, coord + 7)) ret.push_back(coord + 7);
        }
        if (coord % 8 != 7 && arr[coord + 9] && arr[coord + 9]->getSide() == !side) {
            if(isPinned(coord, coord + 9)) ret.push_back(coord + 9);
        }
        if (coord % 8 != 0 && arr[coord - 1] && arr[coord - 1]->getSide() == !side && arr[coord - 1]->getCode() == 'P' && arr[coord - 1]->getEnPassant() == true) {
            if(isPinned(coord, coord + 7)) ret.push_back(coord + 7);
        }
        if (coord % 8 != 7 && arr[coord + 1] && arr[coord + 1]->getSide() == !side && arr[coord + 1]->getCode() == 'P' && arr[coord + 1]->getEnPassant() == true) {
          
           if(isPinned(coord, coord + 9)) ret.push_back(coord + 9);
        }

    } else {
        dir = {-8};
        if (arr[coord]->getMoved() == false) {
            dir.push_back(-16);
        }
        if (coord % 8 != 7 && arr[coord - 7] && arr[coord - 7]->getSide() == !side) {
            if(isPinned(coord, coord - 7)) {
              ret.push_back(coord - 7);
            }
        }
        if (coord % 8 != 0 && arr[coord - 9] && arr[coord - 9]->getSide() == !side) {
            if(isPinned(coord, coord - 9)) ret.push_back(coord - 9);
        }
        if (coord % 8 != 7 && arr[coord + 1] && arr[coord + 1]->getSide() == !side && arr[coord + 1]->getCode() == 'P' && arr[coord + 1]->getEnPassant() == true) {
          if(isPinned(coord, coord - 7)) ret.push_back(coord - 7);
        }
        if (coord % 8 != 0 && arr[coord - 1] && arr[coord - 1]->getSide() == !side && arr[coord - 1]->getCode() == 'P' && arr[coord - 1]->getEnPassant() == true) {
            if(isPinned(coord, coord - 9)) ret.push_back(coord - 9);
        }
    }
    for (size_t i = 0; i < dir.size(); i++) {
        if (coord + dir[i] > 63 || coord + dir[i] < 0) {
            continue;
        }
        if (arr[coord + dir[i]]) {
            break;
        }
        if(!isPinned(coord, coord + dir[i])) continue;
        ret.push_back(coord + dir[i]);
    }
    return ret;
}

// kingMoves: Returns possible king moves
vector<int> Board::kingMoves(int coord, bool side) {
    vector<int> dir{1, -1, 8, -8, 9, -9, 7, -7};
    vector<int> ret;

    if (coord % 8 == 0) {
        dir = {1, 8, -8, 9, -7};
    } else if (coord % 8 == 7) {
        dir = {-1, 8, -8, 7, -9};
    }

    for (size_t i = 0; i < dir.size(); i++) {
        if (coord + dir[i] > 63 || coord + dir[i] < 0) {
            continue;
        }
        if (!isPinned(coord, coord + dir[i])) continue;
        if (arr[coord + dir[i]]) {
            if (arr[coord + dir[i]]->getSide() == side) {
                continue;
            } else {
                ret.push_back(coord + dir[i]);
                continue;
            }
        }
        ret.push_back(coord + dir[i]);
    }

    if (side) {
        if (!arr[whiteKing]->getMoved() && whiteKing == 4) {
            if (isUnderAttack(whiteKing, side) != -1) {}
            else {
              for (int i = 5; i <= 6; i++) {

                if (i == 6 && arr[7] && arr[7]->getCode() == 'R' && arr[7]->getSide() == side && arr[7]->getMoved() == false) {
                  if (isUnderAttack(i, side) != -1 || arr[i]) break;
                    ret.push_back(coord + 2);
                }
                if (isUnderAttack(i, side) != -1 || arr[i]) {
                    break;
                }
            }
            for (int i = 3; i >= 2; i--) {
                if (i == 2 && arr[0] && arr[0]->getCode() == 'R' && arr[0]->getSide() == side && arr[0]->getMoved() == false) {
                  if (isUnderAttack(i, side) != -1 || arr[i]) break;
                    ret.push_back(coord - 2);
                }
                if (isUnderAttack(i, side) != -1 || arr[i]) break;
                              
            }
            }

            
        }
    } else {
      
        if (!arr[blackKing]->getMoved() && blackKing == 60) {
            if (isUnderAttack(blackKing, side) != -1) {}
            else {
              for (int i = 61; i <= 62; i++) {
                if (i == 62 && arr[63] && arr[63]->getCode() == 'R' && arr[63]->getSide() == side && arr[63]->getMoved() == false) {
                  if (isUnderAttack(i, side) != -1 || arr[i]) break;
                    ret.push_back(coord + 2);
                }
                if (isUnderAttack(i, side) != -1 || arr[i]) {
                    break;
                }
                
            }
            for (int i = 61; i >= 58; i--) {
                if (i == 58 && arr[56] && arr[56]->getCode() == 'R' && arr[56]->getSide() == side && arr[56]->getMoved() == false) {
                  if (isUnderAttack(i, side) != -1 || arr[i]) break;
                    ret.push_back(coord - 2);
                }
                if (isUnderAttack(i, side) != -1 || arr[i]) {
                    break;
                }
                
            }
            }
        }
    }
    return ret;
}

// knightMoves: Returns possible knight moves
vector<int> Board::knightMoves(int coord, bool side) {
    vector<int> dir{6, 15, 17, 10, -6, -15, -17, -10};
    vector<int> ret;

    if (coord % 8 == 0) {
        dir = {17, 10, -6, -15};
    } else if (coord % 8 == 1) {
        dir = {15, 17, 10, -6, -15, -17};
    } else if (coord % 8 == 6) {
        dir = {6, 15, 17, -15, -17, -10};
    } else if (coord % 8 == 7) {
        dir = {6, 15, -17, -10};
    }

    for (size_t i = 0; i < dir.size(); i++) {
        if (coord + dir[i] > 63 || coord + dir[i] < 0) {
            continue;
        }
        
        if (arr[coord + dir[i]]) {
            if (arr[coord + dir[i]]->getSide() == side) {
                continue;
            } else {
              if(!isPinned(coord, coord + dir[i])) continue;
                ret.push_back(coord + dir[i]);
                continue;
            }
        }
        if(!isPinned(coord, coord + dir[i])) continue;
        ret.push_back(coord + dir[i]);
    }
    return ret;
}

// legalSlidingMoves: Returns possible moves for sliding pieces (rook, queen, bishop)
vector<int> Board::legalSlidingMoves(int coord, char p, bool side) {
    vector<int> dist;
    vector<int> dir;

    if (p == 'R') {
        dist = distanceToEdge(coord);
        dir = {-1, 1, 8, -8};
    }
    if (p == 'B') {
        dist = bishopDist(coord);
        dir = {7, 9, -7, -9};
    }
    if (p == 'Q') {
        vector<int> dist1 = distanceToEdge(coord);
        vector<int> dist2 = bishopDist(coord);
        dir = {-1, 1, 8, -8, 7, 9, -7, -9};
        dist1.insert(dist1.end(), dist2.begin(), dist2.end());
        dist = dist1;
    }
    vector<int> ret;
    for (size_t i = 0; i < dir.size(); i++) {
        for(int j = 1; j < dist[i] + 1; j++) {
            
            if (arr[coord + j * dir[i]]) {
                if (arr[coord + j * dir[i]]->getSide() == side) break;
                else {
                  if(!isPinned(coord, coord + j * dir[i])) break;
                    ret.push_back(coord + j * dir[i]);
                    break;
                }
            }
            if(!isPinned(coord, coord + j * dir[i])) continue;

            ret.push_back(coord + j * dir[i]);
        }
    }
    return ret;
}

// distanceToEdge: Helper function for legalSlidingMoves,
// Returns distance to each edge of the board {left, right, up down} used by rooks and queens
vector<int> Board::distanceToEdge(int coord) {
    vector<int> dist{ 0, 0, 0, 0 };
    int i = coord;
    while (i % 8 != 0) {
        dist[0]++;
        i--;
    }
    dist[1] = 8 - dist[0] - 1;
    i = coord;
    while (i < 56) {
        dist[2]++;
        i+=8;
    }
    i = coord;
    dist[3] = 8 - dist[2] - 1;
    return dist;
}

// isUnderAttackMove: returns true if coord is attacked by the side opposite move
bool Board::isUnderAttackMove(int coord, Move move) {
  int start = move.getStart();
  int dest = move.getDest();
  bool side = arr[start]->getSide();
  Board boardCopy = *this;
  boardCopy.movePiece(move);
  if (boardCopy.isUnderAttack(dest, side) != -1) {
    return true;
  }
  return false;
}

// isUnderAttack: returns the coord of the highest-value attacking piece if coord is attacked by !side, false otherwise
int Board::isUnderAttack(int coord, bool side) {

  // Sliding moves
    vector<int>rookdir {-1, 1, 8, -8};
    vector<int>bishdir {7, 9, -7, -9};
    vector<int> dist = distanceToEdge(coord);
    vector<int> dist2 = bishopDist(coord);

    for (size_t i = 0; i < rookdir.size(); i++) {
        for(int j = 1; j < dist[i] + 1; j++) {
            if (arr[coord + j * rookdir[i]]) { 
                if ((arr[coord + j * rookdir[i]]->getCode() == 'Q' ||
                     arr[coord + j * rookdir[i]]->getCode() == 'R') && arr[coord + j * rookdir[i]]->getSide() == !side) {
                    return coord + j * rookdir[i];
                } else {
                    break;
                }
            }
        }
    }
    for (size_t i = 0; i < bishdir.size(); i++) {
        for(int j = 1; j < dist2[i] + 1; j++) {
            if (arr[coord + j * bishdir[i]]) {
                if ((arr[coord + j * bishdir[i]]->getCode() == 'Q' ||
                    arr[coord + j * bishdir[i]]->getCode() == 'B') && arr[coord + j * bishdir[i]]->getSide() == !side) {
                    return coord + j * bishdir[i];
                } else {
                    break;
                }
        
            } 
        }
    }


  // Knight
    vector<int> Knightdir{6, 15, 17, 10, -6, -15, -17, -10};
    if (coord % 8 == 0) {
        Knightdir = {17, 10, -6, -15};
    } else if (coord % 8 == 1) {
        Knightdir = {15, 17, 10, -6, -15, -17};
    } else if (coord % 8 == 6) {
        Knightdir = {6, 15, 17, -15, -17, -10};
    } else if (coord % 8 == 7) {
        Knightdir = {6, 15, -17, -10};
    }

    for (size_t i = 0; i < Knightdir.size(); i++) {
        if (coord + Knightdir[i] > 63 || coord + Knightdir[i] < 0) {
            continue;
        }
        if (arr[coord + Knightdir[i]] && arr[coord + Knightdir[i]]->getCode() == 'N') {
            if (arr[coord + Knightdir[i]]->getSide() == !side) {
                    return coord + Knightdir[i];
                }
        }
    }


  // Pawn
    vector<int>pawndir;
    if (side) {
        if (coord % 8 == 0) {
            pawndir = {9};
        } else if (coord % 8 == 7) {
            pawndir = {7};
        } else {
            pawndir = {7, 9};
        }
    } else {
        if (coord % 8 == 0) {
            pawndir = {-7};
        } else if (coord % 8 == 7) {
            pawndir = {-9};
        } else {
            pawndir = {-7, -9};
        }
    }

    for (size_t i = 0; i < pawndir.size(); i++) {
        if (coord + pawndir[i] > 63 || coord + pawndir[i] < 0) {
            continue;
        }
        if (arr[coord + pawndir[i]] != nullptr && arr[coord + pawndir[i]]->getCode() == 'P') {
            if (arr[coord + pawndir[i]]->getSide() == !side) {
                    return coord + pawndir[i];
            }
        }
    }


  // King
  vector<int>kingdir {-1, 1, 8, -8, 7, 9, -7, -9};
    if (coord % 8 == 0) {
        kingdir = {1, 8, -8, 9, -7};
    } else if (coord % 8 == 7) {
        kingdir = {-1, 8, -8, 7, -9};
    }
    for (size_t i = 0; i < kingdir.size(); i++) {
        if (coord + kingdir[i] > 63 || coord + kingdir[i] < 0) {
            continue;
        }
        if (arr[coord + kingdir[i]] && arr[coord + kingdir[i]]->getCode() == 'K') {
            if (arr[coord + kingdir[i]]->getSide() == !side) {
              return coord + kingdir[i];
            }
        }
    }
  
    return -1;
}

// isInCheck: Checks if side is in check in the current position
bool Board::isInCheck(bool side) {
  if (side) return isUnderAttack(whiteKing, 1) != -1;
  else return isUnderAttack(blackKing, 0) != -1;
}

// enPassant: enPassant checker, sets all enPassant flags for !side to be
// false, called after each side's turn
void Board::enPassant(bool side) {
  
    if (side) {
        for (int i = 32; i <= 39; i++) {
            if (arr[i]) {
              arr[i]->setEnPassant(false);
            }
        }
    } else {
        for (int i = 24; i <= 31; i++) {
            if (arr[i]) {
              arr[i]->setEnPassant(false);
            }
        }
    }
}

// helper function for legalSlidingMoves,
//   returns distance on each diagonal to the edges of board
//   {upleft, upright, downright, downleft} used by bishops and queens
vector<int> Board::bishopDist(int coord) {
    vector<int> dist = distanceToEdge(coord);
    vector<int> res{0,0,0,0};
    res[0] = min(dist[0], dist[2]);
    res[1] = min(dist[1], dist[2]);
    res[2] = min(dist[1], dist[3]);
    res[3] = min(dist[0], dist[3]);
    return res;
}

// Returns true if both sides have insufficient material to checkmate
bool Board::insufficientMaterial() {
  return !(sufficientMaterial(1) || sufficientMaterial(0));
}

// Returns true if side has sufficient material to checkmate
bool Board::sufficientMaterial(bool side) {
  
  if(side){
    if (whitePieces.size() == 2) {
    if (((arr[whitePieces[0]]->getCode() == 'K') &&
         (arr[whitePieces[1]]->getCode() == 'N')) ||
        ((arr[whitePieces[1]]->getCode() == 'K') &&
         (arr[whitePieces[0]]->getCode() == 'N'))) {
      return false;
    } else if (((arr[whitePieces[0]]->getCode() == 'K') &&
                (arr[whitePieces[1]]->getCode() == 'B')) ||
               ((arr[whitePieces[1]]->getCode() == 'K') &&
                (arr[whitePieces[0]]->getCode() == 'B'))) {
      return false;
    }else{
      return true;
    }
  } else if (whitePieces.size() == 1) {
    return false;
  } else {
    bool allWhite = true;
    bool allBlack = true;

    for(size_t i = 0; i < whitePieces.size(); i++){
      if((arr[whitePieces[i]]->getCode() != 'B') && (arr[whitePieces[i]]->getCode() != 'K')){
        return true;
      }else if((arr[whitePieces[i]]->getCode() == 'B')){
        if(whitePieces[i] % 2 == 1){
          allBlack = false;
        }else{
          allWhite = false;
        }
      }
    }

      return !(allWhite || allBlack);
  }
  }else{
    if (blackPieces.size() == 2) {
    if (((arr[blackPieces[0]]->getCode() == 'K') &&
         (arr[blackPieces[1]]->getCode() == 'N')) ||
        ((arr[blackPieces[1]]->getCode() == 'K') &&
         (arr[blackPieces[0]]->getCode() == 'N'))) {
      return false;
    } else if (((arr[blackPieces[0]]->getCode() == 'K') &&
                (arr[blackPieces[1]]->getCode() == 'B')) ||
               ((arr[blackPieces[1]]->getCode() == 'K') &&
                (arr[blackPieces[0]]->getCode() == 'B'))) {
      return false;
    }else{
      return true;
    }
  } else if (blackPieces.size() == 1) {
    return false;
  } else {
    bool allWhite = true;
    bool allBlack = true;

    for(size_t i = 0; i < blackPieces.size(); i++){
      if((arr[blackPieces[i]]->getCode() != 'B') && (arr[blackPieces[i]]->getCode() != 'K')){
        return true;
      }else if((arr[blackPieces[i]]->getCode() == 'B')){
        if(blackPieces[i] % 2 == 1){
          allBlack = false;
        }else{
          allWhite = false;
        }
      }
    }
      return !(allWhite || allBlack);
  }
  }
  
}

// getBoard: Gets a copy of the board as characters
vector<char> Board::getBoard() const {
    vector<char> ret(64, 'a');

    for (int i = 0; i < 64; i++) {
      if (arr[i] != nullptr) {
        bool color = arr[i]->getSide();
        char code = arr[i]->getCode();

        if (color) {
          switch (code) {
          case 'K':
            ret[i] = 'K';
            break;
          case 'Q':
            ret[i] = 'q';
            break;
          case 'B':
            ret[i] = 'B';
            break;
          case 'R':
            ret[i] = 'R';
            break;
          case 'N':
            ret[i] = 'N';
            break;
          case 'P':
            ret[i] = 'P';
            break;
          default:
            cout << "Invald" << endl;
          }

        } else {

          switch (code) {
          case 'K':
            ret[i] = 'k';
            break;
          case 'Q':
            ret[i] = 'q';
            break;
          case 'B':
            ret[i] = 'b';
            break;
          case 'R':
            ret[i] = 'r';
            break;
          case 'N':
            ret[i] = 'n';
            break;
          case 'P':
            ret[i] = 'p';
            break;
          default:
            cout << "Inalid" << endl;
          }
        }
      }
    }

    return ret;
}

bool Board::kingExists(bool side) const{
  if(side){
    if(whiteKing <= 63 && whiteKing >= 0){
      return true;
    }else{
      return false;
    }
  }else{
    if(blackKing <= 63 && blackKing >= 0){
      return true;
    }else{
      return false;
    }
  }
}


// Output stream operator for board class
std::ostream& operator<<(std::ostream &out, const Board& b) {
    bool c = false;
    for (int i = 7; i >= 0; i--) {
        out << i + 1 << " ";
        for (int j = 8 * i; j < 8 * (i + 1); j++) {
            if (b.at(j)) {
                if (b.at(j)->getSide()) {
                    out << b.at(j)->getCode();
                } else {
                    int s = int(b.at(j)->getCode()) + 32;
                    out << char(s);
                }
            } else {
                if (c) {
                    out << " ";
                } else {
                    out << "_";
                }
            }

            if ((j + 1) % 8 != 0) {
                c = !c;
            }

        }
        out << endl;
    }
    out << endl << "  abcdefgh" << endl;
  return out;
}

