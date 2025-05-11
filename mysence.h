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
        const int bgSize = 1800;    // 圆形背景的直径（保持不变）
        const int sceneSize = 3000; // 更大的场景尺寸

        // 缩放背景图片（保持比例扩展）
        QPixmap scaledBg = backgroundPixmap.scaled(bgSize,
                                                   bgSize,
                                                   Qt::KeepAspectRatioByExpanding,
                                                   Qt::SmoothTransformation);

        // 创建透明圆形背景
        QPixmap circularBg(bgSize, bgSize);
        circularBg.fill(Qt::transparent);

        // 绘制圆形背景
        {
            QPainter painter(&circularBg);
            painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

            // 设置圆形裁剪区域
            QPainterPath path;
            path.addEllipse(0, 0, bgSize, bgSize);
            painter.setClipPath(path);

            // 居中绘制缩放后的背景
            painter.drawPixmap((bgSize - scaledBg.width()) / 2,
                               (bgSize - scaledBg.height()) / 2,
                               scaledBg);
        }

        // 将圆形背景添加到场景并居中
        QGraphicsPixmapItem* bgItem = addPixmap(circularBg);
        bgItem->setPos((sceneSize - bgSize) / 2, (sceneSize - bgSize) / 2); // 关键居中代码

        // 设置场景大小
        setSceneRect(0, 0, sceneSize, sceneSize);
    }
};

#endif // MYSENCE_H
