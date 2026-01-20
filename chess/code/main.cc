#include <iostream>
#include "board.h"
#include "piece.h"
#include "move.h"
#include "computer.h"
#include "level1.h"
#include <map>
#include "window.h"

using namespace std;

// Print toggling
bool toggleBoardPrint = true;
bool togglePGNPrint = false;

bool toggleMovePrint = false;
size_t maxMoves = 800;


float blackWins = 0;
float whiteWins = 0;


Xwindow wind {500, 500};



//0 is just a move or capture, 1 is en passant, 2 is castling, 3 is promotion
void updateBoard(int start, int end, char code, bool side, int special,
                 char promotionCode) {

  if (special == 1) {
    if (end - start == 9) {
      wind.drawSquare(start);
      wind.drawSquare(start + 1);
      wind.drawPiece(end, code, side);
    }
    if (end - start == 7) {
      wind.drawSquare(start);
      wind.drawSquare(start - 1);
      wind.drawPiece(end, code, side);
    }
    if (end - start == -7) {
      wind.drawSquare(start);
      wind.drawSquare(start + 1);
      wind.drawPiece(end, code, side);
    }
    if (end - start == -9) {
      wind.drawSquare(start);
      wind.drawSquare(start - 1);
      wind.drawPiece(end, code, side);
    }
  } else if (special == 2) {
    if (end - start == 2) {
      wind.drawSquare(start);
      wind.drawSquare(end + 1);
      wind.drawPiece(end, code, side);
      wind.drawPiece(end - 1, 'R', side);
    } else {
      wind.drawSquare(start);
      wind.drawSquare(end - 2);
      wind.drawPiece(end, code, side);
      wind.drawPiece(end + 1, 'R', side);
    }
  } else if (special == 3) {
    wind.drawSquare(start);
    wind.drawSquare(end);
    wind.drawPiece(end, promotionCode, side);
  } else {
    wind.drawSquare(start);
    wind.drawSquare(end);
    wind.drawPiece(end, code, side);
  }
}


// Converts alphanumeric coordinates to integer coordinates
int squareToCoord(string square) {
	int coord = 0;
	coord += (square[0]) - 'a';
	coord += 8 * (((square[1]) - '0') - 1);
	return coord;
}

// Converts integer coordinates to alphanumeric coordinates
string coordToSquare(int coord) {
  char col = coord % 8 + 'a';
  char row = coord / 8 + '1';
  string square = "";
  square += col;
  square += row;
  return square;
}

// Converts a move to chess PGN notation
string moveToPGN(Board *chessBoard, Move move) {
  int start = move.getStart();
  int dest = move.getDest();
  char promotion = move.getPromotion();

  bool side = chessBoard->at(start)->getSide();
  char piece = chessBoard->at(start)->getCode();
  string startSquare = coordToSquare(start);
  string destSquare = coordToSquare(dest);
 
  // Castling
  if (piece == 'K') {
    if ((side && start == 4 && dest == 6) || (!side && start == 60 && dest == 62)) return "O-O";
    else if ((side && start == 4 && dest == 2) || (!side && start == 60 && dest == 58)) return "O-O-O";
  }

	string pgn = "";
 
  // Add piece notation
	if (piece != 'P') {
		pgn += piece;

    // When multiple of the same piece can move to a square
    bool multiple = false;
    bool sameCol = false;
    bool sameRow = false;

    for (auto pieceCoord : chessBoard->getPieces(side)) {
      if (chessBoard->at(pieceCoord)->getCode() == piece && pieceCoord != start) {
        for (auto move : chessBoard->legalMoves(pieceCoord)) {

          if (move.getDest() == dest) {
            multiple = true;

            // Find if a piece in the same column/row can move to the same dest
            if (pieceCoord % 8 == start % 8) sameCol = true;
            else if (pieceCoord / 8 == start / 8) sameRow = true;

            break;
          }
        }
      }
    }

    if (sameCol && sameRow) pgn += startSquare;
    else if (sameCol) pgn += startSquare[1];
    else if (multiple) pgn += startSquare[0];
	}


  // Add capture notation (second line is en passant check)
  if ((chessBoard->at(dest) != nullptr) ||
      (piece == 'P' && chessBoard->at(dest) == nullptr && (abs(dest - start) == 7 || abs(dest - start) == 9))) {

    // Add column of pawn
    if (piece == 'P') {
      pgn += startSquare[0];
    }

    pgn += 'x';
  }

  
  // Add destination square
	pgn += destSquare;

  
  // Add promotion
  if (promotion != '0') {
    pgn += '=';
    pgn += promotion;
  }
  

  // Add check or checkmate notation
  if (chessBoard->isCheck(move)) {
    if (chessBoard->noMoves(!side)) pgn += '#';
    else pgn += '+';
  }

  return pgn;
}

