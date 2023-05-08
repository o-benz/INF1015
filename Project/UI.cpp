#include "UI.h"
#include "model.h"
#include "board.h"
#include "piece.h"
#include "tile.h"
using namespace view;
using namespace model;

ButtonPieces::ButtonPieces(const int positionX, const int positionY, const model::Color& color, ChessWindow* chessWindow) : positionX_(positionX), positionY_(positionY), color_(color), chessWindow_(chessWindow) {
    static constexpr int LETTER_POSITION = 1, NUMBER_POSITION = 0;

    if (positionX == 0 && positionY == BOARD_DIMENSION_Y - 1)
        setText(QString::fromStdString("1A"));

    else if (positionX == 0)
        setText(QString::fromStdString(std::string{ chessWindow->getBoard()->getTile(positionX, positionY)->getTileName()[LETTER_POSITION] }));

    else if (positionY == BOARD_DIMENSION_Y - 1)
        setText(QString::fromStdString(std::string{ chessWindow->getBoard()->getTile(positionX, positionY)->getTileName()[NUMBER_POSITION] }));

    setStyleSheet(TILE_NAMES);

    palette_ = palette();
    initButton();
    initPalette(color_);
}


void ButtonPieces::deletePiece() {
    setIcon(QIcon());
    setIconSize(QSize(ICON_SIZE, ICON_SIZE));
}

void ButtonPieces::addPiece(const QString& path) {
    QPixmap pixmap(path);
    QIcon BoutonPiece(pixmap);
    setIcon(BoutonPiece);
    setIconSize(QSize(ICON_SIZE, ICON_SIZE));
}

void ButtonPieces::initButton() {
    setFixedSize(dimension_, dimension_);
    setFlat(true);
    update();
}

void ButtonPieces::initPalette(const Color& color) {
    if (color == Color::Black)
        palette_.setColor(QPalette::Button, BLACK_COLOR_PALETTE);
    else
        palette_.setColor(QPalette::Button, WHITE_COLOR_PALETTE);

    setAutoFillBackground(true);
    setPalette(palette_);
}

Color ButtonPieces::getColor() const {
    return color_;
}


void ButtonPieces::setColorLabel(const Color& color) const {
    switch (color) {
    case Color::Black:
        chessWindow_->getCurrentTurn()->setText(WHITE_TEXT);
        chessWindow_->getCurrentTurn()->setStyleSheet(WHITE_LABEL);
        break;

    case Color::White:
        chessWindow_->getCurrentTurn()->setText(BLACK_TEXT);
        chessWindow_->getCurrentTurn()->setStyleSheet(BLACK_LABEL);
        break;
    }
}

