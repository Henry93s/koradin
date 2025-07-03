#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include "mainController.h"
#include "book.h"
#include <QVector>
#include <QMap>

class BookManager : public MainController {

public:
    BookManager();
    ~BookManager();

    // STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
    BookManager* getInstance();

    // create book
    QString bookInsert(Book* book);
    // delete book
    QString bookErase(const QString& name, const QString& writer);
    // read one book
    Book* bookSearchByNameAndWriter(const QString& name, const QString& writer);
    // read bookList
    QMap<QString, Book*> bookListRead();

    // bookList.json 파일 -> bookList 로 load
    void bookListJsonLoad();
    // QMap bookList -> bookList.json 파일로 save
    void bookListJsonSave();

private:
    static BookManager* instance;
    QMap<QString, Book*> bookList;
};


#endif // BOOKMANAGER_H
