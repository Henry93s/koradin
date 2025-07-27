# Koradin - C++ & Qt Book Sales Management System

![C++](https://img.shields.io/badge/C++-17-00599C.svg?style=for-the-badge&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6-41CD52.svg?style=for-the-badge&logo=qt)
![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)

**Koradin**은 C++와 Qt 프레임워크를 기반으로 개발된 클라이언트-서버 구조의 데스크톱 도서 판매 및 관리 시스템입니다. 사용자는 도서, 음반, 블루레이 등 다양한 상품을 검색하고 주문할 수 있으며, 관리자는 서버를 통해 상품, 사용자, 주문 내역을 통합적으로 관리할 수 있습니다.

본 프로젝트는 TCP/IP 소켓 프로그래밍, 멀티스레딩, 데이터 직렬화, 데이터베이스 암호화 등 핵심적인 백엔드 기술과 Qt를 활용한 GUI 프로그래밍 역량을 종합적으로 학습하고 적용하는 것을 목표로 합니다.

## 🖼️ 스크린샷 (Screenshots)

| 서버 (Server) |
| :---: |
| <img width="1000" height="700" alt="2차미니_서버홈" src="https://github.com/user-attachments/assets/dab85cf6-3f68-4731-b10a-aab929d4dce0" /> |
| **클라이언트 - 홈 (Client - Home)** |
| <img width="1000" height="700" alt="2차미니_클라이언트홈" src="https://github.com/user-attachments/assets/b616f6e8-df0d-467d-b031-e98118acba48" /> |
| **클라이언트 - 상품 검색 (Client - Search)** |
| <img width="1000" height="700" alt="2차미니_클라이언트_상품 검색 (Music)" src="https://github.com/user-attachments/assets/652b254c-6803-4a88-8800-db7ad542da07" /> |
| **클라이언트 - 채팅 (Client - Chat)** |
| <img width="1299" height="925" alt="2차미니_클라이언트_채팅" src="https://github.com/user-attachments/assets/da072fb9-9b24-42a5-91c0-2048163b8231" /> |

## ✨ 주요 기능 (Key Features)

### 👤 사용자 기능 (Client-Side)
- **상품 검색**: 도서, 음반, 블루레이 등 상품을 이름, 저자/아티스트, 가격대별로 검색
- **상품 주문**: 원하는 상품을 선택하여 주문 목록에 추가
- **주문 관리**: 자신의 주문 내역을 조회하고 삭제
- **실시간 채팅**: 서버에 접속한 다른 사용자들과 실시간으로 메시지 및 파일 교환 (귓속말 기능 포함)
- **다국어 지원**: 한국어/영어를 선택하여 UI 언어 변경

### 🛠️ 관리자 기능 (Server-Side)
- **실시간 모니터링**: 접속 중인 클라이언트 목록 및 서버 로그 실시간 확인
- **상품 관리 (CRUD)**: GUI를 통해 손쉽게 상품 정보(이미지 포함)를 추가, 수정, 삭제
- **사용자 관리**: 전체 회원 목록 조회
- **주문 내역 관리**: 모든 사용자의 주문 현황 확인
- **데일리 로그**: 서버의 모든 주요 활동을 날짜별 로그 파일(`KORADIN_YYYYMMDD.LOG`)로 자동 기록

## 🛠️ 기술 스택 및 아키텍처 (Tech Stack & Architecture)

- **언어**: C++
- **UI 프레임워크**: Qt 6
- **핵심 기술**:
  - **아키텍처**: 클라이언트-서버 모델, 멀티스레드 서버 (`QThread`, `moveToThread`)
  - **네트워크**: TCP/IP 소켓 프로그래밍 (`QTcpServer`, `QTcpSocket`)
  - **데이터 관리**: JSON 파일 기반 데이터 영속성 (`nlohmann/json`)
  - **디자인 패턴**: 싱글턴 (Singleton) 패턴을 적용한 데이터 매니저
  - **보안**: 비밀번호 단방향 해싱 다이제스트 적용 (SHA-512 & Salt) (`OpenSSL`)
  - **통신 프로토콜**: JSON 기반 데이터 직렬화/역직렬화 및 길이 프리픽스(Length-Prefix) 통신

### 시스템 아키텍처
Koradin은 크게 3개의 계층으로 구성됩니다.
1.  **통신 계층 (Communication Layer)**: `Server`와 `Client`가 `CommuInfo` 프로토콜을 통해 TCP/IP 통신을 수행합니다. 서버는 `ClientHandler`를 별도 스레드에 할당하여 다중 접속을 처리합니다.
2.  **데이터 관리 계층 (Controller Layer)**: `UserManager`, `BookManager` 등 싱글턴 패턴으로 구현된 매니저 클래스들이 데이터의 CRUD를 담당하며 일관성을 유지합니다.
3.  **데이터 모델링 계층 (Data Modeling Layer)**: `UserInfo`, `ProductInfo` 등 실제 데이터를 표현하는 클래스들로 구성됩니다.

## 🚀 시작하기 (Getting Started)

### 사전 요구사항 (Prerequisites)
- Qt 6.x 이상
- C++17을 지원하는 컴파일러 (MSVC, MinGW, GCC 등)
- OpenSSL 라이브러리

### 빌드 및 실행 (Build & Run)

1.  **저장소 복제**
    ```sh
    git clone https://github.com/kimsungwon1/koradin.git
    cd Koradin
    ```

2.  **OpenSSL 경로 설정**
    프로젝트 루트의 `.pro` 파일을 열어 자신의 환경에 맞게 OpenSSL 라이브러리 경로를 수정해야 합니다.

    ```pro
    # Koradin.pro
    
    win32 {
        # MSVC (Microsoft Visual C++) 환경일 경우
        contains(QMAKE_CXX, "cl") {
            # 아래 경로를 자신의 OpenSSL 설치 경로로 수정하세요.
            INCLUDEPATH += "D:/programs/OpenSSL-Win64/include"
            LIBS += -LC:/OpenSSL-Win64/lib -lcrypto -lssl
        }
        # MinGW 환경일 경우
        contains(QMAKE_CXX, "g++") {
            # 아래 경로를 자신의 OpenSSL 설치 경로로 수정하세요.
            INCLUDEPATH += "C:/msys64/mingw64/include"
            LIBS += -LC:/msys64/mingw64/lib -lcrypto
        }
    }
    ```

3.  **프로젝트 빌드 (Qt Creator 또는 터미널)**
    ```sh
    qmake
    make # Windows(MSVC)의 경우 nmake 또는 jom 사용
    ```

4.  **프로그램 실행**
    - **서버 먼저 실행**: 빌드된 실행 파일에 관리자 코드를 입력하여 서버를 실행합니다.
    - **클라이언트 실행**: 서버 실행 후, 빌드된 실행 파일을 다시 열어 ID/PW로 로그인하여 클라이언트를 실행합니다.

## ⚙️ 핵심 구현 상세 (Core Implementation Details)

### 1. 길이 프리픽스 통신 (Length-Prefix Communication)
상품 이미지와 같은 대용량 데이터를 안정적으로 전송하기 위해, 실제 데이터를 보내기 전에 4바이트 길이 정보를 먼저 보내는 방식을 구현했습니다. 수신 측은 이 길이 정보를 바탕으로 전체 데이터가 도착할 때까지 대기하여 메시지 파편화 문제를 해결합니다.

```cpp
...
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out << (quint32)byteArray.size(); // 4바이트 길이 프리픽스 추가
    packet.append(byteArray);         // 실제 데이터 추가
    byteArray = packet;
...
```

### 2. SHA-512 비밀번호 암호화
사용자의 비밀번호는 Rainbow Table 공격에 대비하기 위해 Salt와 결합된 후 SHA-512 알고리즘으로 해싱되어 저장됩니다. 이를 통해 데이터베이스가 유출되더라도 원본 비밀번호를 유추하기 어렵게 만듭니다.

### 3. 싱글턴 패턴 데이터 관리
`UserManager`, `BookManager` 등 데이터 관리 클래스들은 싱글턴으로 설계되었습니다. 이를 통해 프로그램 전체에서 데이터에 대한 단일 접근점을 제공하여 데이터 일관성을 보장하고, 메모리에 중복된 데이터가 로드되는 것을 방지합니다.

## 👨‍💻 팀원 (Contributors)

| 이름 (Name) | 역할 (Role) | GitHub |
| :--- | :--- | :---: |
| **김선권** | 서버 & 통신 리드 (Server & Communication Lead) | `[github](https://github.com/kimsungwon1)` |
| **형건우** | 데이터 & 클라이언트 리드 (Data & Client Lead) | `[github](https://github.com/Henry93s)` |

## 📜 라이선스 (License)

이 프로젝트는 [MIT 라이선스](./LICENSE)를 따릅니다.
