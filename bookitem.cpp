#include "bookitem.h"
#include "ui_bookitem.h"
#include <QPixmap>

// BookItem 의 context 각 라인 제한
int ContextLineLimit = 20;
// BookItem 의 context 라인 제한
int ContextMaxLine = 3;

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
    // (qt designer : label 의 setWordWrap 을 true 속성으로 변경 - 자동 줄 바꿈)
    // 후 추가 텍스트 자르기
    QString definedText = makeContext(book);
    // 마우스를 올렸을 때는 전체 컨텍스트 내용을 볼 수 있도록 함
    ui->book_context_label->setToolTip(book->getContext());
    ui->book_context_label->setText(definedText);
    ui->book_amount_label->setText(QString::number(book->getAmount()));
    ui->book_price_label->setText(QString::number(book->getPrice()));
}

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
