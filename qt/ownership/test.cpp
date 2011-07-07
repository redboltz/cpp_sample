#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDebug>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/foreach.hpp>
#include <QGraphicsItem>

namespace {

struct MyQGraphicsRectItem:QGraphicsRectItem {
    MyQGraphicsRectItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 )
        :QGraphicsRectItem(x, y, width, height, parent), id(++master) {
        qDebug() << "MyQGraphicsRectItem():" << id;
    }
    ~MyQGraphicsRectItem() {
        qDebug() << "~MyQGraphicsRectItem():" << id;
    }
    int id;
    static int master;
};

int MyQGraphicsRectItem::master;

struct my_exp:std::exception {};

void test1() {
    qDebug() << "test1()";
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
    qDebug() << "test2()";
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
    qDebug() << "test3()";
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
    qDebug() << "test4()";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // Don't do anything here.
    scene.addItem(item1);
    // item2 is owned by item1.
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
}

void test5() {
    qDebug() << "test5()";
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // item2 is owned by item1.
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
    delete item1;
}

void test6() {
    qDebug() << "test6()";
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // item2 is owned by item1.
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
    // item2 is no longer owned by item1. Hence item2 leaks.
    item2->setParentItem(0);
    delete item1;
}

void test7() {
    qDebug() << "test7()";
    QGraphicsScene scene;
    QGraphicsRectItem* item1 = new MyQGraphicsRectItem(10, 10, 100, 100);
    // Don't do anything here.
    scene.addItem(item1);
    QGraphicsRectItem* item2 = new MyQGraphicsRectItem(20, 20, 80, 80, item1);
    // item2's ownership moves to scene from item1. item2 moves the top level of the scene.
    item2->setParentItem(0);
}

void test8() {
    qDebug() << "test8()";
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



template <class T>
struct UnmanagedQGraphicsItem:T {
    BOOST_STATIC_ASSERT((boost::is_base_of<QGraphicsItem, T>::value));
#if defined(BOOST_HAS_VARIADIC_TMPL) && defined(BOOST_HAS_RVALUE_REFS)
    template <class... Args>
    UnmanagedQGraphicsItem(Args && ... args):T(args) {}
#else
    template <class A1>
    UnmanagedQGraphicsItem(A1 const& a1):T(a1) {}
    template <class A1, class A2>
    UnmanagedQGraphicsItem(A1 const& a1, A2 const& a2):T(a1, a2) {}
    template <class A1, class A2, class A3>
    UnmanagedQGraphicsItem(A1 const& a1, A2 const& a2, A3 const& a3):T(a1, a2, a3) {}
    template <class A1, class A2, class A3, class A4>
    UnmanagedQGraphicsItem(A1 const& a1, A2 const& a2, A3 const& a3, A4 const& a4):T(a1, a2, a3, a4) {}
    template <class A1, class A2, class A3, class A4, class A5>
    UnmanagedQGraphicsItem(A1 const& a1, A2 const& a2, A3 const& a3, A4 const& a4, A5 const& a5):T(a1, a2, a3, a4, a5) {}
#endif
    ~UnmanagedQGraphicsItem() {
        clearParent(*this);
    }
private:
    static void clearParent(QGraphicsItem& item) {
        BOOST_FOREACH(QGraphicsItem* childItem, item.childItems()) {
            clearParent(*childItem);
        }
        item.setParentItem(0);
    }
};


struct UnmanagedQGraphicsScene:QGraphicsScene {
    UnmanagedQGraphicsScene(QObject* parent = 0):QGraphicsScene(parent) {}
    UnmanagedQGraphicsScene (QRectF const& sceneRect, QObject* parent = 0)
        :QGraphicsScene(sceneRect, parent) {}
    UnmanagedQGraphicsScene (qreal x, qreal y, qreal width, qreal height, QObject* parent = 0)
        :QGraphicsScene(x, y, width, height, parent) {}
    ~UnmanagedQGraphicsScene() {
        BOOST_FOREACH(QGraphicsItem* item, items()) {
            removeItem(item);
        }
    }
};

void test9() {
    qDebug() << "test9()";
    typedef UnmanagedQGraphicsItem<MyQGraphicsRectItem> UnmanagedMyQGraphicsRectItem;
    typedef boost::shared_ptr<UnmanagedMyQGraphicsRectItem> UnmanagedMyQGraphicsRectItemSp;
    UnmanagedMyQGraphicsRectItemSp item1(new UnmanagedMyQGraphicsRectItem(10, 10, 100, 100));
    UnmanagedMyQGraphicsRectItem* item2(new UnmanagedMyQGraphicsRectItem(20, 20, 80, 80, &*item1));
    // item2's parent is item1. But when item1 is deleted, item2 is no longer deleted as expected.

}

void test10() {
    qDebug() << "test10()";
    UnmanagedQGraphicsScene scene;
    typedef UnmanagedQGraphicsItem<MyQGraphicsRectItem> UnmanagedMyQGraphicsRectItem;
    UnmanagedMyQGraphicsRectItem* item1(new UnmanagedMyQGraphicsRectItem(10, 10, 100, 100));
    scene.addItem(&*item1);
}

void test11() {
    qDebug() << "test11()";
    UnmanagedQGraphicsScene scene;
    typedef UnmanagedQGraphicsItem<MyQGraphicsRectItem> UnmanagedMyQGraphicsRectItem;
    typedef boost::shared_ptr<UnmanagedMyQGraphicsRectItem> UnmanagedMyQGraphicsRectItemSp;
    UnmanagedMyQGraphicsRectItemSp item1(new UnmanagedMyQGraphicsRectItem(10, 10, 100, 100));
    UnmanagedMyQGraphicsRectItemSp item2(new UnmanagedMyQGraphicsRectItem(20, 20, 80, 80, &*item1));
    scene.addItem(&*item1);
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
    test7();
    test8();
    test9();
    test10();
    test11();
    //app.exec();
}
