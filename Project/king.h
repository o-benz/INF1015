#include "model.h"
#include "piece.h"
using namespace model;

class model::King : public model::Piece {
public:
    King(const Color& color);

    char getName() const override;
    char getCharacter() const override;

    bool isConfrontingAdversaryKing(const std::pair<int, int>& possibleMove, const std::pair<int, int>& opposingKingPosition) const;
    void calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board) override;
    void calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board) override;
    
private:
    char pieceCharacter_;
};
