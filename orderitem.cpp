#include "orderitem.h"
#include "ui_orderitem.h"

OrderItem::OrderItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrderItem)
{
    ui->setupUi(this);
    this->blueraymanager = BluerayManager::getInstance();
    this->bookmanager = BookManager::getInstance();
    this->musicmanager = MusicManager::getInstance();
}

OrderItem::~OrderItem()
{
    delete ui;
}

void OrderItem::setData(const QString& prodType, const QString& prodName, const int& prodPrice, const QString& prodUuid)
{
    //ui->order_title_label->setText(prodName);
    ui->order_price_label->setText(QString::number(prodPrice));

    // productType 별 구분
    if(prodType.compare("book") == 0) {
        Book* book = bookmanager->bookSearchByUuid(prodUuid);
        if (book) {
            ui->order_info_label->setText(book->getName());
        }
    }
    else if(prodType.compare("music") == 0) {
        Music* music = musicmanager->musicSearchByUuid(prodUuid);
        if (music) {
            ui->order_info_label->setText(music->getName());
        }
    }
    else if(prodType.compare("blueray") == 0) {
        Blueray* blueray = blueraymanager->blueraySearchByUuid(prodUuid);
        if (blueray) {
            ui->order_info_label->setText(blueray->getName());
        }
    }
}