// Prints a formatted PGN
void printPGN(vector<string> pgnLog) {
  for (size_t i = 0; i < pgnLog.size(); i++) {
    if (i % 2 == 0) cout << i / 2 + 1 << ". " << pgnLog.at(i) << " ";
    else cout << pgnLog.at(i) << " ";
  }
  cout << endl;
}

// Prints a move list
void printMoves(vector<Move> moveLog) {
  
  for (size_t i = 0; i < moveLog.size(); i++) {
    int start = moveLog.at(i).getStart();
    int dest = moveLog.at(i).getDest();
    char promotion = moveLog.at(i).getPromotion();
    
    cout << "move " << coordToSquare(start) << " " << coordToSquare(dest);

    if (promotion != '0') cout << " " << promotion;
    cout << endl;
  }
}

// Custom board setup
void setupBoard(Board *chessBoard, bool &turn) {
  wind.fillBoard();
	string oper;
  bool flag = true;
  while(flag){
    	cin >> oper;
    while (oper != "done") {
		if (oper == "+") {
			
			char piece;
			cin >> piece;
      string square;
			cin >> square;
			int coord = squareToCoord(square);
			if (0 <= coord && coord <= 63) {
				
				switch (piece) {
				case 'K':
          if(!(chessBoard->kingExists(1))){
            chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new King(1, 0));
            wind.drawSquare(coord);
            wind.drawPiece(coord, 'K', 1);
          }else cout << "Invalid plement" << endl;
					break;
				case 'Q':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Queen(1));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'Q', 1);
					break;
				case 'B':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Bishop(1));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'B', 1);
					break;
				case 'R':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Rook(1, 0));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'R', 1);
					break;
				case 'N':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Knight(1));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'N', 1);
					break;
				case 'P':
          if (8 <= coord && coord <= 55) {
					  chessBoard->removePiece(coord);
  					chessBoard->placePiece(coord, new Pawn(1, 0, 0));
            wind.drawSquare(coord);
          wind.drawPiece(coord, 'P', 1);
          } else cout << "Invalid placement" << endl;
					break;
				case 'k':
          if(!(chessBoard->kingExists(0))){
          chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new King(0, 0));
            wind.drawSquare(coord);
          wind.drawPiece(coord, 'K', 0);
          }else cout << "Invalid placent" << endl;
					break;
				case 'q':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Queen(0));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'Q', 0);
					break;
				case 'b':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Bishop(0));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'B', 0);
					break;
				case 'r':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Rook(0, 0));
          wind.drawSquare(coord);
          wind.drawPiece(coord, 'R', 0);
					break;
				case 'n':
					chessBoard->removePiece(coord);
					chessBoard->placePiece(coord, new Knight(0));
          wind.drawSquare(coord);
        wind.drawPiece(coord, 'N', 0);
					break;
				case 'p':
					if (8 <= coord && coord <= 55) {
					  chessBoard->removePiece(coord);
  					chessBoard->placePiece(coord, new Pawn(0, 0, 0));
            wind.drawSquare(coord);
            wind.drawPiece(coord, 'P', 0);
          } else cout << "Invalid placement" << endl;
          break;
				default:
					cout << "Invalid placement" << endl;
				}
			} else {
				cout << "Invalid placement" << endl;
			}
      
			if (toggleBoardPrint) cout << *chessBoard << endl;
			
		} else if (oper == "-") {
			
			string square;
			cin >> square;
			int coord = squareToCoord(square);
			if (0 <= coord && coord <= 63) {
				if (chessBoard->removePiece(coord)) {
          wind.drawSquare(coord);
					if (toggleBoardPrint) cout << *chessBoard << endl;
				}
			}
			
		} else if (oper == "=") {
			
			string color;
			cin >> color;
			if (color == "white") {
				turn = 1;
			} else if (color == "black") {
				turn = 0;
			} else {
				cout << "Color exists" << endl;
			}
			
		} else {
			 cout << "INVALID SETUP INPUT, AGAIN or say done to end setup" << endl;
		}
		cin >> oper;
	}
    if(!chessBoard->kingExists(1)) {
      cout<< "There is no white king on the board. "<<endl;
    } else if(!chessBoard->kingExists(0)) {
      cout<< " Place a black king on the board and enter done."<<endl;
    } else if(chessBoard->isInCheck(1)) {
      cout<< "The white king is currently in check. "<<endl;
    } else if(chessBoard->isInCheck(0)) {
      cout<< " Remove the check threat and enter done."<<endl;
    } else {
      flag = false;
    }
  }
}

