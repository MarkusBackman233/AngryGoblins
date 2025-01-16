#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "ReceiveClientMessage.h"
#include "User.h"
#include <memory>
#include "GameQueue.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



void do_session(tcp::socket socket, ReceiveClientMessage* receiveClientMessage)
{

    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{ std::move(socket) };

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
        {
            res.set(http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) +" websocket-server-sync");
        }));

        // Accept the websocket handshake
        ws.accept();
        auto user = std::make_unique<User>(&ws); // do this before try and delete him in the catch

        for (;;)
        {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string message = beast::buffers_to_string(buffer.data());
            receiveClientMessage->Handle(message, user.get());
        }
    }
    catch (beast::system_error const& se)
    {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr <<
                "Usage: websocket-server-sync <address> <port>\n" <<
                "Example:\n" <<
                "    websocket-server-sync 0.0.0.0 8080\n";
            return EXIT_FAILURE;
        }

        GameQueue gameQueue;
        ReceiveClientMessage receiveClientMessage(&gameQueue);

        auto const address = net::ip::make_address(argv[1]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[2]));

        // The io_context is required for all I/O
        net::io_context ioc{ 1 };

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ ioc, {address, port} };
        for (;;)
        {
            // This will receive the new connection
            tcp::socket socket{ ioc };

            // Block until we get a connection
            acceptor.accept(socket);
            std::cout << "Started Connection" << std::endl;
            // Launch the session, transferring ownership of the socket
            std::thread(&do_session,std::move(socket),&receiveClientMessage).detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
