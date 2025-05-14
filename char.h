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

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void push_knife();
    void pop_knife();
    void add_health(int amount);
    void picked_boots();

    void handle_pick(int id);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void speed_finished()
    {
        high_speed = false;
        speed = speed / 2;
    }

private:
    QMovie *movie;
    QLabel *label;

    QTimer *moveTimer;
    QSet<int> pressedKeys;

    QVector<QPixmap *> knifes;

    int knife_num;

    QTimer *m_rotateTimer; // 旋转动画定时器
    qreal m_rotationAngle = 0;

    int speed = 3;
    bool high_speed = false;

    QTimer *speed_timer;

signals:
    void position_changed();
};

#endif // CHAR_H