// Default board setup
void defaultBoard(Board *chessBoard, bool &turn) {

  turn = 1;
  
  wind.fillBoard();
  wind.drawPiece(0, 'R', 1);
  wind.drawPiece(1, 'N', 1);
  wind.drawPiece(2, 'B', 1);
  wind.drawPiece(3, 'Q', 1);
  wind.drawPiece(4, 'K', 1);
  wind.drawPiece(5, 'B', 1);
  wind.drawPiece(6, 'N', 1);
  wind.drawPiece(7, 'R', 1);
  wind.drawPiece(8, 'P', 1);
  wind.drawPiece(9, 'P', 1);
  wind.drawPiece(10, 'P', 1);
  wind.drawPiece(11, 'P', 1);
  wind.drawPiece(12, 'P', 1);
  wind.drawPiece(13, 'P', 1);
  wind.drawPiece(14, 'P', 1);
  wind.drawPiece(15, 'P', 1);


  wind.drawPiece(48, 'P', 0);
  wind.drawPiece(49, 'P', 0);
  wind.drawPiece(50, 'P', 0);
  wind.drawPiece(51, 'P', 0);
  wind.drawPiece(52, 'P', 0);
  wind.drawPiece(53, 'P', 0);
  wind.drawPiece(54, 'P', 0);
  wind.drawPiece(55, 'P', 0);
  wind.drawPiece(56, 'R', 0);
  wind.drawPiece(57, 'N', 0);
  wind.drawPiece(58, 'B', 0);
  wind.drawPiece(59, 'Q', 0);
  wind.drawPiece(60, 'K', 0);
  wind.drawPiece(61, 'B', 0);
  wind.drawPiece(62, 'N', 0);
  wind.drawPiece(63, 'R', 0);
  
  
	// White pieces
	chessBoard->placePiece(0, new Rook(1, 0));
	chessBoard->placePiece(1, new Knight(1));
	chessBoard->placePiece(2, new Bishop(1));
	chessBoard->placePiece(3, new Queen(1));
	chessBoard->placePiece(4, new King(1, 0));
	chessBoard->placePiece(5, new Bishop(1));
	chessBoard->placePiece(6, new Knight(1));
	chessBoard->placePiece(7, new Rook(1, 0));
	chessBoard->placePiece(8, new Pawn(1, 0, 0));
	chessBoard->placePiece(9, new Pawn(1, 0, 0));
	chessBoard->placePiece(10, new Pawn(1, 0, 0));
	chessBoard->placePiece(11, new Pawn(1, 0, 0));
	chessBoard->placePiece(12, new Pawn(1, 0, 0));
	chessBoard->placePiece(13, new Pawn(1, 0, 0));
	chessBoard->placePiece(14, new Pawn(1, 0, 0));
	chessBoard->placePiece(15, new Pawn(1, 0, 0));
	
	// Black pieces
	chessBoard->placePiece(48, new Pawn(0, 0, 0));
	chessBoard->placePiece(49, new Pawn(0, 0, 0));
	chessBoard->placePiece(50, new Pawn(0, 0, 0));
	chessBoard->placePiece(51, new Pawn(0, 0, 0));
	chessBoard->placePiece(52, new Pawn(0, 0, 0));
	chessBoard->placePiece(53, new Pawn(0, 0, 0));
	chessBoard->placePiece(54, new Pawn(0, 0, 0));
	chessBoard->placePiece(55, new Pawn(0, 0, 0));
	chessBoard->placePiece(56, new Rook(0, 0));
	chessBoard->placePiece(57, new Knight(0));
	chessBoard->placePiece(58, new Bishop(0));
	chessBoard->placePiece(59, new Queen(0));
	chessBoard->placePiece(60, new King(0, 0));
	chessBoard->placePiece(61, new Bishop(0));
	chessBoard->placePiece(62, new Knight(0));
	chessBoard->placePiece(63, new Rook(0, 0));
}

