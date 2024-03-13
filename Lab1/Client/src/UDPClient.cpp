#include "UDPClient.h"

#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

UDPClient::UDPClient(const std::string& host, uint16_t port)
{
    CreateSocket();

    m_Addr.sin_family = AF_INET;
    m_Addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &m_Addr.sin_addr.s_addr);
}

void UDPClient::CreateSocket()
{
    m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_Socket == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
        WSACleanup();
    }
}
