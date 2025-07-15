#include "bookitem.h"
#include "ui_bookitem.h"
#include <QPixmap>
#include "commonVariable.h"

BookItem::BookItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookItem)
{
    ui->setupUi(this);
}

BookItem::~BookItem()
{
    delete ui;
}

void BookItem::setData(Book* book)
{
    QImage qimage = book->getQImagefromQString(book->getQImagePureQString());
    ui->book_title_label->setText(book->getName());
    ui->book_info_label->setText(book->getWriter() + " | " + book->getCompany());
    // 이미지 라벨 사이즈 고정
    ui->book_image_label->setFixedSize(100, 129);
    ui->book_image_label->setPixmap(QPixmap::fromImage(qimage));
    // image tooltip 에 이미지 크게 볼 수 있도록 html 주소(base64) 추가
    ui->book_image_label->setToolTip(makeImageToolTip(book->getQImagePureQString()));
    // (qt designer : label 의 setWordWrap 을 true 속성으로 변경 - 자동 줄 바꿈)
    // 후 추가 텍스트 자르기
    QString definedText = makeContext(book);
    // 마우스를 올렸을 때는 전체 컨텍스트 내용을 볼 수 있도록 함
    ui->book_context_label->setToolTip(book->getContext());
    ui->book_context_label->setText(definedText);
    ui->book_amount_label->setText("잔여 수량 : " + QString::number(book->getAmount()));
    ui->book_price_label->setText("가격 : " + QString::number(book->getPrice()) + "원");

    // uuid
    ui->book_uuid_label->setText(book->getUuid());
    ui->book_uuid_label->hide();
}

// 책 소개 부분 자르기
QString BookItem::makeContext(Book* book){
    QString definedText = book->getContext();
    int totalSecondChars = ContextLineLimit * (ContextMaxLine - 1); // total 글자 수
    if(definedText.length() > totalSecondChars){
        // 표시될 부분만 자름
        QString lastLine = definedText.mid(totalSecondChars);

        if(lastLine.length()){
            // 지정된 글자 수를 넘으면 자르고 ... 을 붙임
            lastLine = lastLine.left(ContextLineLimit - 3) + "...";
        }

        // 최종 라벨에 출력할 텍스트 생성
        definedText = book->getContext().left(totalSecondChars) + lastLine;
    }
    return definedText;
}

// 책 이미지에 마우스 올릴 때 툴팁에 html 주소로 인코딩된 base64 코드를 넣어서 전체 이미지가 보이도록 함
QString BookItem::makeImageToolTip(QString base64FromPng){
    QString returnQString;
    returnQString = QString("<img src='data:image/png;base64,%1'>").arg(base64FromPng);
    return returnQString;
}

// QListWidget 에서 선택된 QListWidgetItem -> bookItem 에서
// 주문에 필요한 (이름, AUTHOR, COMPANY, PRICE, UUID)를 찾음
QMap<QString, QString> BookItem::getData(){
    QMap<QString, QString> returnData;
    returnData.insert("name", this->ui->book_title_label->text());
    QStringList parts = this->ui->book_info_label->text().split(" | ");
    QString writer = parts.at(0);
    QString company = parts.at(1);
    returnData.insert("writer", writer);
    returnData.insert("company", company);
    returnData.insert("price", this->ui->book_price_label->text());
    returnData.insert("UUID", this->ui->book_uuid_label->text());

    return returnData;
}
