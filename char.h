#ifndef CHAR_H
#define CHAR_H

#include <QDebug>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QMovie>
#include <QTimer>
#include "qpainter.h"
class Character : public QGraphicsObject
{
    Q_OBJECT;

public:
    Character(QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent)
    {
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        movie = new QMovie(":/figs/capoo.gif");
        connect(movie, &QMovie::frameChanged, this, [this](int frame) {
            if (frame != -1)
                update();
        });
        movie->start();
        movie->setScaledSize(QSize(160, 135));

        moveTimer = new QTimer(this);
        connect(moveTimer, &QTimer::timeout, [this]() {
            QPointF direction(0, 0);
            foreach (int key, pressedKeys) {
                if (key == Qt::Key_A)
                    direction.rx() -= 1;
                else if (key == Qt::Key_D)
                    direction.rx() += 1;
                else if (key == Qt::Key_W)
                    direction.ry() -= 1;
                else if (key == Qt::Key_S)
                    direction.ry() += 1;
            }
            if (!direction.isNull()) {
                direction /= sqrt(direction.x() * direction.x() + direction.y() * direction.y());
                setPos(x() + 6 * direction.x(), y() + 6 * direction.y());
                emit position_changed();
            }
        });
        moveTimer->start(16); // 约60FPS
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
        if (!pressedKeys.contains(event->key())) {
            pressedKeys.insert(event->key());
        }
    }

    void keyReleaseEvent(QKeyEvent *event) { pressedKeys.remove(event->key()); }

private:
    QMovie *movie;

    QTimer *moveTimer;
    QSet<int> pressedKeys;

signals:
    void position_changed();
};

#endif // CHAR_H
