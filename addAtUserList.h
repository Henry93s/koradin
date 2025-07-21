#ifndef ADDATUSERLIST_H
#define ADDATUSERLIST_H

#include "useritem.h"

class QListWidget;
class QWidget;

extern void AddAtUserList(QWidget* parent, QListWidget* userList, ClientData* cla);
extern void DeleteAtUserList(QListWidget* userList, ClientData* cla);
extern void DeleteAtUserList(QListWidget* userList, const QString& name);

extern void AddAtChattingList(QWidget* parent, QListWidget* chatList, const QString& userName, const QString& chat,
                              const QByteArray& fileBytes = QByteArray(), const QString& fileName = QString());

#endif // ADDATUSERLIST_H
