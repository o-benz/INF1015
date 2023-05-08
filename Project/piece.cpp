#include "model.h"
#include "piece.h"
using namespace model;

Piece::Piece(const Color& color) : color_(color) {}

void Piece::addMovement(const std::pair<int, int>& newMovement) {
    possibleMovements_.push_back(newMovement);
}

void Piece::clearMovements() {
    possibleMovements_.clear();
}

std::vector<std::pair<int, int>> Piece::getPossibleMovements() const {
    return possibleMovements_;
}

Color Piece::getColor() const {
    return color_;
}

bool Piece::isInsideBounds(const std::pair<int, int>&) const {
    if (possibleMovements_[0].first >= 0 && possibleMovements_[0].first <= (BOARD_DIMENSION_X - 1) && possibleMovements_[0].second >= 0 && possibleMovements_[0].second <= (BOARD_DIMENSION_Y - 1))
        return true;
    return false;
}

void Piece::setColor(const Color& color) {
    color_ = color;
}
