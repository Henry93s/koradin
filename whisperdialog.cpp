#include "whisperdialog.h"

#include "commuInfo.h"

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>

#include "Client.h"

WhisperDialog::WhisperDialog(const QString& CounterName, Client* cla, QWidget *parent)
    : counterName{CounterName}, client{cla}, QDialog{parent}
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    line = new QLineEdit(this);
    connect(line, &QLineEdit::returnPressed, this, &WhisperDialog::sendWhisperMessage);

    layout->addWidget(line);

    auto * button = new QPushButton(tr("File"), this);
    connect(button, &QPushButton::clicked, this, [this, button](){
        if(file){
            file->close();
        }
        QString filename = QFileDialog::getOpenFileName(this);
        if(filename.isEmpty())
            return;
        QFile* newFile = new QFile(filename);
        auto ret = newFile->open(QFile::ReadOnly);
        if(ret){
            file = newFile;
            QFileInfo fileinfo(filename);
            button->setText(fileinfo.fileName());
        }
    });
    layout->addWidget(button);

    layout->setContentsMargins(5, 2, 5, 2); // 여백 설정 (선택사항)
    layout->setSpacing(8); // 위젯 사이 간격

    setLayout(layout);
}

void WhisperDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 우리가 처리하므로 기본 QDialog 동작은 무시
        //event->accept();
    } else {
        QDialog::keyPressEvent(event);
    }
}

bool WhisperDialog::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            // 무시하고 우리가 직접 처리했으므로 accept 호출 방지
            return true;
        }
    }
    return QDialog::event(event);
}

void WhisperDialog::sendWhisperMessage()
{
    CommuInfo info;
    info.SetChat(client->getClientData()->name, line->text(), file, ChattingType::Whisper, counterName);
    //info.AddSizePacket();
    info.AddSizePacket();
    client->writeSocket(info.GetByteArray());
    this->close();
    this->deleteLater();
}
