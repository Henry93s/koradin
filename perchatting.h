#ifndef PERCHATTING_H
#define PERCHATTING_H

#include <QWidget>

namespace Ui {
class PerChatting;
}

class PerChatting : public QWidget
{
    Q_OBJECT

public:
    explicit PerChatting(QWidget *parent = nullptr);
    ~PerChatting();

    void Initialize(const QString& name, const QString& message);
private:
    Ui::PerChatting *ui;
};

#endif // PERCHATTING_H
