#pragma once

#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <WinSock2.h>

enum class ShutdownMode
{
    Receive = 0,
    Send,
    Both
};

class NetworkNode
{
public:
    NetworkNode();
	virtual ~NetworkNode();

    void Shutdown(ShutdownMode mode) const;

private:
    void InitWinsock();

protected:
    WSADATA m_WSAData;
    SOCKET m_Socket;
};

class TCPNetworkNode
    : public NetworkNode
{
public:
    TCPNetworkNode() = default;
    ~TCPNetworkNode() override = default;

    void Send(const std::vector<char>& data) const;
    std::vector<char> Receive() const;
};

class UDPNetworkNode
    : public NetworkNode
{
public:
    UDPNetworkNode() = default;
    ~UDPNetworkNode() override = default;

    void Send(const std::vector<char>& data) const;
    std::vector<char> Receive() const;

protected:
    // In Server it's sender's address, in Client it's server's address
    sockaddr_in m_Addr {};
};
