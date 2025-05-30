#include <QApplication>

#include <QObject>
#include <QRandomGenerator>
#include "QMessageBox"
#include "mob.h"
#include "mysence.h"
#include "myview.h"
#include "player.h"
#include "propfactory.h"
#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    while (true) {
        while (true) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("游戏开始");
            msgBox.setText("欢迎");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setButtonText(QMessageBox::Yes, "开始游戏");
            msgBox.setButtonText(QMessageBox::No, "退出游戏");

            msgBox.setWindowFlags(msgBox.windowFlags() & ~Qt::WindowCloseButtonHint);

            int result = msgBox.exec();
            if (result == QMessageBox::Yes) {
                break;
            } else if (result == QMessageBox::No) {
                exit(0);
            }
        }
        // 初始化游戏场景
        QElapsedTimer timer;
        timer.start();

        // MySence* scene = new MySence();
        std::unique_ptr<MySence> scene_ptr = std::make_unique<MySence>();
        auto scene = scene_ptr.get();

        MyView* view = new MyView();
        view->setScene(scene);
        view->resize(1080, 675);
        //添加人物
        Player* player = new Player();
        scene->addItem(player);
        player->setPos(scene->width() / 2,
                       scene->height() / 2); // 初始位置在场景中心
        player->setFlag(QGraphicsItem::ItemIsFocusable);
        player->setFocus();

        QObject::connect(player, // 直接使用对象指针，保持指针类型
                         &Character::throw_knife_signal,
                         &(scene->aimline),
                         &AimLine::createKnifeAnimation);

        //添加mob
        // 创建存储Mob的容器
        QVector<Mob*> mobVec;
        mobVec.reserve(10); // 预分配内存提升性能

        // 获取场景中心坐标
        const qreal centerX = scene->width() / 2.0;
        const qreal centerY = scene->height() / 2.0;

        // 生成10个Mob
        for (int i = 0; i < 5; ++i) {
            Mob* mob = new Mob(); // 创建新Mob

            // 生成-1500到+1500范围内的随机偏移量
            qreal offsetX = QRandomGenerator::global()->bounded(-1500, 1500);
            qreal offsetY = QRandomGenerator::global()->bounded(-1500, 1500);

            // 设置随机位置（中心坐标 + 偏移量）
            mob->setPos(centerX + offsetX, centerY + offsetY);

            // 添加到场景和容器
            scene->addItem(mob);
            mobVec.push_back(mob);

            QObject::connect(mob,
                             &Character::throw_knife_signal,
                             scene->aimlines[i],
                             &AimLine::createKnifeAnimation);
            scene->aimlines[i]->id = mob->id;
            QObject::connect(mob, &Mob::mob_death_signal, scene, &MySence::handle_mob_death);
        }

        // 关闭滚动条
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->centerOn(player);
        QObject::connect(player, &Character::position_changed, [=]() { view->centerOn(player); });

        view->resetTransform();

        PropFactory* factory = new PropFactory(scene);

        view->show();

        QEventLoop loop2;
        bool win = false;
        QObject::connect(scene, &MySence::player_win_signal, &loop2, [&]() {
            win = true;
            loop2.quit();
        });
        QObject::connect(player, &Character::Dead_signal, &loop2, [&]() { loop2.quit(); });
        loop2.exec();

        int result;

        if (win) {
            int killnum = scene->kill_num;
            int time = timer.elapsed() / 1000;
            QString qs = QString("你赢了，排名：%1。击杀数量：%2。存活时间：%3秒")
                             .arg(scene->cur_ai_num + 1)
                             .arg(killnum)
                             .arg(time);

            result = QMessageBox::information(nullptr, "胜利", qs, "退回", "", "", 0, 1);
        }

        if (!win) {
            int killnum = scene->kill_num;
            int time = timer.elapsed() / 1000;
            QString qs = QString("你输了，排名：%1。击杀数量：%2。存活时间：%3")
                             .arg(scene->cur_ai_num + 1)
                             .arg(killnum)
                             .arg(time);

            int result = QMessageBox::question(nullptr, "失败", qs, "退回", "", "", 0, 1);
        }

        view->hide();
    }

    app.exec();
    return 0;
}
