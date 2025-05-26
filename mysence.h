#ifndef MYSENCE_H
#define MYSENCE_H
#include <QDebug>
#include <QGraphicsScene>
#include <QObject>
#include <Qpainter>
#include "aimline.h"
#include "char.h"
#include "prop.h"

#define AINUM 5

//这个要替换main中的scene
class MySence : public QGraphicsScene
{
    Q_OBJECT
    QTimer* timer;
    QTimer* character_attack_timer;
    QTimer* aimTimer;

public:
    MySence();
    bool areItemsClose(QGraphicsItem* item1, QGraphicsItem* item2, float threshold);
    int return_char_distance_squre(Character* c1, Character* c2)
    {
        QPointF pos1 = c1->mapToScene(c1->boundingRect().center());
        QPointF pos2 = c2->mapToScene(c2->boundingRect().center());
        float dx = pos1.x() - pos2.x();
        float dy = pos1.y() - pos2.y();
        return (dx * dx + dy * dy);
    }

    AimLine aimline;

    QVector<AimLine*> aimlines;
private slots:
    void checkDistance();
    void checkCharacterDistance();
    void getAimedChar();
    void resetAimLine();
signals:
    void propPicked(Prop* p);
};

#endif // MYSENCE_H
