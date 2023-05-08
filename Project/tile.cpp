#include "model.h"
#include "tile.h"
using namespace model;

model::Tile::Tile(const std::string& name, bool isOccupied) : name_(name), isOccupied_(isOccupied) {}

void Tile::setOccupyingPiece(std::unique_ptr<Piece> newPiece) {
    occupyingPiece_.reset();
    occupyingPiece_ = move(newPiece);
}

std::string Tile::getTileName() const {
    return name_;
}

bool Tile::getOccupation() const {
    return isOccupied_;
}

Piece* Tile::getOccupyingPiece() const {
    return occupyingPiece_.get();
}


void Tile::destroyOccupyingPiece() {
    occupyingPiece_.reset();
}

std::unique_ptr<Piece> Tile::changePossessingPiece() {
    return move(occupyingPiece_);
}

void Tile::invertOccupation() {
    if (isOccupied_)
        isOccupied_ = false;

    else
        isOccupied_ = true;
}

