#include "bookitem.h"
#include "ui_bookitem.h"
#include <QPixmap>

BookItem::BookItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookItem)
{
    ui->setupUi(this);
}

BookItem::~BookItem()
{
    delete ui;
}

void BookItem::setData(Book* book)
{
    QImage qimage = book->getQImagefromQString(book->getQImagePureQString());
    ui->book_title_label->setText(book->getName());
    ui->book_info_label->setText(book->getWriter() + " | " + book->getCompany());
    ui->book_image_label->setPixmap(QPixmap::fromImage(qimage));
    ui->book_context_label->setText(book->getContext());
    ui->book_amount_label->setText(QString::number(book->getAmount()));
    ui->book_price_label->setText(QString::number(book->getPrice()));
}
