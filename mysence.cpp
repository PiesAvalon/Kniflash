#include "mysence.h"
#include "player.h"

MySence::MySence()
{
    QPixmap backgroundPixmap(":/figs/background.png");
    if (backgroundPixmap.isNull()) {
        // 时常使用 qDebug 输出调试信息, 是好的开发习惯
        qDebug() << "Failed to load background image!";
    }
    const int bgSize = 1800;    // 圆形背景的直径（保持不变）
    const int sceneSize = 3000; // 更大的场景尺寸

    // 缩放背景图片（保持比例扩展）
    QPixmap scaledBg = backgroundPixmap.scaled(bgSize,
                                               bgSize,
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::SmoothTransformation);

    // 创建透明圆形背景
    QPixmap circularBg(bgSize, bgSize);
    circularBg.fill(Qt::transparent);

    // 绘制圆形背景
    {
        QPainter painter(&circularBg);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        // 设置圆形裁剪区域
        QPainterPath path;
        path.addEllipse(0, 0, bgSize, bgSize);
        painter.setClipPath(path);

        // 居中绘制缩放后的背景
        painter.drawPixmap((bgSize - scaledBg.width()) / 2,
                           (bgSize - scaledBg.height()) / 2,
                           scaledBg);
    }

    // 将圆形背景添加到场景并居中
    QGraphicsPixmapItem *bgItem = addPixmap(circularBg);
    bgItem->setPos((sceneSize - bgSize) / 2, (sceneSize - bgSize) / 2); // 关键居中代码

    // 设置场景大小
    setSceneRect(0, 0, sceneSize, sceneSize);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MySence::checkDistance);
    timer->start(10); // 每10ms检测一次

    character_attack_timer = new QTimer(this);
    connect(character_attack_timer, &QTimer::timeout, this, &MySence::checkCharacterDistance);
    character_attack_timer->start(500);

    aimTimer = new QTimer(this);
    connect(aimTimer, &QTimer::timeout, this, [this]() {
        this->getAimedChar();
        resetAimLine();
    });
    aimTimer->start(16); // 约60fps的更新频率，减少闪烁

    addItem(&aimline);
    aimline.is_player = true;
    for (int i = 0; i < AINUM; i++) {
        auto new_aim = new AimLine();
        aimlines.push_back(new_aim);
        addItem(new_aim);
    }
}

bool MySence::areItemsClose(QGraphicsItem *item1, QGraphicsItem *item2, float threshold)
{
    QPointF pos1 = item1->mapToScene(item1->boundingRect().center());
    QPointF pos2 = item2->mapToScene(item2->boundingRect().center());
    float dx = pos1.x() - pos2.x();
    float dy = pos1.y() - pos2.y();
    return (dx * dx + dy * dy) < (threshold * threshold);
}

void MySence::checkDistance()
{
    // 获取场景中所有Item
    QList<QGraphicsItem *> allItems = this->items();

    // 分离出所有Character和Prop类对象
    QList<QGraphicsItem *> characterItems; // 改为存储多个Character
    QList<QGraphicsItem *> propItems;

    for (QGraphicsItem *item : allItems) {
        if (item->type() == Character::Type) {
            characterItems.append(item); // 收集所有Character
        } else if (item->type() == Prop::Type) {
            propItems.append(item);
        }
    }

    if (characterItems.isEmpty())
        return; // 没有Character对象时提前退出

    const float threshold = 50.0f; // 距离阈值

    // 对每个Character进行操作
    for (QGraphicsItem *characterItem : characterItems) {
        Character *character = dynamic_cast<Character *>(characterItem);
        if (!character)
            continue; // 安全类型检查

        // 检测当前Character与所有Prop的距离
        for (QGraphicsItem *propItem : propItems) {
            Prop *prop = dynamic_cast<Prop *>(propItem);
            if (!prop)
                continue; // 安全类型检查

            if (areItemsClose(characterItem, propItem, threshold)) {
                if (!prop->get_picked()) {
                    int id = prop->get_id();
                    character->handle_pick(id); // 每个Character独立处理
                }
                emit propPicked(prop); // 可以保留原信号发射逻辑
            }
        }
    }
}

