#include "model.h"
#include "king.h"
#include "board.h"
#include "tile.h"
//#include "piece.h"
using namespace model;

model::King::King(const Color& color) : Piece(color) {
    if (color == Color::White)
        pieceCharacter_ = WHITE_KING;
    else
        pieceCharacter_ = BLACK_KING;
}

char King::getName() const {
    return BLACK_KING;
}

char King::getCharacter() const {
    return pieceCharacter_;
}

char King::getName() const {
    return BLACK_KING;
}

char King::getCharacter() const {
    return pieceCharacter_;
}

bool King::isConfrontingAdversaryKing(const std::pair<int, int>& possibleMove, const std::pair<int, int>& opposingKingPosition) const {
    if (sqrt(pow(abs(possibleMove.first - opposingKingPosition.first), ROOT_FACTOR) + pow(abs(possibleMove.second - opposingKingPosition.second), ROOT_FACTOR)) >= MAXIMUM_KINGS_CONFRONTATION)
        return false;
    return true;
}

void King::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board) {
    using namespace std;
    const vector<pair<const int, const int>> POSSIBLE_MOVES = {
        { initialPosition.first + 1,    initialPosition.second },
        { initialPosition.first - 1,    initialPosition.second },
        { initialPosition.first,        initialPosition.second + 1 },
        { initialPosition.first,        initialPosition.second - 1 },
        { initialPosition.first + 1,    initialPosition.second + 1 },
        { initialPosition.first + 1,    initialPosition.second - 1 },
        { initialPosition.first - 1,    initialPosition.second + 1 },
        { initialPosition.first - 1,    initialPosition.second - 1 }
    };

    for (const pair<int, int>& move : POSSIBLE_MOVES) {
        const pair<Tile*, pair<int, int>> OPPOSING_KING = board->findTile(BLACK_KING, true);

        if (!isConfrontingAdversaryKing(move, OPPOSING_KING.second)) {
            if (isInsideBounds(move)) {
                if (board->getCheckState() == false) {
                    if (board->testCheckmate(initialPosition, move)) {
                        if (board->findTile(move)->getOccupation()) {
                            if (board->findTile(move)->getOccupyingPiece()->getColor() != board->getTurn()) {
                                addMovement(move);
                            }
                        }

                        else 
                            addMovement(move);
                    }
                }

                else {
                    if (board->testCheckProtection(initialPosition, move)) 
                        addMovement(move);
                }
            }
        }
    }
}

void King::calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board) {
    calculatePossibleSimpleMovements(initialPosition, board);
}
