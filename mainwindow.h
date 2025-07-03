#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "userManager.h"
#include "musicManager.h"
#include "bluerayManager.h"
#include "bookManager.h"

class QTabWidget;

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
    void on_login_button_clicked();
    void on_join_button_clicked();

    void on_admin_button_clicked();

private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    UserManager* usermanager;
    MusicManager* musicmanager;
    BluerayManager* bluerayManager;
    BookManager* bookManager;
};
#endif // MAINWINDOW_H
