#ifndef CHAR_H
#define CHAR_H

#include <QDebug>
#include <QGraphicsObject>
#include <QKeyEvent>
#include <QLabel>
#include <QList>
#include <QMovie>
#include <QTimer>
#include "qpainter.h"
#define ATTACK_COOLDOWN 500

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
        if (dead)
            return;
        if (!event->isAutoRepeat()) {
            movie->start();
            if (!pressedKeys.contains(event->key())) {
                pressedKeys.insert(event->key());
            }
        }
    }
    void keyReleaseEvent(QKeyEvent *event) override
    {
        if (dead)
            return;
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

    void handle_dead()
    {
        dead = true;
        moveTimer->stop();
        m_rotateTimer->stop();
        give_knife_timer->stop();
        update();
    }

    int get_near_attack_range() { return knife_r; }

    bool dead = false;

    int get_aim_range() { return aim_range; }
    QVector<Character *> can_aim_targets;
    Character *aim_target;
    int id;

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

    QPixmap dead_image;

    QTimer *speed_timer;

    int aim_range;

    QTimer *attack_cooldown;
    bool ready_to_attack = true;

    void add_heart()
    {
        QPixmap *ph = new QPixmap(":/figs/heart.jpg");
        if (!ph->isNull()) {
            hearts.push_back(ph);
        }
    }

    void pop_heart() { hearts.pop_back(); }

    void control_hearts()
    {
        int hearts_cnt = health / 20;
        if (hearts_cnt < hearts.size()) {
            pop_heart();
        } else if (hearts_cnt > hearts.size()) {
            add_heart();
        }
    }
public slots:
    void be_hit()
    {
        if (knife_num) {
            pop_knife();
        } else {
            drop_health(20);
        }
    }

protected:
    QSet<int> pressedKeys;
    void shoot()
    {
        if (ready_to_attack) {
            if (knife_num && aim_target) {
                be_hit();
                // aim_target->be_hit();

                QTimer *t = new QTimer();
                t->setSingleShot(true);
                t->start(150);
                // t->deleteLater();
                QObject::connect(t, &QTimer::timeout, aim_target, &Character::be_hit);

                attack_cooldown->start(ATTACK_COOLDOWN);
                emit throw_knife_signal();
                ready_to_attack = false;
            }
        }
    }

signals:
    void position_changed();
    void Dead_signal();
    void throw_knife_signal();
};

#endif // CHAR_H
