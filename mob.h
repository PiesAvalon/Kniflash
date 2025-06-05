#ifndef MOB_H
#define MOB_H
#include <QRandomGenerator64>
#include "char.h"

class Mob : public Character
{
    Q_OBJECT
    QTimer shoot_timer;
    bool mob_is_dead = false;

public:
    Mob()
    {
        auto placeholder = this;
        connect(placeholder, &Mob::moved_signal, this, &Mob::random_move);
        emit moved_signal();
        shoot_timer.start(1000);
        connect(&shoot_timer, &QTimer::timeout, this, &Mob::handle_shoot);
        connect(&shoot_timer, &QTimer::timeout, this, &Mob::ckeck_death);
        connect(this, &Character::Dead_signal, this, &Mob::handle_mob_dead);

        is_mob = true;
    }
    int hp() { return health; }

    void ckeck_death()
    {
        if (dead && !mob_is_dead) {
            emit mob_death_signal(this->id);
            mob_is_dead = true;
        }
    }

    // enum { MobType = UserType + 7 }; // 换个名字
    // int type() const override { return MobType; }
    // bool is_mob = true;

    bool is_moving = false;

    void handle_direction_input(const QString& direction, int duration_ms)
    {
        if (dead && !mob_is_dead) {
            emit mob_death_signal(this->id);
            mob_is_dead = true;
        }
        if (dead)
            return;

        qDebug() << "moved";
        is_moving = true;
        QList<Qt::Key> keysToPress;

        // 根据方向字符串确定需要按下的按键
        if (direction == "w" || direction == "up") {
            keysToPress.append(Qt::Key_W);
        } else if (direction == "s" || direction == "down") {
            keysToPress.append(Qt::Key_S);
        } else if (direction == "a" || direction == "left") {
            keysToPress.append(Qt::Key_A);
        } else if (direction == "d" || direction == "right") {
            keysToPress.append(Qt::Key_D);
        } else if (direction == "aw" || direction == "up-left") {
            keysToPress.append(Qt::Key_W);
            keysToPress.append(Qt::Key_A);
        } else if (direction == "dw" || direction == "up-right") {
            keysToPress.append(Qt::Key_W);
            keysToPress.append(Qt::Key_D);
        } else if (direction == "as" || direction == "down-left") {
            keysToPress.append(Qt::Key_S);
            keysToPress.append(Qt::Key_A);
        } else if (direction == "ds" || direction == "down-right") {
            keysToPress.append(Qt::Key_S);
            keysToPress.append(Qt::Key_D);
        } else {
            // 无效方向，直接返回
            return;
        }

        // 将按键添加到 pressedKeys 容器中
        for (Qt::Key key : keysToPress) {
            pressedKeys.insert(key);
        }

        // 创建定时器并设置超时处理
        QTimer* qt = new QTimer(this);
        qt->setSingleShot(true); // 设置为单次触发
        qt->start(duration_ms);

        // 连接超时信号到 lambda 函数，处理移动状态和按键移除
        connect(qt, &QTimer::timeout, this, [this, keysToPress, qt]() {
            // 将 is_moving 设为 false
            is_moving = false;

            // 从 pressedKeys 中移除这次按下的按键
            for (Qt::Key key : keysToPress) {
                pressedKeys.remove(key);
            }

            // 删除定时器对象
            qt->deleteLater();
        });
    }
    void change_is_moving() { is_moving = !is_moving; }
public slots:
    void handle_shoot() { shoot(); }

    void handle_mob_dead() { shoot_timer.stop(); }

    void random_move()
    {
        if (dead && !mob_is_dead) {
            emit mob_death_signal(this->id);
            mob_is_dead = true;
        }
        if (dead)
            return;
        // 1. 随机选择 A/S/D/W 中的一个按键
        Qt::Key randomKey;
        int keyChoice = QRandomGenerator::global()->bounded(4); // 0-3

        switch (keyChoice) {
        case 0:
            randomKey = Qt::Key_A;
            break;
        case 1:
            randomKey = Qt::Key_S;
            break;
        case 2:
            randomKey = Qt::Key_D;
            break;
        case 3:
            randomKey = Qt::Key_W;
            break;
        }
        randomKey = Qt::Key_P;

        // 2. 将按键 push 到 keys_pressed 容器中
        pressedKeys.insert(randomKey);

        // 3. 随机生成 1-5 秒的延迟（单位：毫秒）
        int delayMs = QRandomGenerator::global()->bounded(1000, 5001); // 1000ms-5000ms

        // 4. 设置定时器，在延迟后 pop 按键
        QTimer::singleShot(delayMs, this, [this, randomKey]() {
            auto it = std::find(pressedKeys.begin(), pressedKeys.end(), randomKey);
            if (it != pressedKeys.end()) {
                pressedKeys.erase(it);
            }
            emit moved_signal();
        });
    }

signals:
    void moved_signal();
    void mob_death_signal(int id);
};

#endif // MOB_H
