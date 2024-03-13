#include "TCPServer.h"

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

TCPServer::TCPServer(uint16_t port)
{
    CreateSocketAndBind(std::to_string(port));
    Listen();
    Accept();
}

void TCPServer::CreateSocketAndBind(std::string_view serviceName)
{
    addrinfo* result {};
    addrinfo hints {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    const int res = getaddrinfo(nullptr, serviceName.data(), &hints, &result);
    if (res != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << res << "\n";
        WSACleanup();
    }

    // Create a SOCKET for the server to listen for client connections.
    const SOCKET listenSocket { socket(result->ai_family, result->ai_socktype, result->ai_protocol) };
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
    }

    // Setup the TCP listening socket
    if (bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR)
    {
        std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
    }

    m_Socket = listenSocket;

    freeaddrinfo(result);
}

void TCPServer::Listen()
{
    if (listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(m_Socket);
        WSACleanup();
    }
}

void TCPServer::Accept()
{
    // Accept a client socket
    const SOCKET clientSocket = accept(m_Socket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "accept failed with error: " << WSAGetLastError() << "\n";
        closesocket(m_Socket);
        WSACleanup();
    }

    // No longer need server socket
    closesocket(m_Socket);

    m_Socket = clientSocket;
}
