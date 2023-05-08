#include "modele.h"

model::Piece::Piece(const Color& color) :
    color_(color)
{}

void model::Piece::addMovement(const std::pair<int, int>& newMovement)
{
    possibleMovements_.push_back(newMovement);
}

void model::Piece::clearMovements()
{
    possibleMovements_.clear();
}

std::vector<std::pair<int, int>> model::Piece::getPossibleMovements() const
{
    return possibleMovements_;
}

model::Color model::Piece::getColor() const
{
    return color_;
}

bool model::Piece::isInsideBounds(const std::pair<int, int>&) const
{
    //demandez à Alex
    if (possibleMovements_[0].first >= 0 && possibleMovements_[0].first <= (BOARD_DIMENSION_X - 1) && possibleMovements_[0].second >= 0 && possibleMovements_[0].second <= (BOARD_DIMENSION_Y - 1))
        return true;
    return false;

}

void model::Piece::setColor(const Color& color)
{
    color_ = color;
}