void MySence::checkCharacterDistance()
{
    QList<QGraphicsItem *> allItems = this->items();

    // 步骤1：提取所有 Character 对象
    QList<Character *> characters;
    for (QGraphicsItem *item : allItems) {
        if (item->type() == Character::Type) {
            if (Character *ch = dynamic_cast<Character *>(item)) {
                characters.append(ch);
            }
        }
    }

    // 步骤2：创建已处理对象集合
    QSet<Character *> processed;

    // 步骤3：遍历所有唯一对象对 (i < j 避免重复检测)
    for (int i = 0; i < characters.size(); ++i) {
        Character *char1 = characters[i];
        for (int j = i + 1; j < characters.size(); ++j) {
            Character *char2 = characters[j];

            // 计算攻击距离之和
            float threshold = char1->get_near_attack_range() + char2->get_near_attack_range();

            if (areItemsClose(char1, char2, threshold) && !char1->dead && !char2->dead) {
                // 对 char1 执行 be_hit（如果未处理过）
                if (!processed.contains(char1)) {
                    char1->be_hit(); // 修改点1：方法名替换
                    processed.insert(char1);
                }

                // 对 char2 执行 be_hit（如果未处理过）
                if (!processed.contains(char2)) {
                    char2->be_hit(); // 修改点2：方法名替换
                    processed.insert(char2);
                }
            }
        }
    }
}

void MySence::getAimedChar()
{
    QList<QGraphicsItem *> allItems = this->items();

    // 步骤1：提取所有 Character 对象
    QList<Character *> characters;
    for (QGraphicsItem *item : allItems) {
        if (item->type() == Character::Type) {
            if (Character *ch = dynamic_cast<Character *>(item)) {
                characters.append(ch);
            }
        }
    }

    for (int i = 0; i < characters.size(); i++) {
        Character *char1 = characters[i];
        int min_dis_square = char1->get_aim_range() * char1->get_aim_range();
        for (int j = 0; j < characters.size(); ++j) {
            Character *char2 = characters[j];
            if (char2 == char1) {
                continue;
            }

            if (!char1->dead && !char2->dead
                && return_char_distance_squre(char1, char2) < min_dis_square && char1 != char2) {
                min_dis_square = return_char_distance_squre(char1, char2);
                char1->aim_target = char2;
                if (char1->aim_target) {
                }
            }
        }
        if (min_dis_square == char1->get_aim_range() * char1->get_aim_range()) {
            char1->aim_target = nullptr;
        }
        aimTimer->start(10);
    }
}

void MySence::resetAimLine()
{
    QList<QGraphicsItem *> allItems = this->items();
    Player *player = nullptr;
    for (QGraphicsItem *item : allItems) {
        if (item->type() == Player::Type) {
            if (Player *ch = dynamic_cast<Player *>(item)) {
                player = ch;
                break;
            }
        }
    }

    if (player) {
        aimline.setStart(player->pos());
        if (player->aim_target) {
            aimline.setEnd(player->aim_target->pos());
        } else {
            aimline.setEnd(player->pos());
        }
    }

    QVector<Character *> characters;

    for (QGraphicsItem *item : allItems) {
        // 使用 qgraphicsitem_cast 安全转换
        Character *character = qgraphicsitem_cast<Character *>(item);
        if (character) {
            characters.append(character);
        }
    }

    for (int i = 0; i < AINUM; i++) {
        Character *c = characters[i];
        if (c) {
            aimlines[i]->setStart(c->pos());
            if (c->aim_target) {
                aimlines[i]->setEnd(c->aim_target->pos());
            } else {
                aimlines[i]->setEnd(c->pos());
            }
        }
    }
}
