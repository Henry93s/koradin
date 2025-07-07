#include "musicitem.h"
#include "ui_musicitem.h"
#include <QPixmap>
#include "commonVariable.h"

MusicItem::MusicItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicItem)
{
    ui->setupUi(this);
}

MusicItem::~MusicItem()
{
    delete ui;
}

void MusicItem::setData(Music* music)
{
    QImage qimage = music->getQImagefromQString(music->getQImagePureQString());
    ui->music_title_label->setText(music->getName());
    ui->music_info_label->setText(music->getArtist() + " | " + music->getCompany());
    // 이미지 라벨 사이즈 고정
    ui->music_image_label->setFixedSize(100, 129);
    ui->music_image_label->setPixmap(QPixmap::fromImage(qimage));
    // image tooltip 에 이미지 크게 볼 수 있도록 html 주소(base64) 추가
    ui->music_image_label->setToolTip(makeImageToolTip(music->getQImagePureQString()));
    // (qt designer : label 의 setWordWrap 을 true 속성으로 변경 - 자동 줄 바꿈)
    // 후 추가 텍스트 자르기
    QString definedText = makeContext(music);
    // 마우스를 올렸을 때는 전체 컨텍스트 내용을 볼 수 있도록 함
    ui->music_context_label->setToolTip(music->getContext());
    ui->music_context_label->setText(definedText);
    ui->music_amount_label->setText("잔여 수량 : " + QString::number(music->getAmount()));
    ui->music_price_label->setText("가격 : " + QString::number(music->getPrice()) + "원");
}

// 소개 부분 자르기
QString MusicItem::makeContext(Music* music){
    QString definedText = music->getContext();
    int totalSecondChars = ContextLineLimit * (ContextMaxLine - 1); // total 글자 수
    if(definedText.length() > totalSecondChars){
        // 표시될 부분만 자름
        QString lastLine = definedText.mid(totalSecondChars);

        if(lastLine.length()){
            // 지정된 글자 수를 넘으면 자르고 ... 을 붙임
            lastLine = lastLine.left(ContextLineLimit - 3) + "...";
        }

        // 최종 라벨에 출력할 텍스트 생성
        definedText = music->getContext().left(totalSecondChars) + lastLine;
    }
    return definedText;
}

// 이미지에 마우스 올릴 때 툴팁에 html 주소로 인코딩된 base64 코드를 넣어서 전체 이미지가 보이도록 함
QString MusicItem::makeImageToolTip(QString base64FromPng){
    QString returnQString;
    returnQString = QString("<img src='data:image/png;base64,%1'>").arg(base64FromPng);
    return returnQString;
}

// QListWidget 에서 선택된 QListWidgetItem -> musicItem 에서
//  주문에 필요한 데이터(이름, 아티스트 & 음반사, 가격) 만 가져옴
QMap<QString, QString> MusicItem::getData(){
    QMap<QString, QString> returnData;
    returnData.insert("name", this->ui->music_title_label->text());
    qDebug() << this->ui->music_title_label->text();
    QStringList parts = this->ui->music_info_label->text().split(" | ");
    QString artist = parts.at(0);
    QString company = parts.at(1);
    returnData.insert("artist", artist);
    returnData.insert("company", company);
    returnData.insert("price", this->ui->music_price_label->text());

    return returnData;
}
