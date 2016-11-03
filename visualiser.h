#ifndef VISUALISER_H
#define VISUALISER_H

#include <QObject>
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QWidget>
#include <QVector>

#include <QDebug>
#include <QTimer>

#define RECT_WIDTH 20

class visualiser: public QWidget
{
    Q_OBJECT

public:
    explicit visualiser(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent*);
//    void timerEvent(QTimerEvent*);
};

#endif // VISUALISER_H
