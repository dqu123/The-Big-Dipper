#include <string>
#include <boost/asio.hpp>
#include "webserver.h"

using boost::asio::ip::tcp;

WebServer::WebServer(int port) {
    this->port = port;
}

int WebServer::getPort() {
    return port;
}

// GMT Timestamp
std::string WebServer::makeDaytimeString() {
    using namespace std;
    time_t  now = time(0); 
    char *c_str = asctime(gmtime(&now));
    std::string time_str(c_str);
    time_str.pop_back(); // Erase newline
    return "Date: " + time_str + " GMT\n";
}

// Basic functionality
void WebServer::helloWorld() {
    // Access port.
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        // ASIO synchronous socket set up.
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        // Read request.
        boost::system::error_code read_error;
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n", read_error);
        std::istream str(&buffer);
        std::string request;
        std::getline(str, request);
        std::cout << request << '\n';

        // Serve GET request.
        if (request.find("GET") == 0) {
            boost::system::error_code write_error;
            std::string date = makeDaytimeString();
            std::string content = http200 + contentType + date + hello;
            boost::asio::write(socket, boost::asio::buffer(content), write_error);
        }
    }
}


// For CS 3 Class demo on 5/3/15. 
void WebServer::demo() {
    try { 
        // Obtain port.
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
        
        for (;;) {
            // ASIO syncrhonous socket set up.
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            // Read request.
            boost::system::error_code read_error;
            boost::asio::streambuf buffer;
            
            std::string request = "";

            for (int i = 0; i < 7; i++) {
                boost::asio::read_until(socket, buffer, "\r\n\r\n", read_error);
                std::istream str(&buffer);
                std::string line;
                std::getline(str, line);
                std::cout << line << '\n';
                request += line + "<br/>";
            }

            // Echo GET request.
            if (request.find("GET") == 0) {
                boost::system::error_code write_error;
                std::string date = makeDaytimeString();
                std::string content = http200 + contentType + date 
                    + "\n<html><body>" + request + "</body></html>\n";
                boost::asio::write(socket, boost::asio::buffer(content), write_error);
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

