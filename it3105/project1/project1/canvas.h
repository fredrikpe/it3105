#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Canvas : public QWidget
{
    Q_OBJECT


public:
    Canvas(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    int n = 4;
};

#endif // CANVAS_H
