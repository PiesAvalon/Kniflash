#include "char.h"
#include "prop.h"
#define INIT_KNIFE_R 120

#include <memory>

Character::Character(QGraphicsItem *parent)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    movie = new QMovie(":/figs/capoo.gif");
    movie->setCacheMode(QMovie::CacheAll);
    label = new QLabel();
    label->setMovie(movie);
    movie->setScaledSize(QSize(160, 135));
    movie->start();
    movie->setPaused(true);
    connect(movie, &QMovie::frameChanged, this, [this](int frame) {
        if (frame != -1)
            update();
    });
    connect(movie, &QMovie::finished, this, [this]() {
        movie->jumpToFrame(0);
    });

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, [this]() {
        QPointF direction(0, 0);
        foreach (int key, pressedKeys) {
            if (key == Qt::Key_A)
                direction.rx() -= 1;
            else if (key == Qt::Key_D)
                direction.rx() += 1;
            else if (key == Qt::Key_W)
                direction.ry() -= 1;
            else if (key == Qt::Key_S)
                direction.ry() += 1;
            else if (key == Qt::Key_C) {
                drop_health(1);
            } else if (key == Qt::Key_X) {
                pop_knife();
            } else if (key == Qt::Key_Space) {
                shoot();
            }
        }
        if (!direction.isNull()) {
            direction /= sqrt(direction.x() * direction.x() + direction.y() * direction.y());
            setPos(x() + speed * direction.x(), y() + speed * direction.y());
            emit position_changed();
        }
    });

    health = 100;
    for (int i = 0; i < 5; i++) {
        QPixmap *ph = new QPixmap(":/figs/heart.jpg");
        if (!ph->isNull()) {
            // *ph = ph->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            hearts.push_back(ph);
        }
    }

    knife_num = 4;
    for (int i = 0; i < knife_num; i++) {
        QPixmap *pm = new QPixmap(":/figs/knife.jpg");
        if (!pm->isNull()) {
            *pm = pm->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            knifes.push_back(pm);
        }
    }

    m_rotateTimer = new QTimer(this);
    connect(m_rotateTimer, &QTimer::timeout, [this]() {
        m_rotationAngle += 2.0; // 调整数值控制速度
        if (m_rotationAngle >= 360)
            m_rotationAngle -= 360;
        update(); // 触发重绘
    });

    m_rotateTimer->start(10);

    moveTimer->start(16); // 约60FPS

    speed_timer = new QTimer(this);
    connect(speed_timer, &QTimer::timeout, this, &Character::speed_finished);
    connect(moveTimer, &QTimer::timeout, this, &Character::control_hearts);

    QPixmap *boots_lable = new QPixmap(":/figs/boots.jpg");
    if (!boots_lable->isNull()) {
        *boots_lable = boots_lable->scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    buff_lables.push_back(boots_lable);

    knife_r = INIT_KNIFE_R;

    give_knife_timer = new QTimer;
    give_knife_timer->start(3000);

    connect(give_knife_timer, &QTimer::timeout, this, &Character::handle_give_knife_timer);

    dead_image = QPixmap(":/figs/dead.jpg");
    connect(this, &Character::Dead_signal, this, &Character::handle_dead);

    aim_range = 500;
    attack_cooldown = new QTimer;
    // attack_cooldown->start(ATTACK_COOLDOWN);
    connect(attack_cooldown, &QTimer::timeout, this, [this]() {
        ready_to_attack = true;
        attack_cooldown->stop();
    });
}

void Character::handle_give_knife_timer()
{
    if (knife_num < 4) {
        push_knife();
    }
    give_knife_timer->start(3000);
    return;
}

