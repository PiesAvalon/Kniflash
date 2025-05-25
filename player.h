#ifndef PLAYER_H
#define PLAYER_H

#include "char.h"

class Player : public Character
{
public:
    Player();
    enum { Type = QGraphicsItem::UserType + 1 }; // 自定义类型值
    int type() const override { return Type; }   // 必须重写
};

#endif // PLAYER_H
