#ifndef MYSENCE_H
#define MYSENCE_H
#include <QDebug>
#include <QGraphicsScene>
#include <QObject>
#include <Qpainter>
#include "aimline.h"
#include "char.h"
#include "mob.h"
#include "prop.h"

#define AINUM 5

//这个要替换main中的scene
class MySence : public QGraphicsScene
{
    Q_OBJECT
    QTimer* timer;
    QTimer* character_attack_timer;
    QTimer* aimTimer;

public:
    MySence();
    bool areItemsClose(QGraphicsItem* item1, QGraphicsItem* item2, float threshold);
    int return_char_distance_squre(Character* c1, Character* c2);

    AimLine aimline;
    QVector<AimLine*> aimlines;

    void handle_mob_death(int id);
    int cur_ai_num;
    int kill_num = 0;

    QTimer* cmt;

    void character_move()
    {
        QList<QGraphicsItem*> allItems = this->items();
        Character* character = nullptr;
        QList<QGraphicsItem*> propItems;
        QList<QGraphicsItem*> mobItems; // 添加Mob列表

        // 分类所有物品
        for (QGraphicsItem* item : allItems) {
            if (item->type() == Character::Type && dynamic_cast<Character*>(item)->is_mob == false) {
                character = dynamic_cast<Character*>(item); // 收集所有Character
            } else if (item->type() == Prop::Type) {
                propItems.append(item);
            } else if (item->type() == Character::Type
                       && dynamic_cast<Character*>(item)->is_mob == true) {
                mobItems.append(item); // 收集所有Mob
            }
        }

        // 对每个Mob执行AI逻辑
        for (QGraphicsItem* mobItem : mobItems) {
            Mob* mob = static_cast<Mob*>(mobItem);
            // 检查Mob是否死亡或正在移动
            if (mob->dead || mob->is_moving) {
                //BUG:is_moving停不下来！！
                continue;
            }

            // 随机移动概率检查 (15%概率进行随机移动)
            static QRandomGenerator* generator = QRandomGenerator::global();
            int randomChance = generator->bounded(100); // 0-99的随机数

            if (randomChance < 15) { // 15%概率随机移动
                QStringList directions = {"up", "down", "left", "right"};
                QString randomDirection = directions[generator->bounded(directions.size())];
                int randomDuration = generator->bounded(500, 1501); // 500-1500ms随机持续时间

                qDebug() << "Random move:" << randomDirection << "for" << randomDuration << "ms";
                mob->handle_direction_input(randomDirection, randomDuration);
                continue; // 执行随机移动后跳过其他逻辑
            }

            // 查找范围内的Character对象
            Character* nearestCharacter = nullptr;
            double minCharacterDistance = 600.0;

            if (character && !character->dead) {
                // 计算距离
                double dx = character->x() - mob->x();
                double dy = character->y() - mob->y();
                double distance = std::sqrt(dx * dx + dy * dy);
                qDebug() << distance;

                if (distance <= 600.0 && distance < minCharacterDistance) {
                    nearestCharacter = character;
                }
            }

            if (nearestCharacter) {
                // 有Character在范围内，根据血量决定行为
                qDebug() << "yes";
                bool shouldApproach = mob->hp() > 50; // 血量大于50靠近，否则远离
                QString direction = calculateMobDirection(mob, nearestCharacter, shouldApproach);
                if (!direction.isEmpty()) {
                    mob->handle_direction_input(direction, 1000); // 移动1秒
                }
            } else {
                // 没有Character在范围内，寻找最近的Prop
                QGraphicsItem* nearestProp = nullptr;
                double minPropDistance = std::numeric_limits<double>::max();

                for (QGraphicsItem* propItem : propItems) {
                    if (!dynamic_cast<Prop*>(propItem)->get_picked()) {
                        continue;
                    }
                    // 计算距离
                    double dx = propItem->x() - mob->x();
                    double dy = propItem->y() - mob->y();
                    double distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < minPropDistance) {
                        nearestProp = propItem;
                        minPropDistance = distance;
                    }
                }

                if (nearestProp) {
                    // qDebug() << "move to prop";
                    QString direction = calculateMobDirection(mob, nearestProp, true); // 靠近Prop
                    if (!direction.isEmpty()) {
                        mob->handle_direction_input(direction, 1000); // 移动1秒
                    }
                } else {
                    // 如果没有目标，增加随机移动的概率 (30%概率)
                    if (randomChance < 30) {
                        QStringList directions = {"up", "down", "left", "right"};
                        QString randomDirection = directions[generator->bounded(directions.size())];
                        int randomDuration = generator->bounded(300, 1001); // 300-1000ms随机持续时间

                        qDebug() << "Idle random move:" << randomDirection << "for"
                                 << randomDuration << "ms";
                        mob->handle_direction_input(randomDirection, randomDuration);
                    }
                }
            }
        }
    }

    // 辅助函数：计算Mob的移动方向
    QString calculateMobDirection(QGraphicsItem* from, QGraphicsItem* to, bool moveTowards)
    {
        double dx = to->x() - from->x();
        double dy = to->y() - from->y();

        // 如果是远离目标，则反转方向
        if (!moveTowards) {
            dx = -dx;
            dy = -dy;
        }

        // 避免距离太近时的抖动
        if (std::abs(dx) < 10.0 && std::abs(dy) < 10.0) {
            return "";
        }

        // 计算主要方向
        QString horizontalDir = "";
        QString verticalDir = "";

        if (std::abs(dx) > 15) { // 设置阈值避免频繁方向切换
            horizontalDir = (dx > 0) ? "d" : "a";
        }

        if (std::abs(dy) > 15) { // 设置阈值避免频繁方向切换
            verticalDir = (dy > 0) ? "s" : "w";
        }

        // 组合方向
        if (!horizontalDir.isEmpty() && !verticalDir.isEmpty()) {
            // 对角线移动 - 根据你的函数格式
            return horizontalDir + verticalDir;
        } else if (!horizontalDir.isEmpty()) {
            // 水平移动
            return horizontalDir;
        } else if (!verticalDir.isEmpty()) {
            // 垂直移动
            return verticalDir;
        }

        return "";
    }

private slots:
    void checkDistance();
    void checkCharacterDistance();
    void getAimedChar();
    void resetAimLine();
signals:
    void propPicked(Prop* p);
    void player_win_signal();
};

#endif // MYSENCE_H
