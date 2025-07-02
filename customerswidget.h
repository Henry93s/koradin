#ifndef CUSTOMERSWIDGET_H
#define CUSTOMERSWIDGET_H

#include <QWidget>

namespace Ui {
class CustomersWidget;
}

class CustomersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomersWidget(QWidget *parent = nullptr);
    ~CustomersWidget();

private:
    Ui::CustomersWidget *ui;
};

#endif // CUSTOMERSWIDGET_H
