#include "customerswidget.h"
#include "ui_customerswidget.h"

#include "userManager.h"
#include "MapToVector.h"

CustomersWidget::CustomersWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomersWidget)
{
    ui->setupUi(this);

    connect(ui->userSearchButton, &QPushButton::clicked, this, &CustomersWidget::userNameSearch);

    userNameSearch();
}

CustomersWidget::~CustomersWidget()
{
    delete ui;
}

QListWidget *CustomersWidget::GetUserList()
{
    return ui->userList;
}

void CustomersWidget::userNameSearch()
{
    ui->userList->clear();

    QString name = ui->userNameSearchText->toPlainText();

    auto users = MapToVector(UserManager::getInstance()->userListRead());

    std::vector<UserInfo*> usersToPush;
    for(auto iter : users){
        if(name.isEmpty()){
            usersToPush.push_back(iter);
        }
        else if(iter->getName().contains(name)){
            usersToPush.push_back(iter);
        }
    }
    for(auto iter : usersToPush){
        ui->userList->addItem(iter->getName() + QString(" ID:") + iter->getID());
    }
}
