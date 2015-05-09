#include "utils.h"

int getPort(const NginxConfig &config)
{
    for (const auto& statement : config.statements_) 
    {
        bool k1 = false;
        for (const std::string& token : statement->tokens_)
        {
            if (k1)
            {
                try
                {
                    return stoi(token);
                }
                catch (...)
                {
                }
            }
            k1 = (token == "port");
        }
    }
    return -1;
}

std::map<std::string, Handler*> *getMappings(const NginxConfig &config)
{
    std::map<std::string, Handler*> *handlerMap = new std::map<std::string, Handler*>;
    for(const auto& statement: config.statements_)
    {
        bool k1 = false;
        bool k2 = false;
        bool k3 = false;
        bool k4 = false;
        std::string name = "";
        Handler *h;
        for (const std::string& token : statement->tokens_)
        {
            if (k1)
            {
                try
                {
                    h = new HelloWorldHandler;
                    handlerMap->insert(std::pair<std::string, Handler*>(token, h));
                }
                catch (...)
                {
                }
            }
            else if (k2)
            {
                try
                {
                    h = new EchoHandler;
                    handlerMap->insert(std::pair<std::string,Handler*>(token, h));
                }
                catch (...)
                {
                }
            }
            else if (k3)
            {
                try
                {
                    name = token;
                }
                catch (...)
                {
                }
            }
            else if (k4)
            {
                try
                {
                    h = new StaticFileHandler(name, token);
                    handlerMap->insert(std::pair<std::string,Handler*>(name, h));
                }
                catch (...)
                {
                }
            }
            k1 = (token == "helloWorld");
            k2 = (token == "echo");
            k4 = k3;
            k3 = (token == "static");
        }
    }
    return handlerMap;
}
