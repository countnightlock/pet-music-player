#include "visualiser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "universal_functions.h"

void visualiser::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int screenWidth = this->width() - 5;
    int screenHeight = this->height() - 5;
    QVector<QRect> rectVect;
    QRect rec;
    QString color = randomColorGenerator();
    painter.setPen(color);

    for(int i = 0; i < screenWidth/RECT_WIDTH; ++i)
    {
        int rectHeight = 5 + rand() % (3*screenHeight/4);
        QPoint bottomLeft(i * RECT_WIDTH, screenHeight);
        QPoint topRight((i+1) * RECT_WIDTH - 1, screenHeight - rectHeight);
        rec.setBottomLeft(bottomLeft);
        rec.setTopRight(topRight);
        rectVect.push_back(rec);
        painter.drawRect(rectVect.at(i));
    }
}
