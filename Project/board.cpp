#include "model.h"
#include "board.h"
#include "tile.h"
#include "rook.h"
#include "knight.h"
#include "king.h"
using namespace model;

Board::Board(const Color& turn) : turn_(turn) {}



std::unique_ptr<Tile> Board::createTile(const std::string& name, bool isOccupied) const {
    return std::make_unique<Tile>(name, isOccupied);
}



std::unique_ptr<Piece> Board::createPiece(char character) const
{
    using namespace std;

    if (character == BLACK_KNIGHT)
        return make_unique<Knight>(Color::Black);

    else if (character == WHITE_KNIGHT)
        return make_unique<Knight>(Color::White);

    else if (character == BLACK_ROOK)
        return make_unique<Rook>(Color::Black);

    else if (character == WHITE_ROOK)
        return make_unique<Rook>(Color::White);

    else if (character == BLACK_KING) {
        ++nKings_;
        return make_unique<King>(Color::Black);
    }

    else {
        ++nKings_;
        return make_unique<King>(Color::White);
    }
}

void Board::resetNumberOfKings() {
    nKings_ = 0;
}

void Board::create(const std::string& configuration) {
    using namespace iter;

    int x = 0, y = 0;

    for (size_t i : range(configuration.length())) {
        if (nKings_ > MAXIMUM_KINGS_CONFRONTATION)
            throw CorrectNumberofKings(KING_EXCEEDED_LIMIT);

        if (configuration[i] == SPACE) {
            if (configuration[i + 1] == START_WITH_BLACK)
                switchTurn();
            break;
        }

        else if (configuration[i] != SEPARATOR) {
            if (configuration[i] == EMPTY_LINE)
                for ([[maybe_unused]] int k : range(BOARD_DIMENSION_X)) {
                    board_[x][y] = move(createTile(tileNames_[BOARD_DIMENSION_X * y + x], false));
                    ++x;
                }

            else if (configuration[i] == EMPTY_PIECE) {
                board_[x][y] = move(createTile(tileNames_[BOARD_DIMENSION_X * y + x], false));
                ++x;
            }

            else {
                board_[x][y] = move(createTile(tileNames_[BOARD_DIMENSION_X * y + x], true));
                board_[x][y]->setOccupyingPiece(move(createPiece(configuration[i])));
                ++x;
            }

            if ((x + 1) % (BOARD_DIMENSION_X + 1) == 0) {
                x = 0;
                ++y;
            }
        }
    }

    if (nKings_ < MAXIMUM_KINGS_CONFRONTATION)
        throw CorrectNumberofKings(KING_BELOW_LIMIT);
}


std::pair<Tile*, std::pair<int, int>> Board::findTile(const char pieceName, bool adv) const {
    using namespace iter;

    for (int y : range(BOARD_DIMENSION_Y))
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x]->getOccupation() && board_[y][x]->getOccupyingPiece()->getName() == static_cast<char>(tolower(pieceName)))
                if (adv && board_[y][x]->getOccupyingPiece()->getColor() != turn_)
                    return { board_[y][x].get(), { y, x } };

    return { nullptr, { 0,0 } };
}


Tile* Board::findTile(const std::pair<int, int>& position) const {
    return board_[position.first][position.second].get();
}


void Board::calculatePossibleMoves(Piece* piece, const std::pair<int, int>& initialPosition) {
    piece->clearMovements();
    piece->calculatePossibleMovements(initialPosition, this);
}

void Board::resetValidPiecePositions() {
    using namespace iter;

    for (int y : range(BOARD_DIMENSION_Y))
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x]->getOccupation())
                if (board_[y][x]->getOccupyingPiece()->getColor() == turn_)
                    calculatePossibleMoves(board_[y][x]->getOccupyingPiece(), { y, x });
}

void Board::switchTurn() {
    if (turn_ == Color::White)
        turn_ = Color::Black;
    else
        turn_ = Color::White;
}

void Board::setTurn(const Color& color) {
    turn_ = color;
}


Color Board::getTurn() const {
    return turn_;
}

std::pair<int, int> Board::getTilePosition(Tile* tile) const {
    using namespace iter;

    for (int y : range(BOARD_DIMENSION_Y))
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x].get() == tile)
                return { y,x };

    return { 0, 0 };
}



bool Board::testCheckAfterMove(Tile* finalTile) {
    using namespace std;

    finalTile->getOccupyingPiece()->clearMovements();
    finalTile->getOccupyingPiece()->calculatePossibleSimpleMovements(getTilePosition(finalTile), this);

    const vector<pair<int, int>> MOUVEMENTS_POSSIBLES = finalTile->getOccupyingPiece()->getPossibleMovements();

    for (auto movement : MOUVEMENTS_POSSIBLES)
        if (findTile(movement)->getOccupation() && findTile(movement)->getOccupyingPiece()->getName() == BLACK_KING && findTile(movement)->getOccupyingPiece()->getColor() != turn_)
            return true;

    return false;
}

