#ifndef CUSTOMERSWIDGET_H
#define CUSTOMERSWIDGET_H

#include <QWidget>

class QListWidget;

namespace Ui {
class CustomersWidget;
}

class CustomersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomersWidget(QWidget *parent = nullptr);
    ~CustomersWidget();

    QListWidget* GetUserList();
public:
    void userNameSearch();
private:
    Ui::CustomersWidget *ui;
};

#endif // CUSTOMERSWIDGET_H