QVariant Character::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();

        // 计算到中心点(1500,1500)的距离
        qreal dx = newPos.x() - 1420;
        qreal dy = newPos.y() - 1420;
        qreal distance = sqrt(dx * dx + dy * dy);

        // 如果距离超过900，将位置限制在半径为900的圆上
        if (distance > 900) {
            qreal scale = 900 / distance;
            QPointF center(1420, 1420);
            newPos = center + QPointF(dx * scale, dy * scale);
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Character::boundingRect() const
{
    // return QRectF(0, 0, 160, 135); // 根据GIF尺寸调整
    return QRectF(-170, -180, 500, 500);
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->fillRect(boundingRect(), Qt::transparent);
    if (dead) {
        painter->drawPixmap(0, 0, dead_image);
        return;
    }
    painter->drawPixmap(0, 0, movie->currentPixmap());

    // int m_rotationAngle = 0;
    // 启用抗锯齿和平滑贴图变换（网页5、网页8）
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // 获取角色中心点（网页3）
    QPointF center = boundingRect().center();

    // 遍历所有贴图
    for (int i = 0; i < knifes.size(); ++i) {
        painter->save(); // 保存当前坐标系状态

        // 坐标系变换（网页3、网页8）
        painter->translate(center.x(), center.y());                     // 移动坐标系到中心
        painter->rotate(m_rotationAngle + (360.0 / knifes.size()) * i); // 均匀分布角度

        // 计算贴图绘制位置（距离中心的半径）
        // const int radius = 150; // 环绕半径
        const int radius = knife_r; //环绕半径
        QPixmap *knife = knifes[i];
        QPointF drawPos(-knife->width() / 2, -radius - knife->height() / 2);

        painter->drawPixmap(drawPos, *knife);

        painter->restore(); // 恢复坐标系状态

        if (high_speed && !buff_lables.empty()) {
            // 获取第一个QPixmap
            QPixmap *pixmap = buff_lables.first();

            // 计算中央位置
            QRectF rect = boundingRect();
            qreal x = rect.width() / 2 - pixmap->width() / 2 - 200;
            qreal y = rect.height() / 2 - pixmap->height() / 2 - 200;

            // 绘制QPixmap
            painter->drawPixmap(x, y, *pixmap);

            // 可选：添加半透明效果
            painter->setOpacity(0.7); // 设置透明度
            painter->drawPixmap(x, y, *pixmap);
            painter->setOpacity(1.0); // 恢复透明度
        }
    }

    // 获取图形项的有效绘制区域
    qreal itemWidth = boundingRect().width();
    int count = hearts.size();
    if (count == 0)
        return;

    // 计算布局参数
    qreal spacing = 100 / (count + 1);
    qreal pixmapWidth = 20;
    qreal yPos = 10; // 相对于图形项顶部的偏移

    for (int i = 0; i < count; ++i) {
        // 计算水平位置（基于图形项坐标系）
        qreal xPos = 30 + spacing * (i + 1) - pixmapWidth / 2;

        // 保持宽高比缩放
        QPixmap scaled = hearts[i]->scaled(pixmapWidth,
                                           pixmapWidth,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
        painter->drawPixmap(xPos, yPos, scaled);
    }
}

void Character::push_knife()
{
    knife_num++;
    QPixmap *pm = new QPixmap(":/figs/knife.jpg");
    if (!pm->isNull()) {
        *pm = pm->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    knifes.push_back(pm);
    if (knife_num > 4) {
        knife_r = 5 * (knife_num - 4) + INIT_KNIFE_R;
    } else {
        knife_r = INIT_KNIFE_R;
    }
    if (knife_r > 200) {
        knife_r = 200;
    }
}

void Character::pop_knife()
{
    if (knife_num > 0) {
        knife_num--;
        knifes.pop_back();
    }
    if (knife_num > 4) {
        knife_r = 5 * (knife_num - 4) + INIT_KNIFE_R;
    } else {
        knife_r = INIT_KNIFE_R;
    }
    if (knife_r > 200) {
        knife_r = 200;
    }
}

void Character::add_health(int amount)
{
    if (amount == 20) {
        health += 20;
        if (health >= 100) {
            health = 100;
        }
    }
}



void Character::drop_health(int amount)
{
    health -= amount;
    if (health <= 0) {
        emit Dead_signal();
    }
}

void Character::picked_boots()
{
    speed_timer->start(5000);
    if (!high_speed) {
        speed *= 2;
    }
    high_speed = true;
}

void Character::handle_pick(int id)
{
    if (id == KNIFE) {
        push_knife();
    } else if (id == HEALTH) {
        add_health(20);
    } else if (id == BOOTS) {
        picked_boots();
    }
}