bool Board::testCheckAfterOpponentMove(Tile* finalTile) {
    using namespace std;

    switchTurn();
    finalTile->getOccupyingPiece()->clearMovements();
    finalTile->getOccupyingPiece()->calculatePossibleSimpleMovements(getTilePosition(finalTile), this);

    const vector<pair<int, int>> MOUVEMENTS_POSSIBLES = finalTile->getOccupyingPiece()->getPossibleMovements();

    for (auto movement : MOUVEMENTS_POSSIBLES)
        if (findTile(movement)->getOccupation() && findTile(movement)->getOccupyingPiece()->getName() == BLACK_KING && findTile(movement)->getOccupyingPiece()->getColor() != turn_) {
            switchTurn();
            return true;
        }

    switchTurn();

    return false;
}


void Board::invertCheck() {
    if (check_ == true)
        check_ = false;

    else
        check_ = true;
}



bool Board::getCheckState() const {
    return check_;
}



void Board::movePiece(Tile* initialTile, Tile* finalTile) {
    if (finalTile->getOccupation())
        finalTile->destroyOccupyingPiece();

    else
        finalTile->invertOccupation();

    finalTile->setOccupyingPiece(move(initialTile->changePossessingPiece()));

    initialTile->destroyOccupyingPiece();
    initialTile->invertOccupation();
    finalTile->getOccupyingPiece()->setColor(turn_);

    if (testCheckAfterMove(finalTile))
        check_ = true;

    else
        check_ = false;

    switchTurn();
}

Tile* Board::getTile(const int x, const int y) const {
    return board_[x][y].get();
}


bool Board::testCheckProtection(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement) {
    using namespace iter;

    Tile* posTile = findTile(movement);
    Tile* initialTile = findTile(initialPosition);

    if (posTile->getOccupation() && posTile->getOccupyingPiece()->getColor() != turn_)
        return true;

    else if (posTile->getOccupation() && posTile->getOccupyingPiece()->getColor() == turn_)
        return false;

    posTile->setOccupyingPiece(initialTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();

    bool protection = true;

    invertCheck();

    for (int y : range(BOARD_DIMENSION_Y)) {
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x]->getOccupation() && board_[y][x]->getOccupyingPiece()->getColor() != turn_ && board_[y][x]->getOccupyingPiece()->getName() != BLACK_KING)
                if (testCheckAfterOpponentMove(board_[y][x].get())) {
                    protection = false;
                    break;
                }

        if (!protection)
            break;
    }

    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();
    invertCheck();

    return protection;
}

bool Board::testUnprotectedCheck(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement) {
    using namespace std;
    using namespace iter;

    Tile* initialTile = findTile(initialPosition);
    Tile* posTile = findTile(movement);
    bool check = false;
    bool attackingPiece = false;
    unique_ptr<Piece> attackingPieceTemp;

    if (posTile->getOccupation()) {
        attackingPieceTemp = posTile->changePossessingPiece();
        attackingPiece = true;
    }

    posTile->setOccupyingPiece(initialTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();

    for (int y : range(BOARD_DIMENSION_Y)) {
        for (int x : range(BOARD_DIMENSION_X)) {
            if (board_[y][x]->getOccupation() && board_[y][x]->getOccupyingPiece()->getColor() != turn_ && board_[y][x]->getOccupyingPiece()->getName() != BLACK_KING) {
                if (testCheckAfterOpponentMove(board_[y][x].get())) {
                    check = true;
                    break;
                }
            }
        }

        if (check)
            break;
    }

    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();

    if (attackingPiece)
        posTile->setOccupyingPiece(move(attackingPieceTemp));

    return check;
}


bool Board::testCheckmate(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement) {
    using namespace iter;

    Tile* posTile = findTile(movement);
    Tile* initialTile = findTile(initialPosition);

    if (posTile->getOccupation() && posTile->getOccupyingPiece()->getColor() != turn_)
        return true;

    else if (posTile->getOccupation() && posTile->getOccupyingPiece()->getColor() == turn_)
        return false;

    posTile->setOccupyingPiece(initialTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();

    bool protection = true;

    for (int y : range(BOARD_DIMENSION_Y))
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x]->getOccupation() && board_[y][x]->getOccupyingPiece()->getColor() != turn_ && board_[y][x]->getOccupyingPiece()->getName() != BLACK_KING)
                if (testCheckAfterOpponentMove(board_[y][x].get())) {
                    protection = false;
                    break;
                }

    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
    posTile->invertOccupation();
    initialTile->invertOccupation();

    return protection;
}