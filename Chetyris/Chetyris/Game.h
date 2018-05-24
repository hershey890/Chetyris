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
	int		m_rows_destroyed;
	int		m_rows_left;
	int		m_score;
	int		m_counter;
	Piece	m_current_piece;
	Piece	m_next_piece;

	enum m_direction { DOWN, LEFT, RIGHT, SPACE_DOWN };

	/*renders the piece in the well*/
	void printPiece(Piece& piece, const int& x, const int& y);

	/*erases the piece in the well each before the piece moves and 
	is re-rendered by printpiece*/
	void erasePiece(Piece& piece, const int& x, const int& y);

	/*Checks if the piece can move down, left or right*/
	bool canMove(const m_direction& dir, Piece& piece);

	/*Moves the char left or right*/
	void movePiece(Piece& piece, const char& ch, int& x, int& y);

	double timeLeft(Timer& timer) const;

	/*Sets the piece as '$' if it can not shift down any further*/
	void pieceToRow(Piece& piece);

	/*checks if the piece can turn (i.e. won't hit any existing pieces
	or walls*/
	bool rotatePiece(Piece& piece, const int& x, const int& y);

	bool destroyRow();

	int rows_left();

	void reset();

	bool foam_bomb(const int& x, const int& y, int x_filledL = 2, int x_filledR = 2, int y_filledU = 2, int y_filledD = 2);

	void displayNextPiece();

	bool game_ended();

	bool vaporBomb();

	void nextPiece(Timer& timer, int& x_pos, int& y_pos, char& key_press);
};

#endif // GAME_INCLUDED