#include <iostream>
#include <thread>

#include "TCPClient.h"
#include "UDPClient.h"

int main(int argc, char **argv)
{
    // Validate the parameters
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [host]\n";
        return 1;
    }

    constexpr char ch = 'E';

    auto start { std::chrono::steady_clock::now() };
    {
        const TCPClient client(argv[1], 27015);

        std::cout << "TCP client initialized.\n\n";

        std::cout << "Sending char '" << ch << "' to the server using TCP...\n\n";

        client.Send({ ch });
        client.Shutdown(ShutdownMode::Send);

        std::cout << '\n';

        const auto serverResponse = client.Receive();
        std::cout << "\nServer response in double: ";
        for (size_t i = 0; i < serverResponse.size(); i += sizeof(double))
        {
            const double value = *reinterpret_cast<const double*>(&serverResponse[i]);
            std::cout << value << ' ';
        }

        std::cout << '\n';
    }
    auto end { std::chrono::steady_clock::now() };
    std::chrono::duration elapsedSeconds { end - start };

    std::cout << "\nTCP client work took " << static_cast<double>(elapsedSeconds.count()) * 1000.0 << "ms\n";

    std::cout << '\n' << std::string(80, '-') << '\n';

    start = std::chrono::steady_clock::now();
    {
        using namespace std::chrono_literals;
        const UDPClient client {argv[1], 27015 };

        std::cout << "UDP client initialized.\n\n";

        std::cout << "Sending char '" << ch << "' to the server using UDP...\n\n";

        std::this_thread::sleep_for(1s);

        client.Send({ ch });
        client.Shutdown(ShutdownMode::Send);

        std::cout << '\n';

        const auto serverResponse = client.Receive();
        std::cout << "\nServer response in double: ";
        for (size_t i = 0; i < serverResponse.size(); i += sizeof(double))
        {
            const double value = *reinterpret_cast<const double*>(&serverResponse[i]);
            std::cout << value << ' ';
        }

        std::cout << '\n';
    }
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;

    std::cout << "\nUDP client work took " << static_cast<double>(elapsedSeconds.count()) * 1000.0 << "ms\n";

    getchar();
}
