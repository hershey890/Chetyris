#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "UserInterface.h"
#include "Piece.h"
#include <string>
// [Add other #include directives as necessary.]

class Game
{
  public:
    Game(int width, int height);
    void play();
    bool playOneLevel();
    void displayPrompt(const std::string s);
    void displayStatus();
	
  private:
    Well    m_well;
    Screen  m_screen;
    int     m_level;
	enum m_direction { DOWN, LEFT, RIGHT, SPACE_DOWN };

	/*renders the piece in the well*/
	void printPiece(Piece piece, const int& x, const int& y);

	/*erases the piece in the well each before the piece moves and 
	is re-rendered by printpiece*/
	void erasePiece(Piece piece, const int& x, const int& y);

	/*Checks if the piece can move down, left or right*/
	bool canMove(m_direction dir, Piece piece);

	/*Moves the char left or right*/
	void movePiece(const Piece& piece, const char& ch, int& x, int& y);

	double timeLeft(Timer timer, const int level) const;

	/*Sets the piece as '$' if it can not shift down any further*/
	void pieceToRow(Piece piece);

	/*checks if the piece can turn (i.e. won't hit any existing pieces
	or walls*/
	bool rotatePiece(Piece piece, int x, int y);
};

#endif // GAME_INCLUDED