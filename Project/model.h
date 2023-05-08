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
const std::string KING_EXCEEDED_LIMIT = "The number of kings exceeds the allowed limit.";
const std::string KING_BELOW_LIMIT = "The number of kings is below the allowed limit.";

class Board;
enum class Color { Black, White };
class Piece;
class Tile;
class King;
class Knight;
class Rook;

class CorrectNumberofKings : public std::invalid_argument {
public:
    using invalid_argument::invalid_argument;
};

};
