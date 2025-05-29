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
    int return_char_distance_squre(Character* c1, Character* c2);

    AimLine aimline;
    QVector<AimLine*> aimlines;

    void handle_mob_death();
    int cur_ai_num;
private slots:
    void checkDistance();
    void checkCharacterDistance();
    void getAimedChar();
    void resetAimLine();
signals:
    void propPicked(Prop* p);
    void player_win_signal();
};

#endif // MYSENCE_H
