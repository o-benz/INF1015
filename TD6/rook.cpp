#include "modele.h"

model::Rook::Rook(const Color& color) :
    Piece(color)
{
    if (color == Color::White)
        caracterePiece_ = BLACK_ROOK;

    else
        caracterePiece_ = BLACK_ROOK;
}

void model::Rook::calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board)
{
    using namespace std;

    const vector<pair<const int, const int>> POSSIBLE_MOVES_AX = {
        {initialPosition.first + 1, initialPosition.second},
        {initialPosition.first + 2, initialPosition.second},
        {initialPosition.first + 3, initialPosition.second},
        {initialPosition.first + 4, initialPosition.second},
        {initialPosition.first + 5, initialPosition.second},
        {initialPosition.first + 6, initialPosition.second},
        {initialPosition.first + 7, initialPosition.second}
    };

    const vector<pair<const int, const int>> POSSIBLE_MOVES_AY = {
        {initialPosition.first,	initialPosition.second + 1},
        {initialPosition.first,	initialPosition.second + 2},
        {initialPosition.first,	initialPosition.second + 3},
        {initialPosition.first,	initialPosition.second + 4},
        {initialPosition.first,	initialPosition.second + 5},
        {initialPosition.first,	initialPosition.second + 6},
        {initialPosition.first,	initialPosition.second + 7}
    };

    const vector<pair<const int, const int>> POSSIBLE_MOVES_BX = {
        {initialPosition.first - 1, initialPosition.second},
        {initialPosition.first - 2, initialPosition.second},
        {initialPosition.first - 3, initialPosition.second},
        {initialPosition.first - 4, initialPosition.second},
        {initialPosition.first - 5, initialPosition.second},
        {initialPosition.first - 6, initialPosition.second},
        {initialPosition.first - 7, initialPosition.second}
    };

    const vector<pair<const int, const int>> POSSIBLE_MOVES_BY = {
        {initialPosition.first,	initialPosition.second - 1},
        {initialPosition.first,	initialPosition.second - 2},
        {initialPosition.first,	initialPosition.second - 3},
        {initialPosition.first,	initialPosition.second - 4},
        {initialPosition.first,	initialPosition.second - 5},
        {initialPosition.first,	initialPosition.second - 6},
        {initialPosition.first,	initialPosition.second - 7}
    };

    const vector<vector<pair<const int, const int>>> POSSIBLE_MOVES = {
        POSSIBLE_MOVES_AX, POSSIBLE_MOVES_BX,
        POSSIBLE_MOVES_AY, POSSIBLE_MOVES_BY
    };

    for (const auto& vectorMouvement : POSSIBLE_MOVES)
        for (const auto& mouvement : vectorMouvement)
            if (isInsideBounds(mouvement)) {
                if (!board->getTile(mouvement.first, mouvement.second)->getOccupation()) {
                    if (board->getCheckState() == false) {
                        if (!board->testUnprotectedCheck(initialPosition, mouvement)) {
                            addMovement(mouvement);
                            continue;
                        }
                    }

                    else {
                        if (board->testCheckProtection(initialPosition, mouvement) == true) {
                            addMovement(mouvement);
                            continue;
                        }
                    }
                }
                else if (board->getTile(mouvement.first, mouvement.second)->getOccupyingPiece()->getColor() == board->getTurn())
                    break;
                else if (board->getTile(mouvement.first, mouvement.second)->getOccupyingPiece()->getColor() != board->getTurn()) {
                    if (board->getCheckState() == false) {
                        if (!board->testUnprotectedCheck(initialPosition, mouvement)) {
                            addMovement(mouvement);
                            break;
                        }
                    }

                    if (board->testUnprotectedCheck(initialPosition, mouvement) == true) {
                        addMovement(mouvement);
                        break;
                    }
                }
            }
}


/**
*  Calcule les mouvements possibles pour la Rook.
*  \param	[const pair<int, int>&]		initialPosition	<La position initiale de la Rook.>
*  \param	[board*]					board				<L'�chiquier.>
*/
void model::Rook::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board)
{
    using namespace std;

    const vector<pair<const int, const int>> MOUVEMENTS_POSSIBLES_AVANTS_X = {
        {initialPosition.first + 1, initialPosition.second},
        {initialPosition.first + 2, initialPosition.second},
        {initialPosition.first + 3, initialPosition.second},
        {initialPosition.first + 4, initialPosition.second},
        {initialPosition.first + 5, initialPosition.second},
        {initialPosition.first + 6, initialPosition.second},
        {initialPosition.first + 7, initialPosition.second}
    };

    const vector<pair<const int, const int>> MOUVEMENTS_POSSIBLES_AVANTS_Y = {
        {initialPosition.first , initialPosition.second + 1},
        {initialPosition.first , initialPosition.second + 2},
        {initialPosition.first , initialPosition.second + 3},
        {initialPosition.first , initialPosition.second + 4},
        {initialPosition.first , initialPosition.second + 5},
        {initialPosition.first , initialPosition.second + 6},
        {initialPosition.first , initialPosition.second + 7}
    };

    const vector<pair<const int, const int>> MOUVEMENTS_POSSIBLES_ARRIERES_X = {
        {initialPosition.first - 1, initialPosition.second},
        {initialPosition.first - 2, initialPosition.second},
        {initialPosition.first - 3, initialPosition.second},
        {initialPosition.first - 4, initialPosition.second},
        {initialPosition.first - 5, initialPosition.second},
        {initialPosition.first - 6, initialPosition.second},
        {initialPosition.first - 7, initialPosition.second}
    };

    const vector<pair<const int, const int>> MOUVEMENTS_POSSIBLES_ARRIERES_Y = {
        {initialPosition.first , initialPosition.second - 1},
        {initialPosition.first , initialPosition.second - 2},
        {initialPosition.first , initialPosition.second - 3},
        {initialPosition.first , initialPosition.second - 4},
        {initialPosition.first , initialPosition.second - 5},
        {initialPosition.first , initialPosition.second - 6},
        {initialPosition.first , initialPosition.second - 7}
    };

    const vector<vector<pair<const int, const int>>> MOUVEMENTS_POSSIBLES = {
        MOUVEMENTS_POSSIBLES_AVANTS_X, MOUVEMENTS_POSSIBLES_ARRIERES_X,
        MOUVEMENTS_POSSIBLES_AVANTS_Y, MOUVEMENTS_POSSIBLES_ARRIERES_Y
    };

    for (const auto vectorMouvement : MOUVEMENTS_POSSIBLES)
        for (auto mouvement : vectorMouvement)
            if (isInsideBounds(mouvement))
                if (!board->getTile(mouvement.first, mouvement.second)->getOccupation()) {
                    addMovement(mouvement);
                    continue;
                }
                else if (board->getTile(mouvement.first, mouvement.second)->getOccupyingPiece()->getColor() == board->getTurn())
                    break;

                else if (board->getTile(mouvement.first, mouvement.second)->getOccupyingPiece()->getColor() != board->getTurn()) {
                    addMovement(mouvement);
                    break;
                }
}


char model::Rook::getName() const
{
    return BLACK_ROOK;
}

char model::Rook::getCharacter() const
{
    return caracterePiece_;
}
