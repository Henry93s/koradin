#ifndef CHATTINGLOGWIDGET_H
#define CHATTINGLOGWIDGET_H

#include <QWidget>

class QPushButton;
class QFile;

class ChattingLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChattingLogWidget(const QString& userName, const QString& Message, const QString& fileName = QString(), QWidget *parent = nullptr);

    const QPushButton* GetDownloadButton() const { return fileDownloadButton; }
private:
    QPushButton* fileDownloadButton;
signals:
};

#endif // CHATTINGLOGWIDGET_H
