#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDebug>

struct MyQGraphicsRectItem:QGraphicsRectItem {
	MyQGraphicsRectItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 )
		:QGraphicsRectItem(x, y, width, height, parent), id(++master) {
		qDebug() << "MyQGraphicsRectItem():" << id << "\n";
	}
	~MyQGraphicsRectItem() {
		qDebug() << "~MyQGraphicsRectItem():" << id << \n";
	}
	int id;
	static int master;
};

int MyQGraphicsRectItem::master;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QGraphicsScene scene;
	QGraphicsRectItem* pItem1 = new MyQGraphicsRectItem(10, 10, 100, 100);
	QGraphicsRectItem* pItem2 = new MyQGraphicsRectItem(20, 20, 80, 80);
	scene.addItem(pItem1);
	pItem2->setParentItem(pItem1);
	pItem2->scene()->removeItem(pItem2);
	scene.clear();
	QGraphicsView view(&scene);
	view.show();
	app.exec();
}
