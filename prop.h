#ifndef PROP_H
#define PROP_H

#include <QFile>
#include <QGraphicsObject>
#include <QPainter>

#define KNIFE 1472
#define HEALTH 1842
#define BOOTS 1253

class Prop : public QGraphicsObject
{
    Q_OBJECT

public:
    Prop(const QString &imagePath = "",
         int prop_class = 0,
         QSize targetSize = QSize(100, 100),
         QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    bool get_picked() { return picked; }

    int get_id()
    {
        return m_prop_class;
    }

private:
    QPixmap *m_pixmap; // 存储贴图
    bool picked = false;
    int m_prop_class;
public slots:
    void handlePicked(Prop *p);
};

#endif // PROP_H
