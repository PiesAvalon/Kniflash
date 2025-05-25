#include "prop.h"

Prop::Prop(const QString &imagePath, int prop_class, QSize targetSize, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    m_prop_class = prop_class;
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

QRectF Prop::boundingRect() const
{
    return QRectF(QPointF(0, 0),
                  m_pixmap->size()); //boundingRect由自身坐标出发做参考，而非场景坐标
}

void Prop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_pixmap && !m_pixmap->isNull()) {
        painter->drawPixmap(boundingRect().topLeft(), *m_pixmap);
    }
}

void Prop::handlePicked(Prop *p)
{
    if (p == this && !picked) {
        picked = true;
        m_pixmap->fill(Qt::transparent); //透明
        update();
    }
}
