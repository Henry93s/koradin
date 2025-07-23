#include "orderManager.h"
#include <QApplication>
#include <QDir>
#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include "uuidCompare.h"

// singleton 인스턴스 초기화
OrderManager* OrderManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
OrderManager* OrderManager::getInstance(){
    if(!instance){
        instance = new OrderManager();
    }
    return instance;
}

void OrderManager::orderListJsonLoad() {
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../orderList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../orderList.json");
#endif

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "orderList 파일 오픈 실패";
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);  // UTF-8 인코딩 설정
    QString jsonText = in.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        qDebug() << "JSON 파싱 오류:" << parseError.errorString();
        return;
    }

    QJsonArray rootArray = doc.array();

    for (const QJsonValue& orderVal : rootArray) {
        QJsonObject orderObj = orderVal.toObject();

        QString userID = orderObj["userID"].toString();
        UserInfo* user = usermanager->getInstance()->userSearchById(userID);
        if (!user) {
            continue;
        }

        QJsonArray orderItems = orderObj["orderItems"].toArray();
        QVector<ProductInfo*> products;

        for (const QJsonValue& itemVal : orderItems) {
            QString uuid = itemVal.toObject()["uuid"].toString();
            ProductInfo* product = nullptr;

            if (!product) product = bookmanager->getInstance()->bookSearchByUuid(uuid);
            if (!product) product = blueraymanager->getInstance()->blueraySearchByUuid(uuid);
            if (!product) product = musicmanager->getInstance()->musicSearchByUuid(uuid);

            if (product) {
                products.append(product);
                qDebug() << user->getID() << " Loaded order prod uuid, name : " << uuid << " " << product->getName();
            } else {
                qDebug() << "not found user order prod item uuid : " << uuid;
            }
        }

        // orderList에 추가
        if (orderList.contains(user)) {
            for (ProductInfo* p : products) {
                if (!orderList[user].contains(p)) {
                    orderList[user].append(p);
                }
            }
        } else {
            orderList.insert(user, products);
        }
    }
}

void OrderManager::orderListJsonSave() {
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../orderList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../orderList.json");
#endif

    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "orderList 파일 저장 실패";
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // UTF-8 인코딩

    QJsonArray rootArray;

    for (auto it = orderList.begin(); it != orderList.end(); ++it) {
        UserInfo* user = it.key();
        QVector<ProductInfo*> products = it.value();

        QJsonObject orderObject;
        orderObject["userID"] = user->getID();

        QJsonArray orderItemsArray;
        int priceSum = 0;
        for (ProductInfo* product : products) {
            QJsonObject itemObject;
            itemObject["uuid"] = product->getUuid();
            priceSum += product->getPrice();
            orderItemsArray.append(itemObject);
        }
        orderObject["orderItems"] = orderItemsArray;
        orderObject["priceSum"] = priceSum;

        rootArray.append(orderObject);
    }

    QJsonDocument doc(rootArray);
    out << doc.toJson(QJsonDocument::Indented);

    file.close();
    qDebug() << "orderList 저장 완료";
}


OrderManager::OrderManager() {
    orderListJsonLoad();
}

OrderManager::~OrderManager() {
    qDebug() << "프로그램 종료 발생";
    orderListJsonSave();
}

QMap<UserInfo*, QVector<ProductInfo*>> OrderManager::getOrderList(){
    return this->orderList;
}

int OrderManager::addOrderList(UserInfo* user, ProductInfo* product){
    if(!user || !product){
        qDebug() << "user 또는 product 없음";
        return -1; // err
    }

    QString newUuid = product->getUuid();

    if (orderList.contains(user)) {
        QVector<ProductInfo*>& productList = orderList[user];

        // UUID가 같은 상품이 이미 있는지 검사
        bool alreadyExists = false;
        for (ProductInfo* existingProduct : productList) {
            if (existingProduct && existingProduct->getUuid() == newUuid) {
                alreadyExists = true;
                qDebug() << "product uuid 중복";
                return 1; // 중복
            }
        }

        // UUID가 중복되지 않으면 추가
        if (!alreadyExists) {
            productList.append(product);
            qDebug() << "product 추가";
            return 0; // ok
        }
    } else {
        // 유저가 없으면 새로운 벡터 생성 후 상품 추가
        QVector<ProductInfo*> productList;
        productList.append(product);
        orderList.insert(user, productList);
        qDebug() << "user & product 추가";
        return 0; // ok
    }
    return 0;
}
int OrderManager::delOrderList(UserInfo* user, ProductInfo* product){
    if (!user || !product) {
        qDebug() << "user 또는 product 없음";
        return -1; // err
    }

    if (orderList.contains(user)) {
        QVector<ProductInfo*>& productList = orderList[user];

        // product가 있다면 제거
        int index = productList.indexOf(product);
        if (index != -1) {
            productList.remove(index);

        }

        // 상품이 모두 삭제되면 사용자도 제거
        if (productList.isEmpty()) {
            orderList.remove(user);
        }
    }
    return 0; // ok
}

int OrderManager::delOrderListUuid(const QString& product_uuid)
{
    bool deleted = false;

    // map 순회하면서 조건에 맞는 항목 삭제
    QList<UserInfo*> keysToRemove;

    for (auto it = orderList.begin(); it != orderList.end(); ++it) {
        UserInfo* user = it.key();
        QVector<ProductInfo*>& productList = it.value();

        int beforeSize = productList.size();

        // uuid가 일치하는 상품 제거
        productList.erase(
            std::remove_if(productList.begin(), productList.end(),
                           [&product_uuid](ProductInfo* p) {
                               return p && p->getUuid() == product_uuid;
                           }),
            productList.end());

        if (productList.size() != beforeSize) {
            deleted = true;
        }

        // 비었으면 삭제 예정 목록에 추가
        if (productList.isEmpty()) {
            keysToRemove.append(user);
        }
    }

    // 벡터가 비어 삭제 대상인 유저 제거
    for (UserInfo* user : keysToRemove) {
        orderList.remove(user);
    }

    return deleted ? 1 : 0;
}
