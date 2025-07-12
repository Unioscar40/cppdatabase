## Client code 
```cpp
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    /*
    * Initialize winsock librarie 
    * MAKEWORD(2, 2) -> Api version 2.2
    */
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "Can't start winsock" << std::endl;;
        return 1;
    }

    /*
    * Create socket
    * AF_INET -> internetwork: UDP, TCP, etc
    * SOCK_STREAM -> stream socket
     */
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Invalid socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    /*
    * sockaddr_in -> Struct to store server IP and host
    */
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000); /* Make port to byte order red */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); /* Make Ip to binary format */

    /*
    * Connect to server 127.0.0.1:54000
    */
    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Fail to connect: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    const char* msg = "Hello from client!";
    send(sock, msg, strlen(msg), 0);

    closesocket(sock);
    WSACleanup();

    return 0;

}

```

## Server code 

```cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "Can't created wsa: " << res << std::endl;
        return 1;
    }

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Fail to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);       
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    /*
    * Listening socket will be "tied" to 127.0.0.1:54000
    */
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Fail to bind: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    /* 
    * listening mode 
    * SOMAXCONN -> Max connection allowed
    */
    if (listen(listening, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening error: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Acceptance error: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    /* Info from client */
    char host[NI_MAXHOST]; /* host */
    char service[NI_MAXSERV]; /* port */

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << "Client connected from " << host << " in port " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << "Cliente connected from " << host << ":" << ntohs(client.sin_port) << std::endl;
    }

    /* Data client */
    char buffer[4096];
    ZeroMemory(buffer, 4096);

    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0) 
        std::cout << "Received message: " << std::string(buffer, bytesReceived) << std::endl;

    closesocket(clientSocket);
    closesocket(listening);
    WSACleanup();

    return 0;

}
```
