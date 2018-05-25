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

	bool playOneLevel();

	void displayPrompt(const std::string s);

	void displayStatus();

	enum m_direction { DOWN, LEFT, RIGHT, SPACE_DOWN };

	/*renders the piece in the well*/
	void printPiece(Piece& piece, const int& x, const int& y);

	/*erases the piece in the well each before the piece moves and 
	is re-rendered by printpiece*/
	void erasePiece(Piece& piece, const int& x, const int& y);

	/* Checks if the piece is capable of moving in a particular direction */
	bool canMove(const m_direction& dir, Piece& piece);

	/* Moves the piece depending on what key was pressed */
	void movePiece(Piece& piece, const char& ch, int& x, int& y);

	/* TIME LEFT, BASED ON : T = maximum(1000-(100*(L-1)), 100) */
	double timeLeft(Timer& timer) const;

	/*Sets the piece as '$' if it can not shift down any further*/
	void pieceToRow(Piece& piece);

	/*checks if the piece can turn (i.e. won't hit any existing pieces
	or walls*/
	bool rotatePiece(Piece& piece, const int& x, const int& y);

	/* If a row is all filled up, destroys the row and increments m_rows_destroyed */
	bool destroyRow();

	/* Returns the number of rows left to finish a level */
	int rows_left();

	/* Resets the well for the next level */
	void reset();

	/* Executes foam_bomb functionality once the piece hits the bottom */
	bool foam_bomb(const int& x, const int& y, int x_filledL = 2, int x_filledR = 2, int y_filledU = 2, int y_filledD = 2);

	/* Calls the piece class and displays a piece in the well */
	void displayNextPiece();

	/* Checks if the new piece overlaps with anything, if true, 
	returns false. Used to end game in playOneLevel*/
	bool game_ended();

	/* Executes vaporbomb functionality once the piece hits the bottom */
	bool vaporBomb(const int& x, const int& y);

	/* Generates the next piece, converts the current piece into '$', and restarts
	the timer*/
	void nextPiece(Timer& timer, int& x_pos, int& y_pos, char& key_press);
};

#endif // GAME_INCLUDED