#include "chattinglogwidget.h"

#include <QPushButton>
#include <QFile>
#include <QLabel>
#include <QVBoxLayout>

ChattingLogWidget::ChattingLogWidget(const QString& userName, const QString& Message, const QString& fileName, QWidget *parent)
    : QWidget{parent}, fileDownloadButton (nullptr)
{
    // QLabel* label = new QLabel(userName + tr(" : ") + Message, this);
    // if(!fileName.isEmpty()){
    //     fileDownloadButton = new QPushButton(fileName, this);
    //     fileDownloadButton->setMaximumWidth(70);
    // }
    QHBoxLayout* layout = new QHBoxLayout(this);

    // 메시지 라벨
    QLabel* label = new QLabel(userName + tr(" : ") + Message, this);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); // 확장 가능

    setMinimumHeight(40);  // 필요시
    setMaximumHeight(60);  // 너무 커지지 않게 제한

    layout->addWidget(label);

    if (!fileName.isEmpty()) {
        fileDownloadButton = new QPushButton(this);

        // 말줄임표 적용
        QFontMetrics metrics(fileDownloadButton->font());
        QString elidedText = metrics.elidedText(fileName, Qt::ElideRight, 100);
        fileDownloadButton->setText(elidedText);
        fileDownloadButton->setToolTip(fileName);

        // 고정 너비
        fileDownloadButton->setFixedWidth(100);

        layout->addWidget(fileDownloadButton);
    }

    layout->setContentsMargins(5, 2, 5, 2); // 여백 설정 (선택사항)
    layout->setSpacing(8); // 위젯 사이 간격

    setLayout(layout);
}
