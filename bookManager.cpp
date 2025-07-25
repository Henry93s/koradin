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
#include <QApplication>
#include <QDir>
#include <QUuid>
#include "uuidCompare.h"

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
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../bookList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../bookList.json");
#endif
    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
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
        // book, blueray, music 클래스 객체에 대해서 유일 키인 quuid 를 부여
        QString uuid = QString::fromStdString(book["uuid"]);
        QString name = QString::fromStdString(book["name"]);
        QString writer = QString::fromStdString(book["writer"]);
        QString company = QString::fromStdString(book["company"]);
        int price = book["price"];
        QString context = QString::fromStdString(book["context"]);
        int amount = book["amount"];
        QString image = QString::fromStdString(book["image"]);

        Book* newBook = new Book(uuid, name, writer, company, price, context, amount, image);
        // bookList stl Map 컨테이너에 저장
        bookList.insert(uuid, newBook);

        qDebug() << "Loaded Book uuid, name : " << uuid << " (" << name << ")";
    }

    file.close();
}
void BookManager::bookListJsonSave(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../bookList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../bookList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
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
            { "uuid",         it.value()->getUuid().toStdString()},
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

    // uuid 중복 체크 -> 중복하지 않을 때까지 새로운 uuid set
    while(uuidIsduplicate(bookList, book->getUuid()) == true){
        book->setUuid(QUuid::createUuid().toString());
    }

    // book 중복되지 않음
    bookList.insert(book->getUuid(), book);

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

// delete book uuid
QString BookManager::bookEraseUuid(const QString& uuid){
    for (auto it = bookList.begin(); it != bookList.end();) {
        if (!it.value()) continue;
        if(uuid.compare(it.value()->getUuid()) == 0){
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
    for (auto it = bookList.begin(); it != bookList.end(); ++it) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && writer.compare(it.value()->getWriter()) == 0){
            return it.value();
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
            returnBooks.insert(it.value()->getUuid(), it.value());
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
            returnBooks.insert(it.value()->getUuid(), it.value());
        }
    }

    return returnBooks;
}

// err 수정 : uuid 로 조회 시 무조건 1개 return 처리
Book* BookManager::bookSearchByUuid(const QString& uuid){

    for(auto it = bookList.begin(); it != bookList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getUuid().compare(uuid) == 0){
            return it.value();
        }
    }

    return nullptr;
}

// read bookList
QMap<QString, Book*> BookManager::bookListRead(){
    return this->bookList;
}

BookManager::BookManager()
{
    // test data insert
/*
    QImage img("./../../book_images/여름이 긴 것은 수박을 많이 먹으라는 뜻이다.PNG");

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    img.save(&buffer, "PNG");

    QString img_string = QString::fromLatin1(byteArray.toBase64());

    // test
    Book* book = new Book("여름이 긴 것은 수박을 많이 먹으라는 뜻이다", "쩡찌", "세미콜론", 10000, "‘내가 좋아하는 것을 함께 좋아하고 싶은 마음’이라는 캐치프레이즈 아래 계속되어온 띵 시리즈의 스물여덟 번째 주제는 ‘과일’이다. 우리 마음의 결을 섬세하게 어루만지며 수많은 독자의 사랑을 받아온 만화 에세이 『땅콩일기』의 쩡찌 작가. 『여름이 긴 것은 수박을 많이 먹으라는 뜻이다』는 그가 펴내는 첫 산문집이라는 점에서도 의미가 크다. 현재 3권까지 출간된 『땅콩일기』는 분명 만화의 형태를 띠고 있지만, 한 편의 시 같기도 한 아름다운 문장과 깊고 넓은 사유는 이미 숱한 독자의 마음을 울리며 문학적 입지를 증명했다.", 100, img_string);
    this->bookInsert(book);
    // test end
*/
    bookListJsonLoad();
}

BookManager::~BookManager()
{
    qDebug() << "프로그램 종료 발생";
    bookListJsonSave();
    // 7/5 ~ : memory management -> STL list 들 사용 안하거나 종료할 때 모든 요소 delete 처리
}
