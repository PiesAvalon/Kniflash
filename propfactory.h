#ifndef PROPFACTORY_H
#define PROPFACTORY_H

#include <QRandomGenerator>
#include <QtMath>
#include "mysence.h"
#include "prop.h"
#define KNIFE_NUM 50

class PropFactory
{
public:
    PropFactory();
    PropFactory(MySence* scene)
    {
        int knife_number = KNIFE_NUM;
        for (int i = 0; i < knife_number; i++) {
            Prop* pk1 = new Prop(":/figs/knife.jpg", KNIFE);
            scene->addItem(pk1);
            // 圆心坐标 (1500, 1500)，半径 900
            const double centerX = 1500.0;
            const double centerY = 1500.0;
            const double radius = 900.0;

            // 生成随机角度（0~2π）
            double angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;

            // 生成随机半径（需开平方保证均匀性）
            double r = radius * qSqrt(QRandomGenerator::global()->generateDouble());

            // 转换为笛卡尔坐标
            double x = centerX + r * qCos(angle);
            double y = centerY + r * qSin(angle);

            pk1->setPos(x, y);
            Character::connect(scene, &MySence::propPicked, pk1, &Prop::handlePicked);
        }
    }
};

#endif // PROPFACTORY_H
