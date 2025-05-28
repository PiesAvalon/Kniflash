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
    explicit AimLine(const QPointF& start = QPointF(),
                     const QPointF& end = QPointF(),
                     QGraphicsItem* parent = nullptr)
        : QGraphicsObject(parent)
        , m_start(start)
        , m_end(end)
    {
        // 初始化虚线样式
        m_pen.setStyle(Qt::DashLine);
        m_pen.setWidth(5);                    // 固定宽度为5
        m_pen.setColor(Qt::red);              // 可选：设置虚线颜色
        setZValue(9999999);                   // 设置更高的 Z 值
        setCacheMode(QGraphicsItem::NoCache); // 禁用缓存避免残影

        QPixmap* pm = new QPixmap(":/figs/knife.jpg");
        if (!pm->isNull()) {
            *pm = pm->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            knife = pm;
        }
    }

    // 设置起点和终点
    void setStart(const QPointF& start)
    {
        // 方案1：使用prepareGeometryChange()通知几何变化
        prepareGeometryChange();
        QPointF s = start;
        s.rx() += CENTERBIAS;
        s.ry() += CENTERBIAS;
        m_start = s;
        update(); // 触发重绘
    }

    void setEnd(const QPointF& end)
    {
        // 方案1：使用prepareGeometryChange()通知几何变化
        prepareGeometryChange();
        QPointF s = end;
        s.rx() += CENTERBIAS;
        s.ry() += CENTERBIAS;
        m_end = s;
        update(); // 触发重绘
    }

    // 方案2：同时设置起点和终点的函数
    void setLine(const QPointF& start, const QPointF& end)
    {
        prepareGeometryChange();
        m_start = start;
        m_end = end;
        update();
    }

    // 必须重写的函数：定义绘制区域
    QRectF boundingRect() const override
    {
        // 计算线段包围盒（考虑线宽）
        QLineF line(m_start, m_end);
        qreal extra = m_pen.widthF() / 2.0; // 扩展半径（5/2=2.5）

        // 确保边界矩形至少有最小尺寸，避免空矩形
        QRectF rect
            = QRectF(line.p1(), line.p2()).normalized().adjusted(-extra, -extra, extra, extra);

        // 如果矩形太小，给它一个最小尺寸
        if (rect.width() < 1.0) {
            rect.setWidth(1.0);
        }
        if (rect.height() < 1.0) {
            rect.setHeight(1.0);
        }

        return rect;
    }

    // 绘制函数
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing); // 抗锯齿

        // 首先绘制红色矩形填充整个item区域
        // painter->setPen(Qt::NoPen);        // 不需要边框
        // painter->setBrush(Qt::red);        // 红色填充
        // painter->drawRect(boundingRect()); // 绘制覆盖整个item的矩形
        QPainterPath path;

        path.moveTo(m_start);
        path.lineTo(m_end);
        // painter->drawPath(path);
        QPen glowPen = m_pen;
        glowPen.setWidth(8); // 更宽的发光效果
        if (!is_player) {
            glowPen.setColor(QColor(255, 255, 0, 30)); // 纯黄色，透明度30（很浅）
        }
        painter->setPen(glowPen);
        painter->drawPath(path);
    }

    // 方案3：重写shape()函数以获得更精确的碰撞检测
    QPainterPath shape() const override
    {
        QPainterPath path;
        QRectF rect = boundingRect();
        path.addRect(rect);
        return path;
    }

    void set_start(QPoint start) { setStart(start); }

    void set_end(QPoint end) { setEnd(end); }

    bool is_player = false;
public slots:
    void createKnifeAnimation()
    {
        auto start = m_start;
        auto end = m_end;
        QGraphicsPixmapItem* knifeItem = new QGraphicsPixmapItem(*knife);
        knifeItem->setParentItem(this);
        knifeItem->setVisible(true);
        knifeItem->setZValue(this->zValue() + 1);

        // 计算旋转角度（Qt6角度方向处理）
        QLineF directionLine(start, end);
        qreal angle = directionLine.angle();

        // 调整图标方向和位置
        knifeItem->setTransformOriginPoint(knifeItem->boundingRect().center());
        knifeItem->setRotation(-angle + 90); // 调整角度偏移量以适应图标方向
        knifeItem->setPos(start);

        QVariantAnimation* moveAnim = new QVariantAnimation;
        moveAnim->setDuration(150);
        moveAnim->setStartValue(start);
        moveAnim->setEndValue(end);
        moveAnim->setEasingCurve(QEasingCurve::Linear);

        // 连接动画值变化信号以更新位置
        QObject::connect(moveAnim,
                         &QVariantAnimation::valueChanged,
                         [knifeItem](const QVariant& value) {
                             knifeItem->setPos(value.toPointF());
                         });

        // 动画结束后删除动画对象和刀图标
        QObject::connect(moveAnim, &QVariantAnimation::finished, [moveAnim, knifeItem]() {
            // 删除刀图标
            if (knifeItem->scene()) {
                // (knifeItem->scene())->removeItem(knifeItem);
            }
            delete knifeItem;

            // 删除动画对象
            moveAnim->deleteLater();
        });

        moveAnim->start();
    }

private:
    QPointF m_start = {0, 0}; // 起点
    QPointF m_end = {0, 0};   // 终点
    QPen m_pen;               // 画笔（控制虚线样式）
    QPixmap* knife;
};

#endif // AIMLINE_H
