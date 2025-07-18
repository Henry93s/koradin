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
    ui->order_title_label->setText(prodName);
    ui->order_price_label->setText(QString::number(prodPrice));
}
