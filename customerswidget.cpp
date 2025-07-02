#include "customerswidget.h"
#include "ui_customerswidget.h"

CustomersWidget::CustomersWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomersWidget)
{
    ui->setupUi(this);
}

CustomersWidget::~CustomersWidget()
{
    delete ui;
}
