#include "model.h"
using namespace model;

class model::Tile
{
public:
    Tile(const std::string& name, bool isOccupied);

    void setOccupyingPiece(std::unique_ptr<Piece>);
    std::string getTileName() const;
    bool getOccupation() const;
    Piece* getOccupyingPiece() const;

    void destroyOccupyingPiece();
    std::unique_ptr<Piece> changePossessingPiece();
    void invertOccupation();


private:
    std::string	name_;
    bool isOccupied_;
    std::unique_ptr<Piece> occupyingPiece_;
};
