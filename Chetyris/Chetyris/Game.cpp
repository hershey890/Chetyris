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

const int X_POS = 3;
const int Y_POS = 0;

Game::Game(int width, int height)
	: m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_well(width + 2, height + 1),
	m_rows_destroyed(0), m_rows_left(5), m_score(0),
	m_current_piece(chooseRandomPieceType()), m_next_piece(chooseRandomPieceType())
{}


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


void Game::displayNextPiece() {
	/* NEXT PIECE */
	m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
	m_screen.printString("Next Piece:");
	for (int i = 0; i < 16; i++) {
		int p = i % 4;
		int q = i / 4;
		m_screen.gotoXY(NEXT_PIECE_X + p, NEXT_PIECE_Y + q);
		m_screen.printChar(*(m_next_piece.get_piece() + i));
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

	}
}


void Game::displayPrompt(const std::string s) {
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
	m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    m_screen.refresh();
}


void Game::displayStatus() {
	/* SCORE */
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printString("Score:     ");
	m_screen.gotoXY(SCORE_X + 17, SCORE_Y);
	m_screen.printString(std::to_string(m_score));
	m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	
	/* ROWS LEFT */
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printString("Rows Left: ");
	m_screen.gotoXY(ROWS_LEFT_X + 17, ROWS_LEFT_Y);
	m_screen.printString(std::to_string(rows_left()));
	m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

	/* LEVEL */
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);	
	m_screen.printString("Level:     ");
	m_screen.gotoXY(LEVEL_X + 17, LEVEL_Y);
	m_screen.printString(std::to_string(m_level));
	m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
}


bool Game::playOneLevel() {

	int x_pos = 3; //initial x pos
	int y_pos = 0; //initial y pos

	Timer timer;
	timer.start();

	while (rows_left() != 0) {
		displayNextPiece();

		/* TAKES IN KEYSTROKE */
		char key_press= '_';
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

		/* RESTARTS THE TIMER BASED ON T = maximum(1000-(100*(L-1)), 100) */
		if (timer.elapsed() >= timeLeft(timer)) {
			erasePiece(m_current_piece, x_pos, y_pos);
			y_pos++;
			printPiece(m_current_piece, x_pos, y_pos);
			timer.start();
		}
		/*else if (!canMove(DOWN, m_current_piece) && timer.elapsed() < timeLeft(timer)) {
			while ()
			getCharIfAny(key_press);
			if (key_press == '4' || key_press == '6') {
				assert(false);
				movePiece(m_current_piece, key_press, x_pos, y_pos);
				
			}
		}*/
		/* CREATES A NEW PIECE IF THE CURRENT PIECE CAN'T MOVE DOWN ANY FURTHER */
		else if (!canMove(DOWN, m_current_piece)) {
			while (timer.elapsed() < timeLeft(timer)) {
				getCharIfAny(key_press);
				if (key_press == '2')
					break;
				if (key_press == '4' || key_press == '6') {
					movePiece(m_current_piece, key_press, x_pos, y_pos);
					key_press = 'x';
				}
			}

			nextPiece(timer, x_pos, y_pos, key_press);
		}
		else {
			/* PROCESSES KEYSTROKE */
			if (key_press == 'q' || key_press == 'Q' || !game_ended()) { //quit
				return false;
			}

			else if (key_press == '4' || key_press == '6' || key_press == '2') {		//move the char left, right or down
				if  (m_current_piece.get_piece_type() == PIECE_CRAZY) {
					(key_press == '4') ? key_press = '6' :
					(key_press == '6') ? key_press = '4' :
					(key_press == '2');
				}
				movePiece(m_current_piece, key_press, x_pos, y_pos); //move char LR, also modifies x_pos
				if (key_press == '2')	// restarts the timer if the piece is moved down
					timer.start();
				key_press = 'x'; //changes the key_press to a random char that does nothing
			}

			else if (key_press == ' ') {	//sends the piece to the bottom -- spacebar
				while (canMove(DOWN, m_current_piece)) {
					movePiece(m_current_piece, key_press, x_pos, y_pos);
				}
				nextPiece(timer, x_pos, y_pos, key_press);
				continue;
			}

			else if (key_press == '8') {	//up key
				rotatePiece(m_current_piece, x_pos, y_pos);
				key_press = 'x';
			}

			displayStatus();
		}
	}

	reset();
    return true;
}

