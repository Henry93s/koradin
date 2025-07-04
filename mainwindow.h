#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "userManager.h"

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

private slots:
    void on_login_button_clicked();
    void on_join_button_clicked();
    void on_code_Typed();
private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    UserManager* usermanager;
};
#endif // MAINWINDOW_H
