#ifndef CHAR_H
#define CHAR_H

#include <QDebug>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QLabel>
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
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);

        movie = new QMovie(":/figs/capoo.gif");
        movie->setCacheMode(QMovie::CacheAll);
        label = new QLabel();
        label->setMovie(movie);
        movie->setScaledSize(QSize(160, 135));
        movie->start();
        movie->setPaused(true);
        connect(movie, &QMovie::frameChanged, this, [this](int frame) {
            if (frame != -1)
                update();
        });
        connect(movie, &QMovie::finished, this, [this]() {
            movie->jumpToFrame(0);
            qDebug() << "finishtd";
        });

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

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
    {
        if (change == ItemPositionChange && scene()) {
            QPointF newPos = value.toPointF();

            // 计算到中心点(1500,1500)的距离
            qreal dx = newPos.x() - 1420;
            qreal dy = newPos.y() - 1420;
            qreal distance = sqrt(dx * dx + dy * dy);

            // 如果距离超过900，将位置限制在半径为900的圆上
            if (distance > 900) {
                qreal scale = 900 / distance;
                QPointF center(1420, 1420);
                newPos = center + QPointF(dx * scale, dy * scale);
                return newPos;
            }
        }
        return QGraphicsItem::itemChange(change, value);
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

    void keyPressEvent(QKeyEvent *event) override
    {
        if (!event->isAutoRepeat()) {
            qDebug() << "press";
            movie->start();
            if (!pressedKeys.contains(event->key())) {
                pressedKeys.insert(event->key());
            }
        }
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        if (!event->isAutoRepeat()) {
            qDebug() << "release";
            pressedKeys.remove(event->key());
            if (!pressedKeys.count()) {
                movie->setPaused(true);
                qDebug() << "paused";
            }
        }
    }

private:
    QMovie *movie;
    QLabel *label;

    QTimer *moveTimer;
    QSet<int> pressedKeys;

signals:
    void position_changed();
};

#endif // CHAR_H
