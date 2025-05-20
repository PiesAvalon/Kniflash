#include <QApplication>

#include <QObject>
#include <QRandomGenerator>
#include "QMessageBox"
#include "mainwindow.h"
#include "mob.h"
#include "mysence.h"
#include "myview.h"
#include "player.h"
#include "propfactory.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    while (true) {
        MainWindow mw;
        mw.show();

        QEventLoop loop;
        QObject::connect(&mw, &MainWindow::evokeGameSignal, &loop, &QEventLoop::quit);
        loop.exec();

        // 初始化游戏场景
        MySence* scene = new MySence();

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
        QObject::connect(player, &Character::Dead_signal, &loop2, &QEventLoop::quit);
        loop2.exec();

        int result = QMessageBox::question(nullptr,
                                           "确认",
                                           "确定要退出吗？",
                                           QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            exit(0);
        } else {
            view->hide();
            continue;
        }
    }

    // 运行应用, 并以应用的返回值作为整个程序的返回值

    app.exec();
    return 0;
}