// Check the current position for checkmates and stalemates
// Returns true if a checkmate or stalemate is found, false otherwise
bool endCheck(Board *chessBoard, bool turn, bool side, int fiftyMoveCounter, map<vector<char>, int> &boardMap) {
  

	if (chessBoard->noMoves(!turn)) {

    // Checkmate
		if (chessBoard->isInCheck(!turn)) {
			if (side) {
        whiteWins++;
        cout << "Checkmate! White wins" << endl;
      } else {
        blackWins++;
        cout << "Checkmate!Black wins!" << endl;
      }

    // Stalemate
		} else {
			whiteWins += 0.5;
			blackWins += 0.5;
      cout << "Statemate!" << endl;
      
		}
		return true;

  // Draw by insufficient material
	} else if (chessBoard->insufficientMaterial()) {
      whiteWins += 0.5;
			blackWins += 0.5;
      cout << "Draw by insufficient material to mate!" << endl;
    return true;

  // Draw by fifty move rule
  } else if (!turn && fiftyMoveCounter >= 50) {
    whiteWins += 0.5;
		blackWins += 0.5;
    cout << "Draw by fifty rule!" << endl;
    return true;

  // Draw by threefold repetition
  } else if (boardMap.at(chessBoard->getBoard()) >= 3) {
    whiteWins += 0.5;
		blackWins += 0.5;
    cout << "Draw by twofold repetition!" << endl;
    return true;
  }
  return false;
}

// End of turn processing
void processTurnEnd(Board *chessBoard, Computer *whiteComputer, Computer *blackComputer, map<vector<char>, int> &boardMap, 
                    Move &move, int &fiftyMoveCounter, bool &turn, bool &toggleBoardPrint, bool &gameEnd, bool &computerTurn) {
  
  if (toggleBoardPrint) cout << *chessBoard << endl;
  
  boardMap[chessBoard->getBoard()]++;
  
  chessBoard->enPassant(turn);
  gameEnd = endCheck(chessBoard, turn, chessBoard->at(move.getDest())->getSide(), fiftyMoveCounter, boardMap);
  turn = !turn;
  
  if ((turn && whiteComputer != nullptr) || (!turn && blackComputer != nullptr)) computerTurn = true;
  else computerTurn = false;
}

// End of game processing
void processGameEnd(map<vector<char>, int> &bMap, vector<string> &pgnLog, vector<Move> &moveLog, 
              bool &customSetup, bool &gameCreated, bool &gameStart, bool &gameEnd, bool &computerTurn) {

  
  if (togglePGNPrint) printPGN(pgnLog);
  if (toggleMovePrint) printMoves(moveLog);
  pgnLog.erase(pgnLog.begin(), pgnLog.end());
  moveLog.erase(moveLog.begin(), moveLog.end());
  bMap.clear();
  customSetup = false;
  gameCreated = false;
  gameStart = false;
  gameEnd = false;
  computerTurn = false;
}


