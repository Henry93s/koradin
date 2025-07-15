#ifndef ADDATUSERLIST_H
#define ADDATUSERLIST_H

#include "useritem.h"

class QListWidget;
class QWidget;

extern void AddAtUserList(QWidget* parent, QListWidget* userList, ClientData* cla);
extern void DeleteAtUserList(QListWidget* userList, ClientData* cla);

#endif // ADDATUSERLIST_H
