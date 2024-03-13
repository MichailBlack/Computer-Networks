#pragma once
#include <string_view>

#include "NetworkNode.h"

class TCPServer
    : public TCPNetworkNode
{
public:
    TCPServer(uint16_t port);
    ~TCPServer() override = default;

protected:
    void CreateSocketAndBind(std::string_view serviceName);
    void Listen();
    void Accept();
};
