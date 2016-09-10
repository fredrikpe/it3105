#include <iostream>

#include "board.h"

Board::Board(QWidget *parent) :
    QWidget::QWidget(parent)
{
}

bool Board::validState(std::vector<int> queens)
{
    if (queens.size() > this->boardSize)
    {
        std::cout << "Error, too long vector in validState fun." << std::endl;
        return false;
    }

    // Check if any queens lie on the same row
    auto copy = queens;
    std::sort(copy.begin(), copy.end());
    auto hasDuplicates = std::unique(copy.begin(), copy.end());
    if (hasDuplicates != copy.end())
    {
        return false;
    }

    // Check if any queens lie on the same diagonal
    for (int i=0; i<queens.size(); i++)
    {
        for (int j=0; j<queens.size(); j++)
        {
            if (i == j) continue;

            // On diagonal if x and y distance is equal
            int y_dist = std::abs(queens[i] - queens[j]);
            int x_dist = std::abs(i - j);
            if (y_dist == x_dist)
            {
                return false;
            }
        }
    }
    return true;
}

void Board::paintEvent(QPaintEvent *event)
{
    QRect rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("times", 48));

    int sWidth = rect.width()/this->boardSize;
    int sHeight = rect.height()/this->boardSize;

    for (int i=0; i<this->boardSize; i++)
    {
        for (int j=0; j<this->boardSize; j++)
        {
            if ((i % 2 && !(j % 2)) || (i + j) % 2)
            {
                painter.fillRect(rect.left() + i*sWidth, rect.top() + j*sHeight, sWidth, sHeight, Qt::lightGray);
            } else {
                painter.fillRect(rect.left() + i*sWidth, rect.top() + j*sHeight, sWidth, sHeight, Qt::white);
            }
            if (this->queenYPos.size() > i && this->queenYPos[i] == j)
            {
                if (this->queenYPos.size() == this->boardSize && this->validState(this->queenYPos))
                {
                    painter.setPen(Qt::blue);
                } else {
                    painter.setPen(Qt::red);
                }
                painter.drawText(rect.left() + i*sWidth + sWidth/4, rect.top() + j*sHeight + 2*sHeight/3, "Q");
            }
        }
    }
}
