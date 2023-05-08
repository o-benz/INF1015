 #pragma once
#include "model.h"
#include "board.h"
#pragma warning(push, 0)
#include <QApplication>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QGraphicsGridLayout>
#include <QPalette>
#include <QMessageBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVariant>
#include <QGridLayout>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QComboBox>
#pragma pop()
#include <iostream>
#include <type_traits>
#include "include/cppitertools/range.hpp"


#define BLACK_PALETTE_SELECTION "border: 7px groove rgb(40, 26, 13); background-color: rgb(141, 96, 59); font-size: 12px;"
#define WHITE_PALETTE_SELECTION "border: 7px groove rgb(40, 26, 13); background-color: rgb(200, 177, 141); font-size: 12px;"
#define	TILE_NAMES "font-size: 12px; vertical-align: center; text-align: center;"
#define WHITE_BUFFER_STYLE "border: none; background-color: rgb(240, 217, 181); font-size: 12px;"
#define BLACK_BUFFER_STYLE "border: none; background-color: rgb(181, 136, 99); font-size: 12px;"
#define WHITE_LABEL "margin: 5%; font-weight: bold; font-size: 24px; qproperty-alignment: AlignCenter; background: white; color: black; border: none;"
#define BLACK_LABEL "margin: 5%; font-weight: bold; font-size: 24px; qproperty-alignment: AlignCenter; background: black; color: white; border: none;"
#define WHITE_TEXT "White's turn !"
#define BLACK_TEXT "Black's turn !"


namespace view {
const QString CHESS_TITLE = "Echec";
const QString ERROR_TITLE = "Erreur";
const QString ERROR_MESSAGE = "Invalide.";
const QString BLACK_KING_PATH = "img/BlackKing.png";
const QString WHITE_KING_PATH = "img/WhiteKing.png";
const QString BLACK_ROOK_PATH = "img/BlackRook.png";
const QString WHITE_ROOK_PATH = "img/WhiteRook.png";
const QString BLACK_KNIGHT_PATH = "img/BlackKnight.png";
const QString WHITE_KNIGHT_PATH = "img/WhiteKnight.png";
const QString WINDOW_ICON_PATH = "img/windowIcon.png";
const QString WHITE_TURN = "blanc";
const QString BLACK_TURN = "noir";
const QString WINDOW_NAME = "Echiquier";
const QString INIT_POSITION_COMBO_BOX_STYLE = "background: white; font-size: 12px; font-weight: bold; color: black; border: 2px groove white;";
const QString RESET_BUTTON_STYLE = "background: darkgreen; font-size: 12px; font-weight: bold; color: white; border: 2px groove white;";
const QString END_GAME_BUTTON_STYLE = "background: indianred; font-size: 12px; font-weight: bold; color: white; border: 2px groove white;";
const QString END_GAME_BUTTON_TEXT = "End game";
const QString RESET_BUTTON_TEXT = "Reset";
const std::string FAULTY_FIELD = "NxxxKxxR/8/8/8/8/8/8/rxkkxnxx b";
const std::string BOURDONNAIS_MCDONNELL_1834_POSITION = "xRxxxxxx/nxxxxxxx/8/kxKxxxxx/8/8/8/8 w";
const std::string POLGAR_KASPAROV_1996_POSITION = "xxxxRxxx/8/8/8/xxxxxxxK/xxxxxkxx/xxxxnxxx/xxxxxxrx w";
const std::string ALEKHINE_CAPABLANCA_1927_POSITION = "8/rxxnxxxx/xxxkxxxx/8/xxxKxxxx/8/xRxxxxxx/8 w";
const std::string KARPOV_FTACNIK_1988_POSITION = "8/xxxxxRxx/xnxxKxxx/8/8/xxxxxxxk/8/8 w";
const std::string EXCEPTION_NUMBER_OF_KINGS = "Exception CorrectNumberofKings :: ";
const QColor BLACK_COLOR_PALETTE = QColor::fromRgb(181, 136, 99);
const QColor WHITE_COLOR_PALETTE = QColor::fromRgb(240, 217, 181);
constexpr int GAME_BOARD_MARGINS = 10;
constexpr int IS_PAIR = 2;
constexpr int ICON_SIZE = 75;
constexpr int POSITION_ONE = 0;
constexpr int POSITION_TWO = 1;
constexpr int POSITION_THREE = 2;
constexpr int POSITION_FOUR = 3;
constexpr int DIMENSION_H_COMBO_INITIAL_POSITIONS = 100;
constexpr int DIMENSION_V_COMBO_INITIAL_POSITIONS = 50;
constexpr int DIMENSION_H_RESET = 100;
constexpr int DIMENSION_V_RESET = 50;
constexpr int DIMENSION_H_END = 100;
constexpr int DIMENSION_V_END = 50;
constexpr int SPACE = 20;
const QStringList POSSIBLE_INITIAL_POSITIONS =
{
    {"Bourdonnais vs. McDonnell (1834)"},
    {"Polgar vs. Kasparov (1996)"},
    {"Alekhine vs. Capablanca (1927)"},
    {"Karpov vs. Ftáčnik (1988)"}
};
const std::vector<int> SPACE_VALUE = { 0, 2, 4 };

enum class	ButtonState { select, place };
class		ButtonPieces;


class ChessWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void errorMessage();
    void chess();
    void terminateProgram() const;
    void resetProgram();

public:
    ChessWindow(QWidget* parent = nullptr);
    void addApp(QApplication*);
    void initLabel(const model::Color&) const;
    void invertButtonState();
    ButtonState	getButtonState() const;
    QLabel*	getCurrentTurn() const;
    std::pair<int, int> getClickPositionBuffer() const;
    void setClickPositionBuffer(const std::pair<int, int>&);
    ButtonPieces* getButtonBuffer() const;
    void setButtonBuffer(ButtonPieces*);
    model::Board* getBoard() const;
    void addImage(ButtonPieces*, const char) const;
    void addTile(QGridLayout*);
    template <typename T>
    void initObjectSpace(T*, const int, const int, const int, const int, const int) const;
    template <typename T>
    void initObject(T*, const QString&, const int, const int) const;

    ~ChessWindow() override = default;

private:
    QPushButton* newButton(model::Color color, int, int);
    ButtonPieces* buttonBuffer_;
    QLabel*	currentTurn_ = nullptr;
    ButtonState	buttonState_ = ButtonState::select;
    std::pair<int, int>	 clickPositionBuffer_ = { 0, 0 };
    std::unique_ptr<model::Board> graphicBoard_	= std::make_unique<model::Board>(model::Color::White);
    QApplication* app_ = nullptr;
    QGridLayout* layoutChessBoard_ = nullptr;
    QComboBox* comboInitPos_ = nullptr;
};


class ButtonPieces : public QPushButton {
public:
    ButtonPieces(const int, const int, const model::Color&, ChessWindow*);

    void deletePiece();
    void addPiece(const QString&);
    void initButton();
    void initPalette(const model::Color&);a
    model::Color getColor() const;
    void setColorLabel(const model::Color&) const;
    void mousePressEvent(QMouseEvent* event) override;

private:
    int	positionX_;
    int	positionY_;
    model::Color color_;
    QPalette palette_;
    ChessWindow* chessWindow_;
    const int dimension_ = 100;
    bool changePosition = true;
};


class ErrorWindow : QMessageBox {
    Q_OBJECT
public:
    ErrorWindow();
};
};
