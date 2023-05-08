#include "board.h"
#include "knight.h"
#include "model.h"
#include "tile.h"

using namespace model;

Knight::Knight(const Color& color) : Piece(color) {
    if (color == Color::White)
        pieceCharacter_ = WHITE_KNIGHT;
    else
        pieceCharacter_ = BLACK_KNIGHT;
}

char Knight::getName() const {
    return BLACK_KNIGHT;
}

char Knight::getCharacter() const {
    return pieceCharacter_;
}

void Knight::calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board) {
    const std::vector<std::pair<const int, const int>> POSSIBLE_MOVES = {
        {initialPosition.first + 2, initialPosition.second + 1},
        {initialPosition.first + 2, initialPosition.second - 1},
        {initialPosition.first - 2, initialPosition.second + 1},
        {initialPosition.first - 2, initialPosition.second - 1},
        {initialPosition.first + 1, initialPosition.second + 2},
        {initialPosition.first - 1, initialPosition.second + 2},
        {initialPosition.first + 1, initialPosition.second - 2},
        {initialPosition.first - 1, initialPosition.second - 2}
    };

    for (const std::pair<const int, const int>& movement : POSSIBLE_MOVES) {
        if (isInsideBounds(movement)) {
            if (!board->findTile(movement)->getOccupation() || board->findTile(movement)->getOccupyingPiece()->getColor() != board->getTurn()) {
                if (board->getCheckState() == false) {
                    if (!board->testUnprotectedCheck(initialPosition, movement))
                        addMovement(movement);
                }
                else {
                    if (board->testCheckProtection(initialPosition, movement))
                        addMovement(movement);
                }
            }
        }
    }
}

void Knight::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board) {
    const std::vector<std::pair<const int, const int>> POSSIBLE_MOVES = {
        {initialPosition.first + 2, initialPosition.second + 1},
        {initialPosition.first + 2, initialPosition.second - 1},
        {initialPosition.first - 2, initialPosition.second + 1},
        {initialPosition.first - 2, initialPosition.second - 1},
        {initialPosition.first + 1, initialPosition.second + 2},
        {initialPosition.first - 1, initialPosition.second + 2},
        {initialPosition.first + 1, initialPosition.second - 2},
        {initialPosition.first - 1, initialPosition.second - 2}
    };

    for (const std::pair<const int, const int>& movement : POSSIBLE_MOVES) {
        if (isInsideBounds(movement)) {
            if (board->findTile(movement)->getOccupation() == false || board->findTile(movement)->getOccupyingPiece()->getColor() != board->getTurn())
                addMovement(movement);
        }
    }
}

