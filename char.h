#ifndef CHAR_H
#define CHAR_H

#include <QDebug>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QMovie>
#include "qpainter.h"
class Character : public QGraphicsObject
{
    Q_OBJECT;

public:
    Character(QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent)
    {
        {
            setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            movie = new QMovie(":/figs/capoo.gif");
            connect(movie, &QMovie::frameChanged, this, [this](int frame) {
                if (frame != -1)
                    update();
            });
            movie->start();
            movie->setScaledSize(QSize(160, 135));
        }
    }
    QRectF boundingRect() const override
    {
        return QRectF(0, 0, 160, 135); // 根据GIF尺寸调整
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->fillRect(boundingRect(), Qt::transparent);
        painter->drawPixmap(0, 0, movie->currentPixmap());
    }

    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_A) {
            setPos(x() - 10, y());
        } else if (event->key() == Qt::Key_D) {
            setPos(x() + 10, y());
        } else if (event->key() == Qt::Key_W) {
            setPos(x(), y() - 10);
        } else if (event->key() == Qt::Key_S) {
            setPos(x(), y() + 10);
        }
        emit position_changed();
    }

private:
    QMovie *movie;

signals:
    void position_changed();
};

#endif // CHAR_H
