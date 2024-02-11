#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <vector>

// HTTPS 
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ssl.hpp>
using boost::asio::ip::tcp;

// HTTP
/*
In case of HTTP ASIO is standalone, 
not part of boost lib

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace std::placeholders;
*/

namespace Networking
{

class ClientIf
{
public:

    virtual ~ClientIf()
    {
    }

    virtual void preInit() {};
    virtual void postInit() {};

private:
    std::string returnErrorStr = "Error: ClientIf::getName() called from ClientIf!";
};

} // End of namespace