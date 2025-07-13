#include <iostream>
#include <optional>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


std::string
listen(const sockaddr_in &hint, const SOCKET &listener, const SOCKET &clientSocket)
{
    std::string message;

    char host[NI_MAXHOST]; 
    char service[NI_MAXSERV]; 

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    char buffer[4096];
    ZeroMemory(buffer, 4096);

    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0)
        message = std::string(buffer, bytesReceived);

    return message;
}

std::optional<int>
send(const sockaddr_in &hint, const SOCKET &sender, const std::string &message)
{
    const char* msg = message.c_str();
    int sendResult = send(sender, msg, strlen(msg), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Error sending a message: " << WSAGetLastError() << std::endl;
        return std::nullopt;
    }
    return 0;
}

int main()
{
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(res != 0) {
        std::cerr << "Can't start wsa: " << res << std::endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        std::cerr << "Fail to created socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in hint; 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    if (bind(server, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Fail to bind: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(server);
        return 1;
    }

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(server);
        return 1;
    }

    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(server, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Acceptance error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(server);
        return 1;
    }

    std::string message = "";

    while (message.compare("close")) {
        auto optListen = listen(hint, server, clientSocket);

        std::cout << "Client says: " << optListen << std::endl;
        message = optListen;
        std::getline(std::cin, message);
        auto optSend = send(hint, clientSocket, message);
        if (!optSend)
            break;
    }

    closesocket(server);
    closesocket(clientSocket);
    WSACleanup();

    return 0;

}
