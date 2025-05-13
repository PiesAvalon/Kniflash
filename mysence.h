#ifndef MYSENCE_H
#define MYSENCE_H
#include <QDebug>
#include <QGraphicsScene>
#include <QObject>
#include <Qpainter>
#include "char.h"
#include "prop.h"

//这个要替换main中的scene
class MySence : public QGraphicsScene
{
    Q_OBJECT
    QTimer* timer;

public:
    MySence();
    bool areItemsClose(QGraphicsItem* item1, QGraphicsItem* item2, float threshold);
private slots:
    void checkDistance();
signals:
    void propPicked(Prop* p);
};

#endif // MYSENCE_H
