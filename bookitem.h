#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QWidget>
#include "book.h"

namespace Ui {
class BookItem;
}

class BookItem : public QWidget
{
    Q_OBJECT

public:
    explicit BookItem(QWidget *parent = nullptr);
    ~BookItem();
    void setData(Book* book);
    QString makeContext(Book* book);

private:
    Ui::BookItem *ui;
};

#endif // BOOKITEM_H
