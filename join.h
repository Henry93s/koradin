#ifndef JOIN_H
#define JOIN_H

#include <QWidget>
#include <QMainWindow>
#include "userManager.h"

class QTcpSocket;

namespace Ui {
class Join;
}

class Join : public QWidget
{
    Q_OBJECT

public:
    explicit Join(QMainWindow *parent = nullptr);
    ~Join();

    QString duplicateCheck();

    void Initialize(QTcpSocket* Socket);

private slots:
    void on_join_button_clicked();
    void on_cancel_button_clicked();
    void respond();

private:
    Ui::Join *ui;
    UserManager* userManagerInstance;
    QMainWindow* parentWindow; // 호출한 부모 저장을 위한 private 변수

    QTcpSocket* socket;
};

#endif // JOIN_H