int main(int argc, char *argv[]) {

  if (argc > 1) {
    if (string(argv[1]) == "-pgn") togglePGNPrint = true;
  }
  
	Board *chessBoard = nullptr;
  
	bool turn = 1;
	bool customSetup = false;
  bool gameCreated = false; // Tracks if the game command has been called
  bool gameStart = false;   // Tracks if the first move has been made
	bool gameEnd = false;
  int fiftyMoveCounter = 0;

  Computer *whiteComputer = nullptr;
  Computer *blackComputer = nullptr;
  bool computerTurn = false;
  
  vector<string> pgnLog;
  vector<Move> moveLog;
  map<vector<char>, int> boardMap;
  
	string input;

  
  // Main game loop
  while (computerTurn || cin >> input) {

    // Computer move ---------------------------------------------------------
    if (computerTurn) {

      // Generate move
      Move move{0, 0};
      if (turn) move = whiteComputer->generateMove();
      else move = blackComputer->generateMove();
      
      int start = move.getStart();
      int dest = move.getDest();
      // char promotion = move.getPromotion();
      
      // Before-move processing
      moveLog.push_back(move);
      pgnLog.push_back(moveToPGN(chessBoard, move));
      if (chessBoard->at(start)->getCode() == 'P' || chessBoard->at(dest) != nullptr) {
        fiftyMoveCounter = 0;
      }
      if (turn) fiftyMoveCounter++;
      
      
      //Update the graphics
      
      if((chessBoard->at(start)->getCode() == 'P') &&(abs(dest - start) != 16)&&(abs(dest - start) != 8) && (chessBoard->at(dest) == nullptr)){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 1, 'a');
      }else if ((chessBoard->at(start)->getCode() == 'K') &&(abs(dest - start) == 2)){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 2, 'a');
      }else if (move.getPromotion() != '0'){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 3, move.getPromotion());
      }else{
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 0, 'a');
      }
      

      // Move
      chessBoard->movePiece(move);

      // End of turn processing
      processTurnEnd(chessBoard, whiteComputer, blackComputer, boardMap, 
        move, fiftyMoveCounter, turn, toggleBoardPrint, gameEnd, computerTurn);

      // End of game processing
      if (gameEnd) processGameEnd(boardMap, pgnLog, moveLog, customSetup, gameCreated, gameStart, gameEnd, computerTurn);
      
      continue;
    }


    // Human input -----------------------------------------------------------

    
    // Move piece
    if (input == "move") {
      
      if (!gameCreated) {
        cout << "Games not created" << endl;
        continue;
      }
      gameStart = true;

      // Read move
			string startSquare;
			cin >> startSquare;
			string destSquare;
			cin >> destSquare;
			int start = squareToCoord(startSquare);
			int dest = squareToCoord(destSquare);
      Move move{start, dest};

      // Check for illegal moves
			if (!(0 <= start && start <= 63 && 0 <= dest && dest <= 63 && 
            chessBoard->at(start) && chessBoard->at(start)->getSide() == turn)) {
        cout << "Invalid Mve" << endl;
        continue;
      }
			vector<Move> moves = chessBoard->legalMoves(start);
			bool isLegalMove = false;
			if (moves.size() <= 0) {
        cout << "Invlid Move" << endl;
        continue;
      }
			for (size_t i = 0; i < moves.size(); i++) {
				if (moves.at(i).getDest() == dest) {
          isLegalMove = true;
          break;
        }
			}
			if (!isLegalMove) {
        cout << "Invalid Mov" << endl;
        continue;
      }


      // Before-move processing
      moveLog.push_back(move);
      pgnLog.push_back(moveToPGN(chessBoard, move));
      if (chessBoard->at(start)->getCode() == 'P' || chessBoard->at(dest) != nullptr) {
        fiftyMoveCounter = 0;
      }
      if (turn) fiftyMoveCounter++;

      // Read pawn promotion value
			if (chessBoard->isPromotionMove(move)) {
				char newPiece;
				cin >> newPiece;
        
        if (newPiece == 'Q' || newPiece == 'B' || newPiece == 'R' || newPiece == 'N') {
          move.setPromotion(newPiece);
        } else cout << "Invaid Move" << endl;
      }
      
    

      //Update the graphics
      
      if((chessBoard->at(start)->getCode() == 'P') &&(abs(dest - start) != 16)&&(abs(dest - start) != 8) && (chessBoard->at(dest) == nullptr)){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 1, 'a');
      }else if ((chessBoard->at(start)->getCode() == 'K') &&(abs(dest - start) == 2)){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 2, 'a');
      }else if (move.getPromotion() != '0'){
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 3, move.getPromotion());
      }else{
        updateBoard(start, dest, chessBoard->at(start)->getCode(), chessBoard->at(start)->getSide(), 0, 'a');
      }
      

      // Move
      chessBoard->movePiece(move);
      
      // End of turn processing
      processTurnEnd(chessBoard, whiteComputer, blackComputer, boardMap, 
        move, fiftyMoveCounter, turn, toggleBoardPrint, gameEnd, computerTurn);
      

      // End of game processing
			if (gameEnd) processGameEnd(boardMap, pgnLog, moveLog, customSetup, gameCreated, gameStart, gameEnd, computerTurn);

      continue;
    }


    if (input == "moves") {
      
      if (!gameCreated) {
        cout << "Game not created" << endl;
        continue;
      }

      string startSquare;
      cin >> startSquare;
      int start = squareToCoord(startSquare);
      if(chessBoard->at(start) != nullptr) {
        vector<Move> moves = chessBoard->legalMoves(start);
        for (size_t i = 0; i < moves.size(); i++) {
          string posSquare = coordToSquare(moves.at(i).getDest());
          cout << posSquare;
          if (i == (moves.size() - 1)) {
            cout << endl;
          } else {
            cout << ", ";
          }
        }
      }
      continue;
    }


    
    // Starts a new game, stopping the current one if it exists
    // Uses the custom board setup if it exists
    if (input == "game") {
      
      delete whiteComputer;
      delete blackComputer;
      whiteComputer = nullptr;
      blackComputer = nullptr;
      
      gameCreated = false;
      gameStart = false;
      gameEnd = false;
      computerTurn = false;
      fiftyMoveCounter = 0;

      // Default board setup if no custom board is set
      if (!customSetup) {
        delete chessBoard;
        chessBoard = new Board;
  		  defaultBoard(chessBoard, turn);
      }
      customSetup = false;
      
      string white;
      cin >> white;
      string black;
      cin >> black;

      // Create computers
      if (white == "computer1") whiteComputer = new Level1(chessBoard, 1);
      
      if (black == "computer1") blackComputer = new Level1(chessBoard, 0);

      // Invalid input
      if ((white != "human" && whiteComputer == nullptr) ||
          (black != "human" && blackComputer == nullptr)) {
        cout << "Invalid Input" << endl;
        delete whiteComputer;
        delete blackComputer;
        whiteComputer = nullptr;
        blackComputer = nullptr;
        continue;
      }

      if ((turn && whiteComputer != nullptr) || (!turn && blackComputer != nullptr)) computerTurn = true;
      
      if (!computerTurn) if (toggleBoardPrint) cout << *chessBoard << endl;
      gameCreated = true;

      continue;
    }

    
    // Custom board setup
    // Cannot be called if a game is in progress
    if (input == "setup") {
      if (!gameStart) {
        customSetup = true;
        
        delete chessBoard;
        chessBoard = new Board;
		    setupBoard(chessBoard, turn);
      } else {
        cout << "Cannot enter setup mode, game in progress" << endl;
      }

      continue;
    }

    
    // Resignation
    if (input == "resign") {

      if (!gameCreated) {
        cout << "Game not created" << endl;
        continue;
      }
      
			if (turn) {
				blackWins++;
        cout << "Blackwins!" << endl;
			} else {
				whiteWins++;
        cout << "Whitewins" << endl;
			}

      // End of game processing
      processGameEnd(boardMap, pgnLog, moveLog, customSetup, gameCreated, gameStart, gameEnd, computerTurn);
      
			continue;
		}

    
    cout << "Invalid Input" << endl;
  }
  
	cout << "Final Score:" << endl;
	cout << "White   : " << whiteWins << endl;
	cout << "B1ack: " << blackWins << endl;

  // Free memory
  delete chessBoard;
  delete whiteComputer;
  delete blackComputer;
}

