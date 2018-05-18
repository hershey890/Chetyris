#include "Piece.h"
#include <cassert>
Piece::Piece(const PieceType& piecetype): m_piece_type(piecetype), m_orientation(1) {}

int Piece::increment_orientation() {
	m_orientation++;
	if (m_orientation > 4)
		m_orientation = 1;
	return m_orientation;
}

int Piece::decrement_orientation() {
	m_orientation--;
	if (m_orientation < 1)
		m_orientation = 4;
	return m_orientation;
}

int Piece::get_orientation() const {
	return m_orientation;
}

char* Piece::get_piece()
{
	//PieceType piece = m_piece_type;

	assert(!(get_orientation() > 4 || get_orientation() < 1));
	//assert(!(get_orientation() == 3));

	switch (m_piece_type) { //used to hold piece
		case PIECE_I:
			switch (get_orientation()) {
				case 1:
					return m_piece_I_1;
				case 2:
					return m_piece_I_2;
				case 3:
					return m_piece_I_3;
				case 4:
					return m_piece_I_4;
			}
		case PIECE_L:
			switch (get_orientation()) {
				case 1:
					return m_piece_L_1;
				case 2:
					return m_piece_L_2;
				case 3:
					return m_piece_L_3;
				case 4:
					return m_piece_L_4;
			}
		case PIECE_J:
			switch (get_orientation()) {
				case 1:
					return m_piece_J_1;
				case 2:
					return m_piece_J_2;
				case 3:
					return m_piece_J_3;
				case 4:
					return m_piece_J_4;
			}
		case PIECE_T:
			switch (get_orientation()) {
				case 1:
					return m_piece_T_1;
				case 2:
					return m_piece_T_2;
				case 3:
					return m_piece_T_3;
				case 4:
					return m_piece_T_4;
			}
		case PIECE_O:
			switch (get_orientation()) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_O_1;
			}
		case PIECE_S:
			switch (get_orientation()) {
				case 1:
					return m_piece_S_1;
				case 2:
					return m_piece_S_2;
				case 3:
					return m_piece_S_3;
				case 4:
					return m_piece_S_4;
			}
		case PIECE_Z:
			switch (get_orientation()) {
				case 1:
					return m_piece_Z_1;
				case 2:
					return m_piece_Z_2;
				case 3:
					return m_piece_Z_3;
				case 4:
					return m_piece_Z_4;
			}
		case PIECE_VAPOR:
			switch (get_orientation()) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_vapor_1;
			}
		case PIECE_FOAM:
			switch (get_orientation()) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_foam_1;
			}
		case PIECE_CRAZY:
			switch (get_orientation()) {
				case 1:
					return m_piece_crazy_1;
				case 2:
					return m_piece_crazy_2;
				case 3:
					return m_piece_crazy_3;
				case 4:
					return m_piece_crazy_4;
			}
	}
	return nullptr;
}