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

private:
    QMovie *movie;
    QLabel *label;

    QTimer *moveTimer;
    QSet<int> pressedKeys;

    int knife_num = 4;

signals:
    void position_changed();
};

#endif // CHAR_H
