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
    // 책 소개가 길 경우 부분 자르기
    QString makeContext(Book* book);
    // 책 이미지에 마우스 올릴 때 툴팁에 html 주소로 인코딩된 base64 코드를 넣어서 전체 이미지가 보이도록 함
    QString makeImageToolTip(QString base64FromPng);

private:
    Ui::BookItem *ui;
};

#endif // BOOKITEM_H
