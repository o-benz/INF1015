#include "model.h"
using namespace model;

class model::Board
{
public:
    Board(const Color&);

    std::unique_ptr<Tile> createTile(const std::string& name, bool isOccupied) const;
    std::unique_ptr<Piece> createPiece(char character) const;
    void resetNumberOfKings();
    void create(const std::string& configuration);
    std::pair<Tile*, std::pair<int, int>> findTile(const char pieceName, bool adv) const;
    Tile* findTile(const std::pair<int, int>& position) const;
    void calculatePossibleMoves(Piece* piece, const std::pair<int, int>& initialPosition);
    void resetValidPiecePositions();
    
    void switchTurn();
    void setTurn(const Color& color);
    Color getTurn() const;
    
    void movePiece(Tile* initialTile, Tile* finalTile);
    Tile* getTile(const int x, const int y) const;
    void invertCheck();

    bool testCheckAfterMove(Tile* finalTile);
    bool testCheckProtection(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement);
    bool getCheckState() const;
    std::pair<int, int>	getTilePosition(Tile* tile) const;
    bool testCheckAfterOpponentMove(Tile* finalTile);
    bool testUnprotectedCheck(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement);
    bool testCheckmate(const std::pair<int, int>& initialPosition, const std::pair<int, int>& movement);

private:
    Color turn_;
    bool check_ = false;
    std::unique_ptr<Tile> board_[BOARD_DIMENSION_X][BOARD_DIMENSION_Y];
    const char*	tileNames_[NUMBER_OF_TILES] = {
        "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
        "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
        "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
        "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
        "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
        "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
        "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
        "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"
    };
    inline static int       nKings_ = 0;
};
