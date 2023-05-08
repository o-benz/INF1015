#pragma once
#include <QApplication>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <span>
#include "include/cppitertools/range.hpp"
#include "include/cppitertools/enumerate.hpp"

namespace model {
constexpr int BOARD_DIMENSION_X = 8;
constexpr int BOARD_DIMENSION_Y = 8;
constexpr int NUMBER_OF_TILES = 64;
constexpr int MAXIMUM_KINGS_CONFRONTATION = 2;
constexpr int INITIAL_PIECE_POSITION = 0;
constexpr int ROOT_FACTOR = 2;
constexpr char EMPTY_CELL = 'X';
constexpr char BLACK_KING = 'k';
constexpr char WHITE_KING = 'K';
constexpr char BLACK_KNIGHT = 'n';
constexpr char WHITE_KNIGHT = 'N';
constexpr char BLACK_ROOK = 'r';
constexpr char WHITE_ROOK = 'R';
constexpr char SEPARATOR = '/';
constexpr char EMPTY_PIECE = 'x';
constexpr char EMPTY_LINE = '8';
constexpr char SPACE = ' ';
constexpr char START_WITH_BLACK = 'b';
const std::string KING_EXCEEDED_LIMIT = "Le nombre de rois dépasse le seuil autorisé.";
const std::string KING_BELOW_LIMIT = "Le nombre de rois est inférieur au seuil autorisé.";

class Board;
enum class Color { Black, White };

class Piece {
public:
    Piece(const Color&);

    void                                addMovement(const std::pair<int, int>&);
    void                                clearMovements();
    std::vector<std::pair<int, int>>    getPossibleMovements() const;
    Color                               getColor() const;
    bool                                isInsideBounds(const std::pair<int, int>&) const;
    void                                setColor(const Color&);
    virtual char                        getName() const = 0;
    virtual void                        calculatePossibleMovements(const std::pair<int, int>&, Board*) = 0;
    virtual void                        calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) = 0;
    virtual char                        getCharacter() const = 0;

    virtual	~Piece() = default;

private:
    Color                               color_;
    std::vector<std::pair<int, int>>    possibleMovements_;
};


class King : public Piece {
public:
    King(const Color&);

    bool isConfrontingAdversaryKing(const std::pair<int, int>&, const std::pair<int, int>&) const;
    void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
    void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
    char getName()      const override;
    char getCharacter() const override;

private:
    char pieceCharacter_;
};


class Knight : public Piece {
public:
    Knight(const Color&);

    void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
    void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
    char getName()      const override;
    char getCharacter() const override;

private:
    char pieceCharacter_;
};


class Rook : public Piece {
public:
    Rook(const Color&);

    void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
    void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
    char getName()      const override;
    char getCharacter() const override;

private:
    char caracterePiece_;
    bool verifyCheck_ = false;
};


class Tile
{
public:
    Tile(const std::string&, bool);

    void					setOccupyingPiece(std::unique_ptr<Piece>);
    std::string				getTileName()		const;
    bool					getOccupation()		const;
    Piece*					getOccupyingPiece() const;
    void					destroyOccupyingPiece();
    std::unique_ptr<Piece>	changePossessingPiece();
    void					invertOccupation();


private:
    std::string				name_;
    bool					isOccupied_;
    std::unique_ptr<Piece>	occupyingPiece_;
};

class Board
{
public:
    Board(const Color&);

    std::unique_ptr<Tile>					createTile(const std::string&, bool) const;
    std::unique_ptr<Piece>					createPiece(char) const;
    void									resetNumberOfKings();
    void									create(const std::string&);
    std::pair<Tile*, std::pair<int, int>>	findTile(const char, bool) const;
    Tile*									findTile(const std::pair<int, int>&) const;
    void									calculatePossibleMoves(Piece*, const std::pair<int, int>&);
    void									resetValidPiecePositions();
    void									invertTurn();
    void									setTurn(const Color&);
    Color									getTurn() const;
    void									movePiece(Tile*, Tile*);
    Tile*									getTile(const int, const int) const;
    void									invertCheck();
    bool									testCheckAfterMove(Tile*);
    bool									testCheckProtection(const std::pair<int, int>&, const std::pair<int, int>&);
    bool									getCheckState() const;
    std::pair<int, int>						getTilePosition(Tile*) const;
    bool									testCheckAfterOpponentMove(Tile* finalTile);
    bool									testUnprotectedCheck(const std::pair<int, int>&, const std::pair<int, int>& movement);
    bool									testCheckmate(const std::pair<int, int>&, const std::pair<int, int>&);

private:
    Color					turn_;
    bool					check_ = false;
    std::unique_ptr<Tile>	board_[BOARD_DIMENSION_X][BOARD_DIMENSION_Y];
    const char*				tileNames_[NUMBER_OF_TILES] = {
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

class CorrectNumberofKings : public std::invalid_argument
{
public:
    using invalid_argument::invalid_argument;
};
};
