#include "UDPServer.h"

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

UDPServer::UDPServer(uint16_t port)
{
    CreateSocketAndBind(port);
}

void UDPServer::CreateSocketAndBind(uint16_t port)
{
    m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_Socket == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
        WSACleanup();
    }

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_Socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(m_Socket);
        WSACleanup();
    }
}
