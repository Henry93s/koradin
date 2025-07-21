#include "addAtUserList.h"

#include <QListWidget>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>

#include "chattinglogwidget.h"

void AddAtUserList(QWidget* parent, QListWidget* userList, ClientData* cla)
{
    //userList->addItem(cla.name);
    UserItem* userItem = new UserItem(userList);
    userItem->Initialize(cla);
    // QListWidget 에 userItem 을 각각 채워넣음
    // 한 userItem 의 자리를 마련
    QListWidgetItem* item = new QListWidgetItem(userList);
    // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
    item->setSizeHint(userItem->sizeHint());
    // 리스트 위젯에 아이템 추가
    userList->addItem(item);
    // 생성된 틀에 커스텀 위젯을 설정
    userList->setItemWidget(item, userItem);
    // 걍 clientData에서 삭제시 추격.
    cla->listAndItem.push_back({userList, item});
}

void DeleteAtUserList(QListWidget* userList, ClientData* cla)
{
    // 클라이언트 데이터가 추적하는 리스트위젯을 순회
    for(auto& iter : cla->listAndItem){
        // 만약 리스트위젯이 맞으면
        if(iter.list == userList){
            // 리스트 위젯의 아이템을 순회하고 맞으면 삭제.
            for(int i = 0; i < userList->count(); ++i){
                QListWidgetItem* item = userList->item(i);
                if(iter.item == item){
                    userList->removeItemWidget(item);
                    delete userList->takeItem(i);
                    break;
                }
            }
        }
    }
}

void DeleteAtUserList(QListWidget* userList, const QString& name)
{
    for(int i = 0; i < userList->count(); ++i){
        QListWidgetItem* item = userList->item(i);
        if(item->text() == name){
            auto* itemToDelete = userList->takeItem(i);
            delete itemToDelete;
            break;
        }
    }
}

void AddAtChattingList(QWidget* parent, QListWidget *chatList, const QString &userName, const QString &chat, const QByteArray& fileBytes, const QString& fileName)
{
    ChattingLogWidget* log = new ChattingLogWidget(userName, chat, fileName, chatList);

    QListWidgetItem* item = new QListWidgetItem(chatList);
    item->setSizeHint(log->sizeHint());
    chatList->addItem(item);
    chatList->setItemWidget(item, log);

    if(log->GetDownloadButton()){
        QObject::connect(log->GetDownloadButton(), &QPushButton::clicked, chatList, [parent, fileBytes, fileName](){
            if(!fileBytes.isEmpty()){
                QString saveFile = QFileDialog::getSaveFileName(parent, QString("저장할 폴더 선택"));

                if (!saveFile.isEmpty()) {
                    QFile newFile(saveFile);
                    if(newFile.open(QFile::WriteOnly)){
                        QTextStream out(&newFile);
                        out << fileBytes;
                        newFile.close();
                    }
                }
            }
        });
    }
}
