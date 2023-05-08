#include "model.h"
#include "piece.h"
using namespace model;

class model::Rook : public model::Piece {
public:
    Rook(const Color& color);

    char getName() const override;
    char getCharacter() const override;

    void calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board) override;
    void calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board) override;
    
private:
    char caracterePiece_;
    bool verifyCheck_ = false;
};
