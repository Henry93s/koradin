#include "popup.h"
#include "ui_popup.h"

Popup::Popup(QWidget *parent, QString text)
    : QDialog(parent)
    , ui(new Ui::Popup)
{
    ui->setupUi(this);
    ui->text_label->setText(text);
}

Popup::~Popup()
{
    delete ui;
}

void Popup::on_ok_button_clicked()
{
    this->close();
}

