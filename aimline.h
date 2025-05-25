#ifndef AIMLINE_H
#define AIMLINE_H

#include <QGraphicsItem>
#include <QLineF>
#include <QPainter>
#include <QPen>
#include <QColor>

class AimLine : public QGraphicsObject
{
    Q_OBJECT;

public:
    explicit AimLine(const QPointF& start = QPointF(),
                     const QPointF& end = QPointF(),
                     QGraphicsItem* parent = nullptr)
        : QGraphicsObject(parent)
        , m_start(start)
        , m_end(end)
    {
        // qDebug() << "construct";
        // 初始化虚线样式
        m_pen.setStyle(Qt::DashLine);
        m_pen.setWidth(5);       // 固定宽度为5
        m_pen.setColor(Qt::red); // 可选：设置虚线颜色

        setZValue(9999999); // 设置更高的 Z 值
        setCacheMode(QGraphicsItem::DeviceCoordinateCache); // 启用缓存以提高性能
    }

    // 设置起点和终点
    void setStart(const QPointF& start)
    {
        m_start = start;
        update(); // 触发重绘
    }

    void setEnd(const QPointF& end)
    {
        m_end = end;
        update(); // 触发重绘
    }

    // 必须重写的函数：定义绘制区域
    QRectF boundingRect() const override
    {
        // 计算线段包围盒（考虑线宽）
        QLineF line(m_start, m_end);
        qreal extra = m_pen.widthF() / 2.0; // 扩展半径（5/2=2.5）
        return QRectF(line.p1(), line.p2()).normalized().adjusted(-extra, -extra, extra, extra);
    }

    // 必须重写的函数：绘制逻辑
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing); // 抗锯齿
        painter->setPen(m_pen);

        // 绘制线段
        QPainterPath path;
        auto start = m_start;
        auto end = m_end;
        int bias = 85;
        start.rx() += bias;
        start.ry() += bias;
        end.rx() += bias;
        end.ry() += bias;
        path.moveTo(start);
        path.lineTo(end);
        painter->drawPath(path);
        
        // 添加发光效果
        QPen glowPen = m_pen;
        glowPen.setColor(QColor(255, 0, 0, 50)); // 半透明红色
        glowPen.setWidth(15); // 更宽的发光效果
        painter->setPen(glowPen);
        painter->drawPath(path);
    }

private:
    QPointF m_start = {0, 0}; // 起点
    QPointF m_end = {0, 0};   // 终点
    QPen m_pen;               // 画笔（控制虚线样式）
};
#endif // AIMLINE_H
