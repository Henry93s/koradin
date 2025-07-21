#ifndef WHISPERDIALOG_H
#define WHISPERDIALOG_H

#include <QDialog>
#include <QLineEdit>

class Client;
class QFile;

class WhisperDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WhisperDialog(const QString& counterName, Client* cla, QWidget *parent = nullptr);

signals:

private:
    Client* client;
    QString counterName;
    QLineEdit* line;
    QFile* file = nullptr;

    void keyPressEvent(QKeyEvent* event) override;
    bool event(QEvent* event) override;
private slots:
    void sendWhisperMessage();
};

#endif // WHISPERDIALOG_H
