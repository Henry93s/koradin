#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "userManager.h"
#include "musicManager.h"
#include "bluerayManager.h"
#include "bookManager.h"
#include <QTranslator>

class QTabWidget;
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString managerKeyJsonLoad();

private slots:
    void respond();
    void on_login_button_clicked();
    void on_join_button_clicked();
    void on_code_Typed();

    void on_admin_button_clicked();

    void on_language_comboBox_activated(int index);

private:
    QString name;

    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    UserManager* usermanager;
    MusicManager* musicmanager;
    BluerayManager* bluerayManager;
    BookManager* bookManager;

    // MainWindow 클래스 내부 멤버로 QTranslator 포인터 선언
    QTranslator* translator = nullptr;

    QTcpSocket* socket = nullptr;

    quint32 expectedSize = 0;

    bool isClosed = false;

    QByteArray bytearray;
};
#endif // MAINWINDOW_H
