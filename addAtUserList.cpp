#include "addAtUserList.h"

#include <QListWidget>

void AddAtUserList(QWidget* parent, QListWidget* userList, ClientData* cla)
{
    //userList->addItem(cla.name);
    UserItem* userItem = new UserItem(parent);
    userItem->Initialize(cla);
    // QListWidget 에 userItem 을 각각 채워넣음
    // 한 userItem 의 자리를 마련
    QListWidgetItem* item = new QListWidgetItem(userList);
    // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
    // qDebug() << userItem->sizeHint(); // userItem QSize 확인
    item->setSizeHint(userItem->sizeHint());
    // 리스트 위젯에 아이템 추가
    userList->addItem(item);
    // 생성된 틀에 커스텀 위젯을 설정
    userList->setItemWidget(item, userItem);
}

void DeleteAtUserList(QListWidget* userList, ClientData* cla)
{
    for (int i = 0; i < userList->count(); ++i) {
        QListWidgetItem* item = userList->item(i);
        UserItem* widget = qobject_cast<UserItem*>(userList->itemWidget(item));
        if (widget && widget->GetPointingClient() == cla) {
            userList->takeItem(i);
            break;
        }
    }
}
