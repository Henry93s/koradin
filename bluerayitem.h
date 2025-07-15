#ifndef BLUERAYITEM_H
#define BLUERAYITEM_H

#include <QWidget>
#include "blueray.h"

namespace Ui {
class BluerayItem;
}

class BluerayItem : public QWidget
{
    Q_OBJECT

public:
    explicit BluerayItem(QWidget *parent = nullptr);
    ~BluerayItem();
    void setData(Blueray* blueray);
    // 소개가 길 경우 부분 자르기
    QString makeContext(Blueray* blueray);
    // 이미지에 마우스 올릴 때 툴팁에 html 주소로 인코딩된 base64 코드를 넣어서 전체 이미지가 보이도록 함
    QString makeImageToolTip(QString base64FromPng);

    // QListWidget 에서 선택된 QListWidgetItem -> bookItem 에서
    //  주문에 필요한 데이터(이름, uuid, 아티스트, 제조사, 가격) 만 가져옴
    QMap<QString, QString> getData();

private:
    Ui::BluerayItem *ui;
};

#endif // BLUERAYITEM_H
