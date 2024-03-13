#pragma once
#include <string>

#include "NetworkNode.h"

class UDPClient
    : public UDPNetworkNode
{
public:
    UDPClient(const std::string& host, uint16_t port);
    ~UDPClient() override = default;

private:
    void CreateSocket();
};
