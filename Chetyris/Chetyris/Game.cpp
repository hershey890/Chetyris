#include "Game.h"
#include "Well.h"
#include "UserInterface.h"
#include <string>
#include <algorithm>
#include <cassert> //REMOVE LATER

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

Game::Game(int width, int height)
	: m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_well(width + 2, height + 1),
	m_rows_destroyed(0), m_rows_left(5), m_score(0),
	m_current_piece(chooseRandomPieceType()), m_next_piece(chooseRandomPieceType())
	//m_current_piece(PIECE_FOAM), m_next_piece(chooseRandomPieceType())
{
}


void Game::play()
{
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]

    for(;;) {
        if ( ! playOneLevel())
            break;
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();
		displayStatus();
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}


void Game::displayPrompt(const std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}


void Game::displayStatus()
{
	/* SCORE */
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printString("Score:     ");
	m_screen.gotoXY(SCORE_X + 17, SCORE_Y);
	m_screen.printString(std::to_string(m_score));
	
	/* ROWS LEFT */
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printString("Rows Left: ");
	m_screen.gotoXY(ROWS_LEFT_X + 17, ROWS_LEFT_Y);
	m_screen.printString(std::to_string(rows_left()));
	//m_screen.printChar(rows_left() + 48); 

	/* LEVEL */
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);	
	m_screen.printString("Level:     ");
	m_screen.gotoXY(LEVEL_X + 17, LEVEL_Y);
	m_screen.printString(std::to_string(m_level));

	/* NEXT PIECE */
	m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
	m_screen.printString("Next Piece:");
	for (int i = 0; i < 16; i++) {
		int p = i % 4;
		int q = i / 4;
		m_screen.gotoXY(NEXT_PIECE_X + p, NEXT_PIECE_Y + q);
		m_screen.printChar(*(m_next_piece.get_piece() + i));
	}
}


bool Game::playOneLevel() {

	int x_pos = 3; //initial x pos
	int y_pos = 0; //initial y pos

	Timer timer;
	timer.start();

	while (rows_left() != 0) {

		/* TAKES IN KEYSTROKE */
		char key_press;
		getCharIfAny(key_press);

		/* CALCULATES SCORE */
		int prev_rows_destroyed = m_rows_destroyed;
		destroyRow();	//modifies m_rows_detroyed

		switch (m_rows_destroyed - prev_rows_destroyed) {
			case(1): 
				m_score += 100;
				break;
			case(2):
				m_score += 200;
				break;
			case(3):
				m_score += 400;
				break;
			case(4):
				m_score += 800;
				break;
			case(5):
				m_score += 1600;
				break;
			default: //maybe should do something with this
				break;
		}

		/* CREATES A NEW PIECE IF THE CURRENT PIECE CAN'T MOVE DOWN ANY FURTHER */
		if (!canMove(DOWN, m_current_piece)) {
			m_current_piece = m_next_piece;
			Piece new_piece(chooseRandomPieceType());	//selects a new piece randomly 
			m_next_piece = new_piece;
			timer.start();
			x_pos = 3;
			y_pos = 0;
			key_press = 'x';
			displayStatus();
			continue;
		}
		
		/* RESTARTS THE TIMER BASED ON T = maximum(1000-(100*(L-1)), 100) */
		if (timer.elapsed() >= timeLeft(timer)) {
			erasePiece(m_current_piece, x_pos, y_pos);
			y_pos++;
			printPiece(m_current_piece, x_pos, y_pos);
			timer.start();
			displayStatus();
			continue;
		}

		/* PROCESSES KEYSTROKE */
		if (key_press == '4' || key_press == '6' || key_press == '2') {		//move the char left, right or down
			movePiece(m_current_piece, key_press, x_pos, y_pos); //move char LR, also modifies x_pos
			if (key_press == '2')	// restarts the timer if the piece is moved down
				timer.start();
			key_press = 'x'; //changes the key_press to a random char that does nothing
		}
		else if (key_press == ' ') {	//sends the piece to the bottom -- spacebar
			movePiece(m_current_piece, key_press, x_pos, y_pos);
		}
		else if (key_press == 'q' || key_press == 'Q') { //quit
			return false;
		}
		else if (key_press == '8') {	//up key
			rotatePiece(m_current_piece, x_pos, y_pos);
			key_press = 'x';
			
		}
		displayStatus();
	}

	reset();
    return true;
}


void Game::printPiece(Piece& piece, const int& x, const int& y) {
	for (int i = 0; i < 16; i++) {
		int p = i % 4;
		int q = i / 4;
		m_screen.gotoXY(x + p, q + y);

		if (*(piece.get_piece() + i) == '#') {
			m_screen.printChar(*(piece.get_piece() + i));
			m_well.set_well(*(piece.get_piece() + i), x + p, q + y);
		}
	}
}


