#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <thread>

#include "TCPServer.h"
#include "UDPServer.h"

int main()
{
    constexpr std::array arr { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    const std::vector<char> arrInBytes {reinterpret_cast<const char*>(arr.data()), reinterpret_cast<const char*>(arr.data()) + arr.size() * sizeof(arr[0])};

    auto start { std::chrono::steady_clock::now() };
    {
        const TCPServer server { 27015 };

        std::cout << "TCP server initialized.\n\n";

        const auto data { server.Receive() };

        std::cout << "\nClient sent: ";
        for (const char byte : data)
            std::cout << byte;

        std::cout << "\n\n";

        std::cout << "Sending array of doubles to the client using TCP...\n";
        std::cout << "Array: ";
        for (const auto& elem : arr)
            std::cout << elem << ' ';
        std::cout << "\n\n";

        server.Send(arrInBytes);
        server.Shutdown(ShutdownMode::Send);
    }
    auto end { std::chrono::steady_clock::now() };
    std::chrono::duration elapsedSeconds { end - start };

    std::cout << "\nTCP server work took " << static_cast<double>(elapsedSeconds.count()) * 1000.0 << "ms\n";

    std::cout << '\n' << std::string(80, '-') << '\n';

    start = std::chrono::steady_clock::now();
    {
        const UDPServer server { 27015 };

        std::cout << "UDP server initialized.\n\n";

        const auto data { server.Receive() };

        std::cout << "\nClient sent: ";
        for (const char byte : data)
            std::cout << byte;

        std::cout << "\n\n";

        std::cout << "Sending array of doubles to the client using UDP...\n";
        std::cout << "Array: ";
        for (const auto& elem : arr)
            std::cout << elem << ' ';
        std::cout << '\n';
        server.Send(arrInBytes);
        server.Shutdown(ShutdownMode::Send);
    }
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;

    std::cout << "\nUDP server work took " << static_cast<double>(elapsedSeconds.count()) * 1000.0 << "ms\n";

    getchar();
}