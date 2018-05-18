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
	m_rows_destroyed(0)
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
        m_level++;
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
	/*SCORE*/
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printString("Score:     ");
	m_screen.gotoXY(SCORE_X + 17, SCORE_Y);
	m_screen.printChar('0'); //TEMPORARY PLACEHOLDER
	
	/*ROWS LEFT*/
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printString("Rows Left: ");
	m_screen.gotoXY(ROWS_LEFT_X + 17, ROWS_LEFT_Y);
	m_screen.printChar('5'); //TEMPORARY PLACEHOLDER

	/*LEVEL*/
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);	
	m_screen.printString("Level:     ");
	m_screen.gotoXY(LEVEL_X + 17, LEVEL_Y);
	m_screen.printChar('1'); //TEMPORARY PLACEHOLDER
    // [Replace this with code to display the status.]
}

bool Game::playOneLevel()
{
	Piece piece(chooseRandomPieceType());

	int x_pos = 3; //initial x pos
	int y_pos = 0; //initial y pos

	Timer t;
	t.start();

	while (1) { //change to stop when the well fills up

		/*takes in keystroke*/
		char key_press;
		getCharIfAny(key_press);

		destroyRow();

		if (!canMove(DOWN, piece)) { /*restarts the loop essentially if piece can't move down further*/
			Piece piece1(chooseRandomPieceType()); //selects a new piece randomly 
			piece = piece1;
			t.start();
			x_pos = 3;
			y_pos = 0;
			key_press = 'x';
			continue;
		}
		
		if (t.elapsed() >= timeLeft(t, m_level)) {
			erasePiece(piece, x_pos, y_pos);
			y_pos++;
			printPiece(piece, x_pos, y_pos);
			t.start();
			continue;
		}

		/*processes keystroke input*/
		if (key_press == '4' || key_press == '6' || key_press == '2') {//move the char left or right
			movePiece(piece, key_press, x_pos, y_pos); //move char LR, also modifies x_pos
			if (key_press == '2')
				t.start();
			key_press = 'x'; //changes the key_press to a random char that does nothing
			continue;
		}
		else if (key_press == ' ') { //sends the piece to the bottom -- spacebar
			movePiece(piece, key_press, x_pos, y_pos);
			continue;
		}
		else if (key_press == 'q' || key_press == 'Q') { //quit
			break;
		}
		else if (key_press == '8') { //up key
			rotatePiece(piece, x_pos, y_pos);
			key_press = 'x';
			continue;
		}

		//discardPendingKeys();
	}

    return false;
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

		////orientation in the parameter here is incorrect
		if (*(piece.get_piece() + i) == '#' && (m_well.get_well(x + p, y + q) == '$' || m_well.get_well(x + p, y + q) == '@')) { //change orientation number later
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
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i, j + 1) == '@' || m_well.get_well(i, j + 1) == '$')) {
					pieceToRow(piece); /*converts the piece to '$'*/
					return false;
				}
			}
		}
		return true;
	}

	else if (dir == LEFT) { //Checks to see if the piece is capable of moving to the left
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i - 1, j) == '@' || m_well.get_well(i - 1, j) == '$')) {
					return false;
				}
			}
		}
		return true;
	}

	else if (dir == RIGHT) { //Checks to see if the piece is capable of moving to the right
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i + 1, j) == '@' || m_well.get_well(i + 1, j) == '$')) {
					return false;
				}
			}
		}
		return true;
	}

	else if (dir == SPACE_DOWN) { /*CHECKS IF THE PIECE IS CAPABLE OF SHIFTING ALL THE WAY DOWNWARDS
								probably incorrect, need to account for moving all the way
								down, not just 1 space down*/
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(i, j) == '#' && (m_well.get_well(i, j + 1) == '@' || m_well.get_well(i, j + 1) == '$')) {
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

//T = maximum(1000-(100*(L-1)), 100)
double Game::timeLeft(Timer& timer, const int level) const {
	if (1000 - (100 * (level - 1)) < 100)
		return 100;
	else
		return 1000 - (100 * (level - 1));
}

bool Game::destroyRow() {

	for (int j = m_well.get_sizeY() - 1; j > 0; j--) {
		int num_dollar = 0;
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			if (m_well.get_well(i, j) == '$') {
				num_dollar++;
			}
		}
		
		// Shift all blocks down by one
		if (num_dollar == 10) {
			for (int y = j; y > 0; y--) {
				for (int x = 1; x < m_well.get_sizeX() - 1; x++) {
					m_well.set_well(m_well.get_well(x, y - 1), x, y);
					if (m_well.get_well(x, y - 1) == '$') {
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