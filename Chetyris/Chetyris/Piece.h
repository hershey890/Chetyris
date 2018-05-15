#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

class Piece {
private:
	enum PieceType {
		PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
		PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
	};

	char PIECE_T_1[4][4] = { 
			{' ', '#', ' ', ' '},
			{'#', '#', '#', ' '},
			{' ', ' ', ' ', ' '}, 
			{' ', ' ', ' ', ' '}
	};

	char PIECE_T_2[]

public:
	Piece();
	PieceType chooseRandomPieceType();
};

#endif // PIECE_INCLUDED
