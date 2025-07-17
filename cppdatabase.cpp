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