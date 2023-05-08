#include "model.h"
#include "piece.h"
using namespace model;

class model::Knight : public model::Piece
{
public:
    Knight(const Color& color);

    virtual char getName() const override;
    virtual char getCharacter() const override;

    virtual void calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board) override;
    virtual void calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board) override;

private:
    char pieceCharacter_;
};
