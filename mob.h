#ifndef MOB_H
#define MOB_H
#include <QRandomGenerator64>
#include "char.h"

class Mob : public Character
{
    Q_OBJECT
    QTimer shoot_timer;

public:
    Mob()
    {
        auto placeholder = this;
        connect(placeholder, &Mob::moved_signal, this, &Mob::random_move);
        emit moved_signal();
        shoot_timer.start(1000);
        connect(&shoot_timer, &QTimer::timeout, this, &Mob::handle_shoot);
    }
public slots:
    void handle_shoot() { shoot(); }

    void random_move()
    {
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
};

#endif // MOB_H
