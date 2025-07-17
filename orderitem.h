#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QWidget>
#include "bookManager.h"
#include "bluerayManager.h"
#include "musicManager.h"

namespace Ui {
class OrderItem;
}

class OrderItem : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItem(QWidget *parent = nullptr);
    ~OrderItem();
    void setData(const QString& prodType, const QString& prodName, const int& prodPrice, const QString& prodUuid);

private:
    Ui::OrderItem *ui;
    BookManager* bookmanager;
    BluerayManager* blueraymanager;
    MusicManager* musicmanager;
};

#endif // ORDERITEM_H