void ButtonPieces::mousePressEvent(QMouseEvent* event) {
    using namespace std;

    if (chessWindow_->getButtonState() == ButtonState::select) {
        if (chessWindow_->getBoard()->getTile(positionX_, positionY_)->getOccupation() &&
            chessWindow_->getBoard()->getTurn() == chessWindow_->getBoard()->getTile(positionX_, positionY_)->getOccupyingPiece()->getColor()) {

            switch (color_) {
            case Color::White:
                setStyleSheet(WHITE_PALETTE_SELECTION);
                break;

            case Color::Black:
                setStyleSheet(BLACK_PALETTE_SELECTION);
                break;
            }

            chessWindow_->setClickPositionBuffer({ positionX_, positionY_ });
            chessWindow_->setButtonBuffer(this);
            chessWindow_->getBoard()->getTile(positionX_, positionY_)->getOccupyingPiece()->clearMovements();
            chessWindow_->getBoard()->getTile(positionX_, positionY_)->getOccupyingPiece()->calculatePossibleMovements({ positionX_, positionY_ }, chessWindow_->getBoard());
            chessWindow_->invertButtonState();

            if (chessWindow_->getBoard()->getCheckState() == true && !changePosition)
                chessWindow_->getBoard()->invertCheck();

            changePosition = true;
        }
    }

    else {
        pair<int, int> initialTile = { chessWindow_->getClickPositionBuffer().first, chessWindow_->getClickPositionBuffer().second };
        pair<int, int> desiredMove = { positionX_, positionY_ };

        switch (chessWindow_->getButtonBuffer()->getColor()) {
        case Color::White:
            chessWindow_->getButtonBuffer()->setStyleSheet(WHITE_BUFFER_STYLE);
            break;
        case Color::Black:
            chessWindow_->getButtonBuffer()->setStyleSheet(BLACK_BUFFER_STYLE);
            break;
        }

        bool moveAccept = false;

        for (const pair<int, int>& possibleMoves : chessWindow_->getBoard()->getTile(initialTile.first, initialTile.second)->getOccupyingPiece()->getPossibleMovements()) {
            if (desiredMove == possibleMoves) {
                setColorLabel(chessWindow_->getBoard()->getTile(initialTile.first, initialTile.second)->getOccupyingPiece()->getColor());

                chessWindow_->getBoard()->movePiece(chessWindow_->getBoard()->getTile(initialTile.first, initialTile.second), chessWindow_->getBoard()->getTile(desiredMove.first, desiredMove.second));

                switch (chessWindow_->getBoard()->getTile(desiredMove.first, desiredMove.second)->getOccupyingPiece()->getCharacter()) {
                case BLACK_KING:
                    addPiece(BLACK_KING_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;

                case WHITE_KING:
                    addPiece(WHITE_KING_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;

                case BLACK_ROOK:
                    addPiece(BLACK_ROOK_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;

                case WHITE_ROOK:
                    addPiece(WHITE_ROOK_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;

                case BLACK_KNIGHT:
                    addPiece(BLACK_KNIGHT_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;

                case WHITE_KNIGHT:
                    addPiece(WHITE_KNIGHT_PATH);
                    chessWindow_->getButtonBuffer()->deletePiece();
                    break;
                }

                chessWindow_->invertButtonState();
                moveAccept = true;

                if (chessWindow_->getBoard()->getCheckState() == true) {
                    emit chessWindow_->chess();
                }

                break;
            }
        }

        if (!moveAccept) {
            changePosition = false;
            chessWindow_->invertButtonState();
            emit chessWindow_->errorMessage();
        }
    }
}

void ChessWindow::invertButtonState()
{
    switch (buttonState_) {
    case ButtonState::select:
        buttonState_ = ButtonState::place;
        break;
    case ButtonState::place:
        buttonState_ = ButtonState::select;
        break;
    }
}
ButtonState ChessWindow::getButtonState() const
{
    return buttonState_;
}
QLabel* ChessWindow::getCurrentTurn() const
{
    return currentTurn_;
}
std::pair<int, int> ChessWindow::getClickPositionBuffer() const
{
    return clickPositionBuffer_;
}
void ChessWindow::setClickPositionBuffer(const std::pair<int, int>& nBuffer)
{
    clickPositionBuffer_ = nBuffer;
}
PiecesBouton* ChessWindow::getButtonBuffer() const
{
    return buttonBuffer_
}
void ChessWindow::setButtonBuffer(ButtonPieces* buttonBuffer)
{
    buttonBuffer_ = buttonBuffer;
}
modele::Plateau* ChessWindow::getBoard() const
{
    return graphicBoard_.get();
}
void ChessWindow::errorMessage() {
    QMessageBox::critical(this, ERROR_TITLE, ERROR_MESSAGE);
}
void ChessWindow::chess() {
    if (getBoard()->getTurn() == Color::White)
        QMessageBox::warning(this, CHESS_TITLE, "Le roi " + WHITE_TURN + " est en échec");
            else
            QMessageBox::warning(this, CHESS_TITLE, "Le roi " + BLACK_TURN + " est en échec");
}
void ChessWindow::terminateProgram() const
{
    app_->quit();
}
void ChessWindow::resetProgram()
{
    using namespace iter;
    graphicBoard_->resetNumberOfKings();
    if (layoutChessBoard_ != nullptr) {
        QLayoutItem* item;
        while ((item = layoutChessBoard_->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    const int indexCombo = comboInitPos_->currentIndex();
    switch (indexCombo) {
    case POSITION_ONE:
        graphicBoard_->create(BOURDONNAIS_MCDONNELL_1834_POSITION);
        break;
    case POSITION_TWO:
        graphicBoard_->create(POLGAR_KASPAROV_1996_POSITION);
        break;
    case POSITION_THREE:
        graphicBoard_->create(ALEKHINE_CAPABLANCA_1927_POSITION);
        break;
    case POSITION_FOUR:
        graphicBoard_->create(KARPOV_FTACNIK_1988_POSITION);
        break;
    }
    addTile(layoutChessBoard_);
    graphicBoard_->setTour(Color::White);
    initLabel(Color::White);
}
void ChessWindow::addImage(ButtonPieces* button, const char character) const
{
    using namespace std;
    map<const char, const QString> imagePath {
        { BLACK_KING,		BLACK_KING_PATH },
        { WHITE_KING,		WHITE_KING_PATH },
        { BLACK_ROOK,		BLACK_ROOK },
        { WHITE_ROOK,		WHITE_ROOK_PATH },
        { BLACK_KNIGHT,		BLACK_KNIGHT_PATH },
        { WHITE_KNIGHT, 	WHITE_KNIGHT_PATH }
    };
    QPixmap pixmap(imagePath[character]);
    QIcon ButtonPieces(pixmap);
    button->setIcon(ButtonPiece);
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
}
QPushButton* ChessWindow::nouveauBouton(Color color, int positionY, int positionX)
{
    ButtonPieces* button = new ButtonPieces(positionY, positionX, color, this);
    if (graphicBoard_->getTile(positionY, positionX)->getOccupation())
        switch (graphicBoard_->getTile(positionY, positionX)->getOccupyingPiece()->getCharacter()) {
        case BLACK_KING:
            addImage(button, BLACK_KING);
            break;
        case WHITE_KING:
            addImage(button, WHITE_KING);
            break;
        case BLACK_ROOK:
            addImage(button, BLACK_ROOK);
            break;
        case WHITE_ROOK:
            addImage(button, WHITE_ROOK);
            break;
        case BLACK_KNIGHT:
            addImage(button, BLACK_KNIGHT);
            break;
        case WHITE_KNIGHT:
            addImage(button, WHITE_KNIGHT);
            break;
        }
    return bouton;
}
void ChessWindow::addApp(QApplication* app)
{
    app_ = app;
}
void ChessWindow::initLabel(const Color& color) const
{
    switch (color) {
    case Color::White
        getCurrentTurn()->setText(WHITE_TEXT);
        getCurrentTurn()->setStyleSheet(WHITE_LABEL);
        break;
    case Color::Black:
        getCurrentTurn()->setText(BLACK_TEXT);
        getCurrentTurn()->setStyleSheet(BLACK_LABEL);
        break;
    }
}
void ChessWindow::addTile(QGridLayout* layout)
{
    using namespace iter;
    for (int i : range(BOARD_DIMENSION_Y))
        for (int j : range(BOARD_DIMENSION_X)) {
            if ((i + j) % IS_PAIR)
                layout->addWidget(newButton(Color::White, j, i), i, j);
            else
                layout->addWidget(newButton(Color::Black, j, i), i, j);
        }
}
template <typename T>
void ChessWindow::initObjectSpace(T* layout, const int left, const int up, const int right, const int down, const int space) const
{
    Expects(dynamic_cast<QGridLayout*>(layout) != nullptr ||
            dynamic_cast<QVBoxLayout*>(layout) != nullptr ||
            dynamic_cast<QHBoxLayout*>(layout) != nullptr);
    layout->setContentsMargins(left, up, right, down);
    layout->setSpacing(global);
}
template <typename T>
void ChessWindow::initObjet(T* objet, const QString& style, const int hori, const int verti) const
{
    Expects(dynamic_cast<QComboBox*>(objet)		!= nullptr ||
            dynamic_cast<QPushButton*>(objet)	!= nullptr);
    objet->setStyleSheet(style);
    objet->setMinimumWidth(hori);
    objet->setMinimumHeight(veri);
}
ChessWindow::ChessWindow(QWidget* parent)
{
    using namespace iter;
    using namespace std;
    QWidget*		widgetPrincipal	= new QWidget(this);
    QWidget*		chessBoard = new QWidget;
    QGridLayout*	layoutChessBoard = new QGridLayout(chessBoard);
    QVBoxLayout*	height = new QVBoxLayout;
    QLabel*		currentTurn = new QLabel;
    QHBoxLayout*	chessBoardButtons = new QHBoxLayout;
    QVBoxLayout*	buttonPanel = new QVBoxLayout;
    QComboBox*		comboInitPos = new QComboBox;
    QPushButton*	reset = new QPushButton;
    QPushButton*	end = new QPushButton;
    layoutChessBoard_ = layoutChessBoard;
    try {
        graphicBoard_->create(FAULTY_FIELD);
    }
    catch (CorrectNumberofKings & exception) {
        cout << EXCEPTION_NUMBER_OF_KINGS << exception.what() << endl;
        graphicBoard_->resetNumberOfKings();
        graphicBoard_->create(BOURDONNAIS_MCDONNELL_1834_POSITION);
    }
    addTile(layoutChessBoard);
    setWindowTitle(WINDOW_NAME);
    setWindowIcon(QIcon(WINDOW_ICON_PATH));
    initObjectSpace(layoutChessBoard, 0, 0, 0, 0, 0);
    initObjectSpace(height, 0, 0, 0, 0, 0);
    currentTurn->setContentsMargins(GAME_BOARD_MARGINS, GAME_BOARD_MARGINS, GAME_BOARD_MARGINS, GAME_BOARD_MARGINS);
    currentTurn_ = currentTurn;
    initLabel(Color::White);
    height->addWidget(currentTurn);
    initObjet(comboInitPos, INIT_POSITION_COMBO_BOX_STYLE, DIMENSION_H_COMBO_INITIAL_POSITIONS, DIMENSION_V_COMBO_INITIAL_POSITIONS);
    comboInitPos->addItems(POSSIBLE_INITIAL_POSITIONS);
    comboInitPos_ = comboInitPos;
    initObjet(reset, RESET_BUTTON_STYLE, DIMENSION_H_RESET, DIMENSION_V_RESET);
    reinitialiser->setText(RESET_BUTTON_TEXT);
    connect(reset, SIGNAL(clicked()), this, SLOT(reinitialiserProgramme()));
    initObjet(end, END_GAME_BUTTON_STYLE, DIMENSION_H_END, DIMENSION_V_END);
    end->setText(END_GAME_BUTTON_TEXT);
    connect(end, SIGNAL(clicked()), this, SLOT(terminateProgram()));
    buttonPanel->addWidget(comboInitPos);
    buttonPanel->addWidget(reset);
    buttonPanel->addWidget(terminateProgram);
    chessBoardButtons->addWidget(chessBoard);
    chessBoardButtons->addLayout(buttonPanel);
    initObjectSpace(chessBoardButtons, 0, 0, 0, 0, SPACE);
    for (int space : SPACE_VALUE)
        chessBoardButtons->insertStretch(space);
    height->addLayout(chessBoardButtons);
    widgetPrincipal->setLayout(height);
    setCentralWidget(widgetPrincipal);
}
