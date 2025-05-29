#ifndef AIMLINE_H
#define AIMLINE_H
#include <QColor>
#include <QGraphicsItem>
#include <QLineF>
#include <QPainter>
#include <QPen>

#include <QPropertyAnimation>

#define CENTERBIAS 80;

class AimLine : public QGraphicsObject
{
    Q_OBJECT;

public:
    int id;
    explicit AimLine(const QPointF& start = QPointF(),
                     const QPointF& end = QPointF(),
                     QGraphicsItem* parent = nullptr);

    // 设置起点和终点
    void setStart(const QPointF& start);

    void setEnd(const QPointF& end);

    // 方案2：同时设置起点和终点的函数
    void setLine(const QPointF& start, const QPointF& end);

    // 必须重写的函数：定义绘制区域
    QRectF boundingRect() const override;

    // 绘制函数
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // 方案3：重写shape()函数以获得更精确的碰撞检测
    QPainterPath shape() const override;

    void set_start(QPoint start) { setStart(start); }

    void set_end(QPoint end) { setEnd(end); }

    bool is_player = false;
public slots:
    void createKnifeAnimation();

private:
    QPointF m_start = {0, 0}; // 起点
    QPointF m_end = {0, 0};   // 终点
    QPen m_pen;               // 画笔（控制虚线样式）
    QPixmap* knife;
};

#endif // AIMLINE_H
