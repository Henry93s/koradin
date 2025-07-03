#ifndef POPUP_H
#define POPUP_H

#include <QDialog>

namespace Ui {
class Popup;
}

class Popup : public QDialog
{
    Q_OBJECT

public:
    explicit Popup(QWidget *parent, QString text);
    ~Popup();

private slots:
    void on_ok_button_clicked();

private:
    Ui::Popup *ui;
};

#endif // POPUP_H
