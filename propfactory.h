#ifndef PROPFACTORY_H
#define PROPFACTORY_H

#include <QRandomGenerator>
#include <Qpair>
#include <QtMath>
#include "bush.h"
#include "mysence.h"
#include "prop.h"
#define KNIFE_NUM 50
#define HEALTH_NUM 7
#define BOOTS_NUM 7
#define BUSH_NUM 3

class PropFactory
{
public:
    PropFactory();
    PropFactory(MySence* scene)
    {
        int knife_number = KNIFE_NUM;
        int health_number = HEALTH_NUM;
        int bootes_number = BOOTS_NUM;
        for (int i = 0; i < knife_number; i++) {
            Prop* pk1 = new Prop(":/figs/knife.jpg", KNIFE);
            scene->addItem(pk1);

            QPair p = randomPos(900);
            pk1->setPos(p.first, p.second);
            Character::connect(scene, &MySence::propPicked, pk1, &Prop::handlePicked);
        }

        for (int i = 0; i < health_number; i++) {
            Prop* pk1 = new Prop(":/figs/healt_bottle.jpg", HEALTH);
            scene->addItem(pk1);

            QPair p = randomPos(850);
            pk1->setPos(p.first, p.second);
            Character::connect(scene, &MySence::propPicked, pk1, &Prop::handlePicked);
        }

        for (int i = 0; i < bootes_number; i++) {
            Prop* pk1 = new Prop(":/figs/boots.jpg", BOOTS);
            scene->addItem(pk1);

            QPair p = randomPos(850);
            pk1->setPos(p.first, p.second);
            Character::connect(scene, &MySence::propPicked, pk1, &Prop::handlePicked);
        }

        for (int i = 0; i < BUSH_NUM; i++) {
            Bush* bush = new Bush(":/figs/bush.jpg");
            scene->addItem(bush);
            QPair p = randomPos(600);
            bush->setPos(p.first, p.second);
        }
    }

    QPair<int, int> randomPos(const double ra)
    {
        // 圆心坐标 (1500, 1500)，半径 900
        const double centerX = 1500.0;
        const double centerY = 1500.0;
        const double radius = ra;

        // 生成随机角度（0~2π）
        double angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;

        // 生成随机半径（需开平方保证均匀性）
        double r = radius * qSqrt(QRandomGenerator::global()->generateDouble());

        // 转换为笛卡尔坐标
        double x = centerX + r * qCos(angle);
        double y = centerY + r * qSin(angle);
        return {x, y};
    }
};

#endif // PROPFACTORY_H
