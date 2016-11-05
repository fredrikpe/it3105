#include <iostream>

#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QPen(Qt::blue, 8));

    QRect rect = event->rect();
    int x_scalar = rect.right() - rect.left() - 10;
    int y_scalar = rect.bottom() - rect.top() - 10;

    for (auto&& city : som->cities)
    {
        qreal x = x_scalar * city.first + 5;
        qreal y = y_scalar * city.second + 5;
        p.drawPoint(QPointF(x, y));
    }

    p.setPen(QPen(Qt::red, 6));
    QPointF prev_point;
    QPointF current_point;
    bool not_first_point = false;
    for (auto node : som->nodes)
    {
        qreal x = x_scalar * node.first + 5;
        qreal y = y_scalar * node.second + 5;
        current_point = QPointF(x, y);
        p.drawPoint(current_point);

        if (not_first_point)
        {
            p.setPen(QPen(Qt::black, 1));
            p.drawLine(prev_point, current_point);
            p.setPen(QPen(Qt::red, 6));
        } else {
            not_first_point = true;
        }

        prev_point = current_point;
    }

    p.setPen(QPen(Qt::green, 2));
    not_first_point = false;
    if (som->tour_indexes.size() > 0)
    {
        for (auto i : som->tour_indexes)
        {
            qreal x = x_scalar * som->cities[i].first + 5;
            qreal y = y_scalar * som->cities[i].second + 5;

            current_point = QPointF(x, y);

            if (not_first_point)
            {
                p.drawLine(prev_point, current_point);
            } else {
                not_first_point = true;
            }

            prev_point = current_point;
        }
    }

    // Draw tour distance
    p.setPen(QPen(Qt::black, 2));
    QString distance = "Distance: ";
    distance += QString::number(som->tour_distance);
    p.drawText(10, 50, distance);
}













