#ifndef MYSENCE_H
#define MYSENCE_H
#include <QDebug>
#include <QGraphicsScene>
#include <Qpainter>
#include "char.h"

//这个要替换main中的scene
class MySence : public QGraphicsScene
{
public:
    MySence()
    {
        QPixmap backgroundPixmap(":/figs/background.jpg");
        if (backgroundPixmap.isNull()) {
            // 时常使用 qDebug 输出调试信息, 是好的开发习惯
            qDebug() << "Failed to load background image!";
        }

        const int bgSize = 1800; // 圆形背景的直径
        QPixmap scaledBg = backgroundPixmap.scaled(bgSize,
                                                   bgSize,
                                                   Qt::KeepAspectRatioByExpanding,
                                                   Qt::SmoothTransformation);

        QPixmap circularBg(bgSize, bgSize);
        circularBg.fill(Qt::transparent);
        QPainter painter(&circularBg);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.setClipRegion(QRegion(0, 0, bgSize, bgSize, QRegion::Ellipse));
        painter.drawPixmap((bgSize - scaledBg.width()) / 2,
                           (bgSize - scaledBg.height()) / 2,
                           scaledBg);

        addPixmap(circularBg);
        setSceneRect(0, 0, bgSize, bgSize);
        //以上是初始化背景的部分
        qDebug() << "the bg is init";
        //以下添加人物
        Character* cha = new Character();
        addItem(cha);
        cha->setPos(width() / 2,
                    height() / 2); // 初始位置在场景中心
    }
};

#endif // MYSENCE_H
