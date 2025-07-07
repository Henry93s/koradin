#ifndef MUSICITEM_H
#define MUSICITEM_H

#include <QWidget>
#include "music.h"

namespace Ui {
class MusicItem;
}

class MusicItem : public QWidget
{
    Q_OBJECT

public:
    explicit MusicItem(QWidget *parent = nullptr);
    ~MusicItem();
    void setData(Music* music);
    // 소개가 길 경우 부분 자르기
    QString makeContext(Music* music);
    // 이미지에 마우스 올릴 때 툴팁에 html 주소로 인코딩된 base64 코드를 넣어서 전체 이미지가 보이도록 함
    QString makeImageToolTip(QString base64FromPng);

    // QListWidget 에서 선택된 QListWidgetItem -> bookItem 에서
    //  주문에 필요한 데이터(이름, 아티스트, 음반사, 가격) 만 가져옴
    QMap<QString, QString> getData();

private:
    Ui::MusicItem *ui;
};

#endif // MUSICITEM_H
