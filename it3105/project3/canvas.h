#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include <memory>

#include "selforganizingmap.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);

    std::shared_ptr<SelfOrganizingMap> som;

signals:

public slots:
};

#endif // CANVAS_H
