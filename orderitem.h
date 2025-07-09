#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QWidget>
#include "bookitem.h"
#include "bluerayitem.h"
#include "musicitem.h"

namespace Ui {
class OrderItem;
}

class OrderItem : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItem(QWidget *parent = nullptr);
    ~OrderItem();

private:
    Ui::OrderItem *ui;
};

#endif // ORDERITEM_H
