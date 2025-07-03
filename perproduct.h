#ifndef PERPRODUCT_H
#define PERPRODUCT_H

#include <QWidget>

namespace Ui {
class PerProduct;
}

class PerProduct : public QWidget
{
    Q_OBJECT

public:
    explicit PerProduct(QWidget *parent = nullptr);
    ~PerProduct();

private:
    Ui::PerProduct *ui;
};

#endif // PERPRODUCT_H
