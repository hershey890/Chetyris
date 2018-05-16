#include "Piece.h"

Piece::Piece(PieceType piecetype): m_piece_type(piecetype) {}

char* Piece::get_piece(const int& orientation)
{
	PieceType piece = m_piece_type;

	switch (piece) {
		case PIECE_I:
			switch (orientation) {
				case 1:
					return m_piece_I_1;
				case 2:
					return m_piece_I_1;
				case 3:
					return m_piece_I_1;
				case 4:
					return m_piece_I_1;
			}
		case PIECE_L:
			switch (orientation) {
				case 1:
					return m_piece_L_1;
				case 2:
					return m_piece_L_1;
				case 3:
					return m_piece_L_1;
				case 4:
					return m_piece_L_1;
			}
		case PIECE_J:
			switch (orientation) {
				case 1:
					return m_piece_J_1;
				case 2:
					return m_piece_J_1;
				case 3:
					return m_piece_J_1;
				case 4:
					return m_piece_J_1;
			}
		case PIECE_T:
			switch (orientation) {
				case 1:
					return m_piece_T_1;
				case 2:
					return m_piece_T_1;
				case 3:
					return m_piece_T_1;
				case 4:
					return m_piece_T_1;
			}
		case PIECE_O:
			switch (orientation) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_O_1;
			}
		case PIECE_S:
			switch (orientation) {
				case 1:
					return m_piece_S_1;
				case 2:
					return m_piece_S_1;
				case 3:
					return m_piece_S_1;
				case 4:
					return m_piece_S_1;
			}
		case PIECE_Z:
			switch (orientation) {
				case 1:
					return m_piece_Z_1;
				case 2:
					return m_piece_Z_1;
				case 3:
					return m_piece_Z_1;
				case 4:
					return m_piece_Z_1;
			}
		case PIECE_VAPOR:
			switch (orientation) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_vapor_1;
			}
		case PIECE_FOAM:
			switch (orientation) {
				case 1:
				case 2:
				case 3:
				case 4:
					return m_piece_foam_1;
			}
		case PIECE_CRAZY:
			switch (orientation) {
				case 1:
					return m_piece_crazy_1;
				case 2:
					return m_piece_crazy_1;
				case 3:
					return m_piece_crazy_1;
				case 4:
					return m_piece_crazy_1;
			}
		default:
			break;
	}
	return nullptr;
}