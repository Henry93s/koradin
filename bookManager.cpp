#include "bookManager.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDebug>
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "book.h"

using namespace std;
using json = nlohmann::json;

// singleton 인스턴스 초기화
BookManager* BookManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
BookManager* BookManager::getInstance(){
    if(!instance){
        instance = new BookManager();
    }
    return instance;
}

void BookManager::bookListJsonLoad(){
    // Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    QFile file("./../../bookList.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "bookList 파일 오픈 실패";
        return;
    }

    QTextStream in(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString jsonText = in.readAll();

    // Qstring -> standard string
    string jsonStdStr = jsonText.toStdString();

    json j = json::parse(jsonStdStr);

    for(auto book : j){
        QString name = QString::fromStdString(book["name"]);
        QString writer = QString::fromStdString(book["writer"]);
        QString company = QString::fromStdString(book["company"]);
        int price = book["price"];
        QString context = QString::fromStdString(book["context"]);
        int amount = book["amount"];
        QString image = QString::fromStdString(book["image"]);

        Book* newBook = new Book(name, writer, company, price, context, amount, image);
        // bookList stl Map 컨테이너에 저장
        bookList.insert(name, newBook);

        qDebug() << "Loaded Book name, writer : " << name << " (" << writer << ")";
    }

    file.close();
}
void BookManager::bookListJsonSave(){
    QFile file("./../../bookList.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "bookList 파일 저장 실패";
        return;
    }

    QTextStream out(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    out.setEncoding(QStringConverter::Utf8);

    json j_array = json::array();  // 최상위 배열 구조

    for (auto it = bookList.begin(); it != bookList.end(); ++it) {
        if (!it.value()) continue;
        json userObj = {
            { "name",        it.value()->getName().toStdString() },
            { "writer",      it.value()->getWriter().toStdString() },
            { "company",  it.value()->getCompany().toStdString() },
            { "price",     it.value()->getPrice() },
            { "context",   it.value()->getContext().toStdString() },
            { "amount",    it.value()->getAmount() },
            { "image",     it.value()->getQImagePureQString().toStdString()}
        };
        // json 각 요소들을 j_array 에 push
        j_array.push_back(userObj);
    }

    // JSON 텍스트 생성
    QString jsonText = QString::fromStdString(j_array.dump(4)); // 4칸 들여쓰기 pretty print
    out << jsonText;

    file.close();
    qDebug() << "bookList 저장 완료";
}

// create book
QString BookManager::bookInsert(Book* book){
    for (auto it = bookList.begin(); it != bookList.end(); it++) {
        if (!it.value()) continue;
        if(book->getName().compare(it.value()->getName()) == 0
            && book->getWriter().compare(it.value()->getWriter()) == 0){
            // 중복 book 처리
            return "Duplicate_MUSIC";
        }
    }

    // book 중복되지 않음
    bookList.insert(book->getName(), book);

    return "OK";
}

// delete book
QString BookManager::bookErase(const QString& name, const QString& writer){
    for (auto it = bookList.begin(); it != bookList.end();) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && writer.compare(it.value()->getWriter()) == 0){
            bookList.erase(it);
            return "OK";
        } else {
            it++;
        }
    }

    return "NotFound";
}

// read one book
Book* BookManager::bookSearchByNameAndWriter(const QString& name, const QString& writer){
    Book* returnBook;

    for (auto it = bookList.begin(); it != bookList.end(); ++it) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && writer.compare(it.value()->getWriter()) == 0){
            returnBook = it.value();
            return returnBook;
        }
    }

    return nullptr;
}

// read some books search by name
QMap<QString, Book*> BookManager::bookSearchAllByName(const QString& name){
    QMap<QString, Book*> returnBooks;

    for(auto it = bookList.begin(); it != bookList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getName().contains(name) == true){
            returnBooks.insert(it.value()->getName(), it.value());
        }
    }

    return returnBooks;
}

// read some books search by writer
QMap<QString, Book*> BookManager::bookSearchAllByWriter(const QString& writer){
    QMap<QString, Book*> returnBooks;

    for(auto it = bookList.begin(); it != bookList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getWriter().contains(writer) == true){
            returnBooks.insert(it.value()->getName(), it.value());
        }
    }

    return returnBooks;
}

// read bookList
QMap<QString, Book*> BookManager::bookListRead(){
    return this->bookList;
}

BookManager::BookManager()
{
    // test start
    QImage img("./k102039149_1.PNG");

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    img.save(&buffer, "PNG");

    // 압축된 PNG 데이터를 qCompress로 추가 압축
    QByteArray compressed = qCompress(byteArray);
    QString img_string = QString::fromLatin1(compressed.toBase64());

    qDebug() << img_string;

    // test
    Book* book = new Book("잔소리탈출연구소1", "윤선아", "어크로스주니어", 13000, "아이와 부모 모두에게 필요한 책", 100, img_string);
    this->bookInsert(book);
    // test end

    bookListJsonLoad();
}

BookManager::~BookManager()
{
    qDebug() << "프로그램 종료 발생";
    bookListJsonSave();
}
