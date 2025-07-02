#include "join.h"
#include "ui_join.h"

Join::Join(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Join)
{
    ui->setupUi(this);
}

Join::~Join()
{
    delete ui;
}
