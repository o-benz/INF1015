#include "modele.h"

model::Tile::Tile(const std::string& name, bool isOccupied) :
    name_(name_), isOccupied_(isOccupied)
{}

void model::Tile::setOccupyingPiece(std::unique_ptr<Piece> newPiece)
{
    occupyingPiece_.reset();
    occupyingPiece_ = move(newPiece);
}

std::string model::Tile::getTileName() const
{
    return name_;
}

bool model::Tile::getOccupation() const
{
    return isOccupied_;
}

model::Piece* model::Tile::getOccupyingPiece() const
{
    return occupyingPiece_.get();
}


void model::Tile::destroyOccupyingPiece()
{
    occupyingPiece_.reset();
}

std::unique_ptr<model::Piece> model::Tile::changePossessingPiece()
{
    return move(occupyingPiece_);
}

void model::Tile::invertOccupation()
{
    if (isOccupied_)
        isOccupied_ = false;

    else
        isOccupied_ = true;
}