void Game::erasePiece(Piece& piece, const int& x, const int& y) {

	int num_hash = 0;

	for (int i = 0; i < 16; i++) {
		if (num_hash == 4)
			break;

		int p = i % 4;
		int q = i / 4;
		m_screen.gotoXY(x + p, q + y);

		if (*(piece.get_piece() + i) == '#') {
			num_hash++;
			m_screen.printChar(' ');
			m_well.set_well(' ', x + p, q + y);
		}
	}
}


bool Game::rotatePiece(Piece& piece, const int& x, const int& y) {

	for (int i = 0; i < 16; i++) {

		int p = i % 4;
		int q = i / 4;

		piece.increment_orientation();

		if (*(piece.get_piece() + i) == '#' && (m_well.get_well(x + p, y + q) == '$' || 
			m_well.get_well(x + p, y + q) == '@' || m_well.get_well(x + p, y + q) == '*')) {
			piece.decrement_orientation();
			return false;
		}
		piece.decrement_orientation();
	}

	erasePiece(piece, x, y);
	piece.increment_orientation();
	printPiece(piece, x, y);

	return true;
}


void Game::pieceToRow(Piece& piece) {
	//if (piece.get_piece_type() == PIECE_VAPOR || piece.get_piece_type() == PIECE_FOAM)

	for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
		for (int j = 0; j < m_well.get_sizeY(); j++) {
			if (m_well.get_well(i, j) == '#') {
				m_screen.gotoXY(i, j);
				m_screen.printChar('$');
				m_well.set_well('$', i, j);
			}
		}
	}
}


bool Game::canMove(const m_direction& dir, Piece& piece) {
	/*Checks if the piece is capable of shifting downwards, also converts the piece to '$' if it can't
	move down any further*/
	if (dir == DOWN) {
		for (int j = 0; j < m_well.get_sizeY(); j++) {
			for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i, j + 1) == '@' || 
					m_well.get_well(i, j + 1) == '$' || m_well.get_well(i, j + 1) == '*')) {

					/* FOAM BOMB */
					if (piece.get_piece_type() == PIECE_FOAM) {
						m_well.set_well('*', i, j);
						m_screen.gotoXY(i, j);
						m_screen.printChar('*');
						foam_bomb(i, j);
					}

					else 
					{
						/* VAPOR BOMB */
						if (piece.get_piece_type() == PIECE_VAPOR) {
							for (int y = j; y < m_well.get_sizeY() - 1 && y - j < 3; y++) {
								m_well.set_well(' ', i, y);
								m_screen.gotoXY(i, y);
								m_screen.printChar(' ');

								m_well.set_well(' ', i + 1, y);
								m_screen.gotoXY(i + 1, y);
								m_screen.printChar(' ');
							}

							for (int y = j; y > 0 && j - y < 3; y--) {
								m_well.set_well(' ', i, y);
								m_screen.gotoXY(i, y);
								m_screen.printChar(' ');

								m_well.set_well(' ', i + 1, y);
								m_screen.gotoXY(i + 1, y);
								m_screen.printChar(' ');
							}
						}
						/* REGULAR PIECE - converts the piece from '#' to '$' */ 
						pieceToRow(piece);
					}
					return false;
				}
			}
		}
		return true;
	}

	/*
	if (dir == DOWN) {
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i, j + 1) == '@' || m_well.get_well(i, j + 1) == '$')) {
					if (piece.get_piece_type == PIECE_VAPOR) {

						return false;
					}
					else {
						pieceToRow(piece);
						return false;
					}
				}
			}
		}
		return true;
	}
	*/

	/* LEFT - STOPS PIECE WHEN IT CAN'T MOVE ANY FURTHER TO THE LEFT */
	else if (dir == LEFT) {
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i - 1, j) == '@' || m_well.get_well(i - 1, j) == '$') || m_well.get_well(i - 1, j) == '*') {
					return false;
				}
			}
		}
		return true;
	}

	/* RIGHT - STOPS PIECE WHEN IT CAN'T MOVE ANY FURTHER TO THE RIGHT*/
	else if (dir == RIGHT) {
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i + 1, j) == '@' || m_well.get_well(i + 1, j) == '$' || m_well.get_well(i + 1, j) == '*')) {
					return false;
				}
			}
		}
		return true;
	}

	/* SPACE - STOPS THE PIECE WHEN IT HITS THE BOTTOM OF THE WELL/TOP ROW */
	else if (dir == SPACE_DOWN) {
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i, j + 1) == '@' || m_well.get_well(i, j + 1) == '$' || m_well.get_well(i, j + 1) == '*')) {
					return false;
				}
			}
		}
		return true;
	}

	return false; //if none of the cases are false, return true - piece can move in that direction
}


