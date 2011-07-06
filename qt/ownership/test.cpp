#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDebug>

namespace {

struct MyQGraphicsRectItem:QGraphicsRectItem {
    MyQGraphicsRectItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 )
        :QGraphicsRectItem(x, y, width, height, parent), id(++master) {
        qDebug() << "MyQGraphicsRectItem():" << id << "\n";
    }
    ~MyQGraphicsRectItem() {
        qDebug() << "~MyQGraphicsRectItem():" << id << "\n";
    }
    int id;
    static int master;
};

int MyQGraphicsRectItem::master;

struct my_exp:std::exception {};

void test1() {
    qDebug() << "test1()\n";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80);

    // exception unsafe

    // item2 is owned by item1
    item2->setParentItem(item1); // It's too late.

    // item1 is owned by scene
    scene.addItem(item1); // It's too late.
}

void test2() {
    qDebug() << "test2()\n";
    try {
        QGraphicsScene scene;
        QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
        QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80);
        // exception unsafe
        throw(my_exp());

        // item2 is owned by item1.
        item2->setParentItem(item1); // It's too late.
        // item1 is owned by scene.
        scene.addItem(item1); // It's too late.
    }
    catch (...) {}
}

void test3() {
    qDebug() << "test3()\n";
    try {
        QGraphicsScene scene;
        QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
        // Don't do anything here.
        scene.addItem(item1); // item1 will delete automatically by scene.
        QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80);
        // exception unsafe
        throw(my_exp());

        // item2 is owned by item1.
        item2->setParentItem(item1); // It's too late.
    }
    catch (...) {}
}

void test4() {
    qDebug() << "test4()\n";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // Don't do anything here.
    scene.addItem(item1);
    // item2 is owned by item1.
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
}

void test5() {
    qDebug() << "test5()\n";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // Don't do anything here.
    scene.addItem(item1);
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
    // item2 is still owned by item1. Just move the top level of the scene.
    item2->setParentItem(0);
}

void test6() {
    qDebug() << "test6()\n";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // Don't do anything here.
    scene.addItem(item1);
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
    // item2 is no longer owned by item1.
    // We can access the scene via item2.
    item2->scene()->removeItem(item2);
    // item2 doesn't delete automatically.
}

} // noname

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    //app.exec();
}
