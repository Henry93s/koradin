#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include "mainController.h"
#include <QMap>
#include <QVector>
#include "userManager.h"
#include "bookManager.h"
#include "musicManager.h"
#include "bluerayManager.h"

class OrderManager : public MainController {
public:
    OrderManager();
    ~OrderManager();

    static OrderManager* getInstance();

    QMap<UserInfo*, QVector<ProductInfo*>> getOrderList();
    int addOrderList(UserInfo* user, ProductInfo* product);
    int delOrderList(UserInfo* user, ProductInfo* product);
    int delOrderListUuid(const QString& product_uuid);

    void orderListJsonLoad();
    void orderListJsonSave();

private:
    static OrderManager* instance;
    // userInfo, orderItems
    QMap<UserInfo*, QVector<ProductInfo*>> orderList;

    UserManager* usermanager;
    BookManager* bookmanager;
    BluerayManager* blueraymanager;
    MusicManager* musicmanager;
};

#endif // ORDERMANAGER_H
