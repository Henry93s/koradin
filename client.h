#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include "bookManager.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    void printBookList();

private:
    Ui::Client *ui;
    BookManager* bookmanager;
};

#endif // CLIENT_H
