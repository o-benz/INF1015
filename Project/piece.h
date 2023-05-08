#include "model.h"
using namespace model;

class model::Piece {
public:
    Piece(const Color& color);

    void addMovement(const std::pair<int, int>&);
    void clearMovements();
    std::vector<std::pair<int, int>> getPossibleMovements() const;

    Color getColor() const;
    void setColor(const Color&);

    bool isInsideBounds(const std::pair<int, int>&) const;
    
    virtual char getName() const = 0;
    virtual void calculatePossibleMovements(const std::pair<int, int>&, Board*) = 0;
    virtual void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) = 0;
    virtual char getCharacter() const = 0;

    virtual	~Piece() = default;

private:
    Color color_;
    std::vector<std::pair<int, int>> possibleMovements_;
};
