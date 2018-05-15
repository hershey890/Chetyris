#include "Game.h"
#include "Well.h"
#include "UserInterface.h"
#include <string>

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
 : m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_well(width, height)
{
}

void Game::play()
{
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]

    for(;;)
    {
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
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printString("Score:     ");
	m_screen.gotoXY(SCORE_X + 17, SCORE_Y);
	m_screen.printChar('0'); //TEMPORARY PLACEHOLDER
	
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printString("Rows Left: ");
	m_screen.gotoXY(ROWS_LEFT_X + 17, ROWS_LEFT_Y);
	m_screen.printChar('5'); //TEMPORARY PLACEHOLDER

	m_screen.gotoXY(LEVEL_X, LEVEL_Y);	
	m_screen.printString("Level:     ");
	m_screen.gotoXY(LEVEL_X + 17, LEVEL_Y);
	m_screen.printChar('1'); //TEMPORARY PLACEHOLDER, CHANGE TO 1
    // [Replace this with code to display the status.]
}

bool Game::playOneLevel()
{
    return false;  // [Replace this with the code to play a level.]
}