void Game::nextPiece(Timer& timer, int& x_pos, int& y_pos, char& key_press) {
	timer.start();
	pieceToRow(m_current_piece);
	m_current_piece = m_next_piece;
	Piece new_piece(chooseRandomPieceType());	//selects a new piece randomly 
	m_next_piece = new_piece;
	timer.start();
	x_pos = 3;
	y_pos = 0;
	key_press = 'x';
}


void Game::printPiece(Piece& piece, const int& x, const int& y) {
	for (int i = 0; i < 16; i++) {
		int p = i % 4;
		int q = i / 4;

		if (*(piece.get_piece() + i) == '#' && m_well.set_well(*(piece.get_piece() + i), x + p, q + y)) {
			m_screen.gotoXY(x + p, q + y);
			m_screen.printChar(*(piece.get_piece() + i));
			m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

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

		if (*(piece.get_piece() + i) == '#' && m_well.set_well(' ', x + p, q + y)) {
			num_hash++;
			m_screen.gotoXY(x + p, q + y);
			m_screen.printChar(' ');

			m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		}
	}
}


bool Game::rotatePiece(Piece& piece, const int& x, const int& y) {

	for (int i = 0; i < 16; i++) {

		int p = i % 4;
		int q = i / 4;

		char well_contents;

		piece.increment_orientation();

		if (*(piece.get_piece() + i) == '#' && m_well.get_well(well_contents, x + p, y + q) && (
			well_contents == '$' || well_contents == '@' || well_contents == '*')) {
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

	char well_contents;

	for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
		for (int j = 0; j < m_well.get_sizeY(); j++) {
			if (m_well.get_well(well_contents, i, j) && well_contents == '#' && m_well.set_well('$', i, j)) {
				m_screen.gotoXY(i, j);
				m_screen.printChar('$');

				m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
			}
		}
	}
}


bool Game::canMove(const m_direction& dir, Piece& piece) {
	
	char well_contents;

	/* Checks if the piece is capable of shifting downwards, also converts the piece to '$' if it can't
	move down any further */
	if (dir == DOWN) {
		for (int j = 0; j < m_well.get_sizeY(); j++) {
			for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
				if (m_well.get_well(well_contents, i, j) && well_contents == '#' && (m_well.get_well(well_contents, i, j + 1) && (
					well_contents == '@' || well_contents == '$' || well_contents == '*'))) {

					/* FOAM BOMB */
					if (piece.get_piece_type() == PIECE_FOAM) {
						foam_bomb(i, j);
					}

					else {
						/* VAPOR BOMB */
						if (piece.get_piece_type() == PIECE_VAPOR) {
							vaporBomb();
							/*for (int y = j; y < m_well.get_sizeY() - 1 && y - j < 3; y++) {
								if (m_well.set_well(' ', i, y) && m_well.set_well(' ', i + 1, y)) {
									m_screen.gotoXY(i, y);	
									m_screen.printChar(' ');

									m_screen.gotoXY(i + 1, y);
									m_screen.printChar(' ');

									m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
								}
							}

							for (int y = j; y > 0 && j - y < 3; y--) {
								if (m_well.set_well(' ', i, y) && m_well.set_well(' ', i + 1, y)) {
									m_screen.gotoXY(i, y);
									m_screen.printChar(' ');

									m_screen.gotoXY(i + 1, y);
									m_screen.printChar(' ');

									m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
								}
							}*/
							return false;
						}

						/* REGULAR PIECE - converts the piece from '#' to '$' */ 
						//pieceToRow(piece);
					}

					return false;
				}
			}
		}
		return true;
	}

	/* LEFT - STOPS PIECE WHEN IT CAN'T MOVE ANY FURTHER TO THE LEFT */
 	else if (dir == LEFT) {
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			for (int j = 0; j < m_well.get_sizeY(); j++) {
				if (m_well.get_well(well_contents, i, j) && well_contents == '#' && (
					m_well.get_well(well_contents, i - 1, j) && (well_contents == '@' || well_contents == '$' || well_contents == '*'))) {
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
				if (m_well.get_well(well_contents, i, j) && well_contents == '#' && (
					m_well.get_well(well_contents, i + 1, j) && (well_contents == '@' || well_contents == '$' || well_contents == '*'))) {
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
				if (m_well.get_well(well_contents, i, j) && well_contents == '#' && (m_well.get_well(well_contents, i, j + 1) && (
					well_contents == '@' || well_contents == '$' || well_contents == '*'))) {
					return false;
				}
			}
		}
		return true;
	}

	return false; //if none of the cases are false, return true - piece can move in that direction
}


bool Game::vaporBomb() {
	int num_hash;
	char well_contents;

	if (m_current_piece.get_piece_type() == PIECE_VAPOR) {
		for (int j = 0; j < m_well.get_sizeY(); j++) {
			for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
				if (m_well.get_well(well_contents, i, j) && well_contents == '#') {

					for (int y = j; y < m_well.get_sizeY() - 1 && y - j < 3; y++) {
						if (m_well.set_well(' ', i, y) && m_well.set_well(' ', i + 1, y)) {
							m_screen.gotoXY(i, y);
							m_screen.printChar(' ');

							m_screen.gotoXY(i + 1, y);
							m_screen.printChar(' ');

							m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
						}
					}

					for (int y = j; y > 0 && j - y < 3; y--) {
						if (m_well.set_well(' ', i, y) && m_well.set_well(' ', i + 1, y)) {
							m_screen.gotoXY(i, y);
							m_screen.printChar(' ');

							m_screen.gotoXY(i + 1, y);
							m_screen.printChar(' ');

							m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
						}
					}

					return false;
				}
			}
		}
	}
	return true;
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

	char well_contents;

	for (int j = m_well.get_sizeY() - 1; j > 0; j--) {
		int num_dollar = 0;
		for (int i = 1; i < m_well.get_sizeX() - 1; i++) {
			if (m_well.get_well(well_contents, i, j) && (well_contents == '$' || well_contents == '*')) {
				num_dollar++;
			}
		}
		
		// Shift all blocks down by one
		if (num_dollar == 10) {
			m_rows_destroyed++;
			displayStatus();

			for (int y = j; y > 0; y--) {
				for (int x = 1; x < m_well.get_sizeX() - 1; x++) {
					m_well.get_well(well_contents, x, y - 1);
					m_well.set_well(well_contents, x, y);
					if (m_well.get_well(well_contents, x, y - 1)  && (well_contents == '$' || well_contents == '*')) {
						m_screen.gotoXY(x, y);
						m_screen.printChar(well_contents);
						m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
					}
					else if (m_well.get_well(well_contents, x, y - 1) && well_contents == ' ') {
						m_screen.gotoXY(x, y);
						m_screen.printChar(' ');

						m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
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
			char well_contents;
			if (m_well.get_well(well_contents, x, y)) {
				m_screen.gotoXY(x, y);
				m_screen.printChar(well_contents);

				m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
			}
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
	else {
		m_screen.gotoXY(x, y);
		m_screen.printChar('*');
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	}

	char well_contents1, well_contents2, well_contents3, well_contents4;

	if (m_well.get_well(well_contents1, x + 1, y) && well_contents1 == ' ' && x_filledR > 0 && m_well.set_well('*', x + 1, y) && foam_bomb(x + 1, y, x_filledL, --x_filledR, y_filledU, y_filledD)) {
		m_screen.gotoXY(x + 1, y);
		m_screen.printChar('*');	
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		return true;
	}
	if (m_well.get_well(well_contents2, x - 1, y) && well_contents2 == ' ' && x_filledL > 0 && m_well.set_well('*', x - 1, y) && foam_bomb(x - 1, y, --x_filledL, x_filledR, y_filledU, y_filledD)) {
		m_screen.gotoXY(x - 1, y);
		m_screen.printChar('*');
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		return true;
	}
	if (m_well.get_well(well_contents3, x, y + 1) && well_contents3 == ' ' && y_filledU > 0 && m_well.set_well('*', x, y + 1) && foam_bomb(x, y + 1, x_filledL, x_filledR, --y_filledU, y_filledD)) {
		m_screen.gotoXY(x, y + 1);
		m_screen.printChar('*');
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		return true;
	}
	if (m_well.get_well(well_contents4, x, y - 1) && well_contents4 == ' ' && y_filledD > 0 && m_well.set_well('*', x, y - 1) && foam_bomb(x, y - 1, x_filledL, x_filledR, y_filledU, --y_filledD)) {
		m_screen.gotoXY(x, y - 1);
		m_screen.printChar('*');
		m_screen.gotoXY(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		return true;
	}

	return false;
}


bool Game::game_ended() {
	for (int i = 0; i < 16; i++) {
		int p = i % 4;
		int q = i / 4;

		char well_contents;		


		if (*(m_current_piece.get_piece() + i) == '#' && (m_well.get_well(well_contents, X_POS + p, Y_POS + q) && well_contents == '$' || m_well.get_well(well_contents, X_POS + p, Y_POS + q) && well_contents == '*')) {
			return false;
		}
	}
	return true;
}