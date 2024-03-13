#pragma once
#include "NetworkNode.h"

class UDPServer
    : public UDPNetworkNode
{
public:
    UDPServer(uint16_t port);
    ~UDPServer() override = default;

private:
    void CreateSocketAndBind(uint16_t port);
};
