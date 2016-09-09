#include "canvas.h"

Canvas::Canvas(QWidget *parent) :
    QWidget::QWidget(parent)
{
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QRect rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    //painter.drawRect(rect);

    int sWidth = rect.width()/this->n;
    int sHeight = rect.height()/this->n;

    for (int i=0; i<this->n; i++)
    {
        for (int j=0; j<this->n; j++)
        {
            if ((i % 2 && !(j % 2)) || (i + j) % 2)
            {
                painter.fillRect(rect.left() + i*sWidth, rect.top() + j*sHeight, sWidth, sHeight, Qt::lightGray);
            } else {
                painter.fillRect(rect.left() + i*sWidth, rect.top() + j*sHeight, sWidth, sHeight, Qt::white);
            }
        }
    }
}
