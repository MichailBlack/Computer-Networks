#pragma once
#include <string_view>

#include "NetworkNode.h"

class TCPClient
    : public TCPNetworkNode
{
public:
    TCPClient(std::string_view host, uint32_t port);
    ~TCPClient() override = default;

private:
    void Connect(std::string_view nodeName, std::string_view serviceName);
};
