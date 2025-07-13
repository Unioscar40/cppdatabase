#include <iostream>
#include <optional>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


std::string
listen(const sockaddr_in& hint, const SOCKET& listener)
{
    std::string message;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    char buffer[4096];
    ZeroMemory(buffer, 4096);

    int bytesReceived = recv(listener, buffer, 4096, 0);
    if (bytesReceived > 0)
        message = std::string(buffer, bytesReceived);

    return message;
}

std::optional<int>
send(const sockaddr_in& hint, const SOCKET& sender, const std::string& message)
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
    if (res != 0) {
        std::cerr << "Can't start wsa: " << res << std::endl;
        return 1;
    }

    SOCKET sender = socket(AF_INET, SOCK_STREAM, 0);
    if (sender == INVALID_SOCKET) {
        std::cerr << "Fail to created socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    if (connect(sender, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Fail to connect: " << WSAGetLastError() << std::endl;
        closesocket(sender);
        WSACleanup();
        return 1;
    }

    std::string message = "";

    while (message.compare("close")) {
        std::cout << "Send a message: ";
        std::getline(std::cin, message);

        auto optSend = send(hint, sender, message);
        if (!optSend)
            break;

        message = listen(hint, sender);
        std::cout << "Server received: " << message << std::endl;
    }

    closesocket(sender);
    WSACleanup();

    return 0;

}