void Game::movePiece(Piece& piece, const char& ch, int& x, int& y) {
	if (ch == '4' && canMove(LEFT, piece)) {//left case
		erasePiece(piece, x, y);
		printPiece(piece, --x, y);
	}
	else if (ch == '6' && canMove(RIGHT, piece)) {// right case
		erasePiece(piece, x, y);
		printPiece(piece, ++x, y);
	}
	else if (ch == '2' && canMove(DOWN, piece)) {
		erasePiece(piece, x, y);
		printPiece(piece, x, ++y);
	}
	else if (ch == ' ' && canMove(SPACE_DOWN, piece)) {
		erasePiece(piece, x, y);
		printPiece(piece, x, ++y);
	}
}


/* TIME LEFT, BASED ON : T = maximum(1000-(100*(L-1)), 100) */
double Game::timeLeft(Timer& timer) const {
	if (1000 - (100 * (m_level - 1)) < 100)
		return 100;
	else
		return 1000 - (100 * (m_level - 1));
}


bool Game::destroyRow() {

	for (int j = m_well.get_sizeY() - 1; j > 0; j--) {
		int num_dollar = 0;
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			if (m_well.get_well(i, j) == '$' || m_well.get_well(i, j) == '*') {
				num_dollar++;
			}
		}
		
		// Shift all blocks down by one
		if (num_dollar == 10) {
			m_rows_destroyed++;
			displayStatus();

			for (int y = j; y > 0; y--) {
				for (int x = 1; x < m_well.get_sizeX() - 1; x++) {
					m_well.set_well(m_well.get_well(x, y - 1), x, y);
					if (m_well.get_well(x, y - 1) == '$' || m_well.get_well(x, y - 1) == '*') {
						m_screen.gotoXY(x, y);
						m_screen.printChar(m_well.get_well(x, y - 1));
					}
					else if (m_well.get_well(x, y - 1) == ' ') {
						m_screen.gotoXY(x, y);
						m_screen.printChar(' ');
					}
				}
			}
			return true;
		}
	}
	
	return false;
}


int Game::rows_left() {
	m_rows_left = m_level * 5 - m_rows_destroyed;
	return m_rows_left;
}


void Game::reset() {
	m_rows_destroyed = 0;
	++m_level;

	m_well.reset();

	for (int x = 1; x < m_well.get_sizeX() - 1; x++) {
		for (int y = 0; y < m_well.get_sizeY() - 1; y++) {
			m_well.set_well(m_well.get_well(x, y), x, y);
			m_screen.gotoXY(x, y);
			m_screen.printChar(m_well.get_well(x, y));
		}
	}
}

bool Game::foam_bomb(const int& x, const int& y, int x_filledL, int x_filledR, int y_filledU, int y_filledD) {
	if (x_filledL == 0 && x_filledR == 0 && y_filledU == 0 && y_filledD == 0)
		return true;
	if (x_filledL < 0 || x_filledR < 0 || y_filledU < 0 || y_filledD < 0)
		return false;

	if (!m_well.set_well('*', x, y))
		return false;
	//m_screen.gotoXY(x, y);
	//m_screen.printChar('*')

	if (m_well.get_well(x + 1, y) == ' ' && x_filledR > 0 && foam_bomb(x + 1, y, x_filledL, --x_filledR, y_filledU, y_filledD)) {
		m_well.set_well('*', x + 1, y);
		m_screen.gotoXY(x + 1, y);
		m_screen.printChar('*');	
		//foam_bomb(x + 1, y, x_filledR--);
		return true;
	}
	if (m_well.get_well(x - 1, y) == ' ' && x_filledL > 0 && foam_bomb(x - 1, y, --x_filledL, x_filledR, y_filledU, y_filledD)) {
		m_well.set_well('*', x - 1, y);
		m_screen.gotoXY(x - 1, y);
		m_screen.printChar('*');
		//foam_bomb(x - 1, y, x_filledL--);
		return true;
	}
	if (m_well.get_well(x, y + 1) == ' ' && y_filledU > 0 && foam_bomb(x, y + 1, x_filledL, x_filledR, --y_filledU, y_filledD)) {
		m_well.set_well('*', x, y + 1);
		m_screen.gotoXY(x, y + 1);
		m_screen.printChar('*');
		//foam_bomb(x, y + 1, y_filledU--);
		return true;
	}
	if (m_well.get_well(x, y - 1) == ' ' && y_filledD > 0 && foam_bomb(x, y - 1, x_filledL, x_filledR, y_filledU, --y_filledD)) {
		m_well.set_well('*', x, y - 1);
		m_screen.gotoXY(x, y - 1);
		m_screen.printChar('*');
		//foam_bomb(x, y - 1, y_filledD--);
		return true;
	}

	return false;
	/*
	if (m_well.get_well(x, y) == ' ') {
		m_well.set_well('*', x, y);
		m_screen.gotoXY(x, y);
		m_screen.printChar('*');
	}
	*/
}