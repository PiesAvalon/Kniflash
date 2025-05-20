#ifndef MOB_H
#define MOB_H
#include <QRandomGenerator64>
#include "char.h"

class Mob : public Character
{
    Q_OBJECT
public:
    Mob();
public slots:
    void random_move();
signals:
    void moved_signal();
};

#endif // MOB_H
