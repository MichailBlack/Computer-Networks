#include "TCPClient.h"

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

TCPClient::TCPClient(std::string_view host, uint32_t port)
{
    Connect(host, std::to_string(port));
}

void TCPClient::Connect(std::string_view nodeName, std::string_view serviceName)
{
    addrinfo* result {};
    addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    SOCKET connectSocket = INVALID_SOCKET;

    // Resolve the server address and port
    int res = getaddrinfo(nodeName.data(), serviceName.data(), &hints, &result);
    if (res != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << res << "\n";
        WSACleanup();
    }

    // Attempt to connect to an address until one succeeds
    for (const auto* node = result; node != nullptr; node = node->ai_next)
    {
        // Create a SOCKET for connecting to server
        connectSocket = socket(node->ai_family, node->ai_socktype, node->ai_protocol);
        if (connectSocket == INVALID_SOCKET)
        {
            std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
            WSACleanup();
        }

        // Connect to server.
        res = connect(connectSocket, node->ai_addr, static_cast<int>(node->ai_addrlen));
        if (res == SOCKET_ERROR)
        {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
    }

    m_Socket = connectSocket;
}
