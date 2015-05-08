#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H 
#include <string>
#include <sstream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Handler {
    public:
        virtual std::string handleRequests(std::string request) = 0;
        std::string makeDaytimeString();
    protected:
        const std::string http200 = "HTTP/1.0 200 OK\r\n";
        const std::string http404 = "HTTP/1.0 404 Not Found\r\n";
        const std::string contentTypeHTML = "Content-Type: text/html;\r\n";
        const std::string contentTypeJPEG = "Content-Type: image/jpeg;\r\n";
};

class HelloWorldHandler: public Handler {
    public:
        std::string handleRequests(std::string request) {
            std::string date = makeDaytimeString();
            return http200 + contentTypeHTML + date + hello;
        }
    private:
        const std::string hello = "\r\n<html><body>" 
                                  "Hello, world!</body></html>\r\n";
};

class EchoHandler: public Handler {
    public:
        std::string handleRequests(std::string request) {
            std::string date = makeDaytimeString();
            return http200 + contentTypeHTML + date + "\r\n" + request;
        }
};

class StaticFileHandler: public Handler {
    public:
        StaticFileHandler(std::string requestPath, std::string basePath) {
            this->requestPath = requestPath;
            this->basePath = basePath;
        }

        std::string handleRequests(std::string request) {
            // Strip GET to form the std::string path 
            std::istringstream iss(request);
            std::string path;
            iss >> path; // Remove GET
            iss >> path;
            // Remove request path.
            // E.g. remove "/static" in GET /static/foo.html
            std::cout << "path: "  << path << '\n';
            path.erase(0, requestPath.size()); 

            // Check for index request cases. 
            if (path.empty()) {
                path = "/index.html";
            }
            else if (path[path.size() - 1] == '/') {
                path += "index.html";
            }

            // Get requested file.
            std::string full_path = basePath + path;
            std::cout << "Searching for " + full_path + " ...\n";
            std::string content = "";
            std::ifstream file(full_path);
            if (file.is_open()) {
                std::string line = "";
                while (getline(file, line)) {
                    content += line + "\n";
                }
                file.close();
            }

            std::string date = makeDaytimeString();

            std::string contentType;
            if (!content.empty() && path.find(".jpg") == path.size() - 4) {
                contentType = contentTypeJPEG;
            }
            else {
                contentType = contentTypeHTML;
            }

            if (content.empty()) {
                return http404 + contentType + date + "\r\n"
                    "Error 404: Not Found.";
            }
            return http200 + contentType + date + "\r\n" + content;
        }

    private: 
        std::string requestPath;
        std::string basePath;
};
#endif
