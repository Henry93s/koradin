#include "productinfo.h"

ProductInfo::ProductInfo(){}
ProductInfo::~ProductInfo(){}

QString ProductInfo::getCompany(){
    return this->company;
}
void ProductInfo::setCompany(const QString& company){
    this->company = company;
}
int ProductInfo::getPrice(){
    return this->price;
}
void ProductInfo::setPrice(int price){
    this->price = price;
}
QString ProductInfo::getContext(){
    return this->context;
}
void ProductInfo::setContext(const QString& context){
    this->context = context;
}

int ProductInfo::getAmount(){
    return this->amount;
}
void ProductInfo::setAmount(int amount){
    this->amount = amount;
}

// QString -- base64 인코딩 --> QImage
QImage ProductInfo::getQImagefromQString(const QString& imageString) {
    // Base64 디코딩 후 압축 해제
    QByteArray decompressed = QByteArray::fromBase64(imageString.toLatin1());

    // 이미지로 복원
    QImage image;
    image.loadFromData(decompressed, "PNG");
    return image;
}
// image -- base64 디코딩 --> QString -> set
void ProductInfo::setQImageToQString(const QImage& image) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG");

    this->image = QString::fromLatin1(byteArray.toBase64());
}
// return pure QString QImage(base64)
QString ProductInfo::getQImagePureQString(){
    return this->image;
}

void ProductInfo::setQImagePureQString(const QString& image){
    this->image = image;
}
