#include "char.h"
#include "prop.h"

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
        qDebug() << "finishtd";
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
        }
        if (!direction.isNull()) {
            direction /= sqrt(direction.x() * direction.x() + direction.y() * direction.y());
            setPos(x() + 6 * direction.x(), y() + 6 * direction.y());
            emit position_changed();
        }
    });
    moveTimer->start(16); // 约60FPS
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
    return QRectF(0, 0, 160, 135); // 根据GIF尺寸调整
}

void Character::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->fillRect(boundingRect(), Qt::transparent);
    painter->drawPixmap(0, 0, movie->currentPixmap());
}

void Character::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        movie->start();
        if (!pressedKeys.contains(event->key())) {
            pressedKeys.insert(event->key());
        }
    }
}

void Character::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        pressedKeys.remove(event->key());
        if (!pressedKeys.count()) {
            movie->setPaused(true);
        }
    }
}

void Character::push_knife()
{
    knife_num++;
    qDebug() << "knife pushed" << knife_num;
}

void Character::pop_knife()
{
    knife_num--;
    qDebug() << "knife poped" << knife_num;
}

void Character::add_health(int amount)
{
    qDebug() << "health add" << amount;
}

void Character::picked_boots()
{
    qDebug() << "boots picked! ";
}

void Character::handle_pick(int id)
{
    // qDebug() << "handle" << id;
    if (id == KNIFE) {
        push_knife();
    } else if (id == HEALTH) {
        add_health(50);
    } else if (id == BOOTS) {
        picked_boots();
    }
}
