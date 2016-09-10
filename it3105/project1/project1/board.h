#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

#include <vector>

class Board : public QWidget
{
    Q_OBJECT


public:
    Board(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    bool validState(std::vector<int> queens);

    int boardSize = 4;
    std::vector<int> queenYPos;
};

#endif // BOARD_H
