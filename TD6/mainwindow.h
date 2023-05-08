#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QMouseEvent>
#include "ui_mainwindow.h"
#include <QEvent>
#include "modele.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ChessWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(model::Board& chessBoard, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    void mousePressEvent(QMouseEvent* mouseEvent)
    {

        isFirstClick = !isFirstClick;
        emit clickedSignal(mouseEvent->x(), mouseEvent->y());

    }


signals:
    void clickedSignal(int x, int y);

public slots:

    void squareClickedPosition(int xMousePos, int yMousePos) {

    }


private:

    bool isFirstClick = false;

    Ui::ChessWindow ui;
    model::Board uiChessBoard;
};
#endif
