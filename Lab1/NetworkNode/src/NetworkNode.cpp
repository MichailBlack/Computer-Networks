#include "NetworkNode.h"

#include <array>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace Utils
{
    int GetWSAShutdownMode(ShutdownMode mode)
    {
        switch (mode)
        {
        case ShutdownMode::Receive:
            return SD_RECEIVE;
        case ShutdownMode::Send:
            return SD_SEND;
        case ShutdownMode::Both:
            return SD_BOTH;
        }

        return SD_BOTH;
    }
}

NetworkNode::NetworkNode()
{
    InitWinsock();
}

NetworkNode::~NetworkNode()
{
    Shutdown(ShutdownMode::Both);

    closesocket(m_Socket);
    WSACleanup();
}

void NetworkNode::Shutdown(ShutdownMode mode) const
{
    const int res = shutdown(m_Socket, Utils::GetWSAShutdownMode(mode));

    if (res == SOCKET_ERROR)
    {
        std::cerr << "shutdown failed with error: " << WSAGetLastError() << '\n';
        closesocket(m_Socket);
        WSACleanup();
    }
}

void NetworkNode::InitWinsock()
{
    const int res = WSAStartup(MAKEWORD(2, 2), &m_WSAData);
    if (res != 0)
        std::cerr << "WSAStartup failed with error: " << res << '\n';
}

void TCPNetworkNode::Send(const std::vector<char>& data) const
{
    const int res = send(m_Socket, data.data(), static_cast<int>(data.size()), 0);
    if (res == SOCKET_ERROR)
    {
        std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
        closesocket(m_Socket);
        WSACleanup();
    }

    std::cout << "Bytes Sent: " << res << '\n';
}

std::vector<char> TCPNetworkNode::Receive() const
{
    std::vector<char> buffer;

    int bytesReceived;
    do
    {
        std::array<char, 512> recvbuff {};
        bytesReceived = recv(m_Socket, recvbuff.data(), static_cast<int>(recvbuff.size()), 0);
        if (bytesReceived > 0)
        {
            std::cout << "Bytes received: " << bytesReceived << '\n';

            buffer.reserve(buffer.size() + bytesReceived);
            buffer.insert(buffer.end(), recvbuff.begin(), recvbuff.begin() + bytesReceived);
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed.\n";
        }
        else
        {
            std::cerr << "recv failed with error: " << WSAGetLastError() << '\n';
            closesocket(m_Socket);
            WSACleanup();
        }

    } while (bytesReceived > 0);

    return buffer;
}

void UDPNetworkNode::Send(const std::vector<char>& data) const
{
    const int bytesSent = sendto(m_Socket, data.data(), static_cast<int>(data.size()), 0, reinterpret_cast<const sockaddr*>(&m_Addr), sizeof(m_Addr));

    std::array<char, INET_ADDRSTRLEN> clientIP;
    inet_ntop(AF_INET, &m_Addr.sin_addr, clientIP.data(), INET_ADDRSTRLEN);

    std::cout << "Bytes Sent: " << bytesSent << '\n';
    std::cout << "To: " << clientIP.data() << ":" << ntohs(m_Addr.sin_port) << "\n";
}

std::vector<char> UDPNetworkNode::Receive() const
{
    std::array<char, 512> recvbuff {};
    int clientAddrLength = sizeof(m_Addr);

    const int bytesReceived = recvfrom(m_Socket, recvbuff.data(), static_cast<int>(recvbuff.size()), 0, const_cast<sockaddr*>(reinterpret_cast<const sockaddr*>(&m_Addr)), &clientAddrLength);

    std::array<char, INET_ADDRSTRLEN> clientIP;
    inet_ntop(AF_INET, &m_Addr.sin_addr, clientIP.data(), INET_ADDRSTRLEN);

    std::cout << "Bytes received: " << bytesReceived << '\n';
    std::cout << "From: " << clientIP.data() << ":" << ntohs(m_Addr.sin_port) << '\n';

    return { recvbuff.begin(), recvbuff.begin() + std::abs(bytesReceived) };
}
