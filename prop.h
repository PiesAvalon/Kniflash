#ifndef PROP_H
#define PROP_H

#include <QFile>
#include <QGraphicsObject>
#include <QPainter>

class Prop : public QGraphicsObject
{
    Q_OBJECT

public:
    Prop(const QString &imagePath = "",
         QSize targetSize = QSize(100, 100),
         QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent)
    {
        // 加载原始图片并立即缩放
        QPixmap original(imagePath);
        if (!original.isNull()) {
            m_pixmap = new QPixmap(
                original.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qWarning() << "Failed to load image:" << imagePath;
            m_pixmap = new QPixmap(targetSize);
            m_pixmap->fill(Qt::transparent);
        }
    }

    // 重写基类的纯虚函数
    QRectF boundingRect() const override
    {
        return QRectF(QPointF(0, 0),
                      m_pixmap->size()); //boundingRect由自身坐标出发做参考，而非场景坐标
    }
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override
    {
        if (m_pixmap && !m_pixmap->isNull()) {
            painter->drawPixmap(boundingRect().topLeft(), *m_pixmap);
        }
    }

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

private:
    QPixmap *m_pixmap; // 存储贴图
    bool picked = false;
public slots:
    void handlePicked(Prop *p)
    {
        if (p == this && !picked) {
            picked = true;
            m_pixmap->fill(Qt::transparent); //透明
            update();
        }
    }
};

#endif // PROP_H
