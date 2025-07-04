#include "perchatting.h"
#include "ui_perchatting.h"

PerChatting::PerChatting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PerChatting)
{
    ui->setupUi(this);
}

PerChatting::~PerChatting()
{
    delete ui;
}

void PerChatting::Initialize(const QString &name, const QString &message)
{
    ui->nameLabel->setText(name);
    ui->chattingLabel->setText(message);
}
