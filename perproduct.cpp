#include "perproduct.h"
#include "ui_perproduct.h"

PerProduct::PerProduct(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PerProduct)
{
    ui->setupUi(this);
}

PerProduct::~PerProduct()
{
    delete ui;
}
