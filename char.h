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
    Character(QGraphicsItem *parent = nullptr);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void keyPressEvent(QKeyEvent *event) override
    {
        if (!event->isAutoRepeat()) {
            movie->start();
            if (!pressedKeys.contains(event->key())) {
                pressedKeys.insert(event->key());
            }
        }
    }
    void keyReleaseEvent(QKeyEvent *event) override
    {
        if (!event->isAutoRepeat()) {
            pressedKeys.remove(event->key());
            if (!pressedKeys.count()) {
                movie->setPaused(true);
            }
        }
    }

    void push_knife();
    void pop_knife();
    void add_health(int amount);
    void drop_health(int amount);
    void picked_boots();

    void handle_pick(int id);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void speed_finished()
    {
        if (high_speed) {
            speed = speed / 2;
        }
        high_speed = false;
    }

    void handle_give_knife_timer();

    void handle_dead();

private:
    QMovie *movie;
    QLabel *label;

    QTimer *moveTimer;

    QVector<QPixmap *> knifes;
    QVector<QPixmap *> hearts;
    QVector<QPixmap *> buff_lables;

    int knife_num;

    QTimer *m_rotateTimer; // 旋转动画定时器
    QTimer *give_knife_timer;
    qreal m_rotationAngle = 0;

    int speed = 3;
    bool high_speed = false;

    int health;
    int knife_r;

    bool dead = false;
    QPixmap dead_image;

    QTimer *speed_timer;

protected:
    QSet<int> pressedKeys;
signals:
    void position_changed();
    void Dead_signal();
};

#endif // CHAR_H
