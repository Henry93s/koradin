#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include "bookManager.h"
#include "bluerayManager.h"
#include "musicManager.h"
#include "clientBookService.h"
#include "clientMusicService.h"
#include "clientBluerayService.h"
#include "clientHomeService.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    // print Music, Blueray, Book list
    void printMusicList();
    void printBluerayList();
    void printBookList();
    // ClientService, clientBookService 등 각 탭의 Service 클래스에서 Ui 에 입력된 값을 요구할 수 있기 때문에 필요
    Ui::Client* getUi();
    void printSearchBookList(const QVector<Book*>& list);
    void printSearchMusicList(const QVector<Music*>& list);
    void printSearchBluerayList(const QVector<Blueray*>& list);

private slots:
    // book tab button event
    void on_book_search_pushButton_clicked();
    void on_book_order_pushButton_clicked();

    // music tab button event
    void on_music_search_pushButton_clicked();
    void on_music_order_pushButton_clicked();

    // 블루레이 tab button event
    void on_blueray_search_pushButton_clicked();
    void on_blueray_order_pushButton_clicked();

    // 홈 tab button event
    void on_home_search_pushButton_clicked();
    void on_home_orderSearch_pushButton_clicked();

private:
    Ui::Client *ui;
    BookManager* bookmanager;
    BluerayManager* blueraymanager;
    MusicManager* musicmanager;
    ClientBookService clientBookService;
    ClientBluerayService clientBluerayService;
    ClientMusicService clientMusicService;
    ClientHomeService clientHomeService;
};

#endif // CLIENT_H
