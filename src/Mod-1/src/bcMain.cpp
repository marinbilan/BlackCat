#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "FactoryIf.h"
#include "Factory.h"
// #include "FactoryImpl.h"

//#include "ServiceIf.h"
//#include "Service0.h"

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace std::placeholders;

class HttpClient {
public:
    HttpClient(asio::io_service& io_service, asio::ip::tcp::resolver& resolver,
               const std::string& host, const std::string& path)
        : host_(host), path_(path), resolver_(resolver), sock_(io_service) { }


    void Start() {
        // The client must start by resolving the hostname into an IP endpoint.
        // This will give us a destination for the TCP connection. We can
        // safely hard code the "http" service name.
        resolver_.async_resolve(
            asio::ip::tcp::resolver::query(host_, "http"),
            [this](std::error_code ec,
                   asio::ip::tcp::resolver::iterator it) {
                if (ec) {
                    std::cout << "Error resolving " << host_ << ": "
                    	<< ec.message();
                    return;
                }

                // For simplicity, we'll assume the first endpoint will always
                // be available.
                std::cout << host_ << ": resolved to " << it->endpoint()
                          << std::endl;

                do_connect(it->endpoint());
            });
    }


private:
    void do_connect(const asio::ip::tcp::endpoint& dest) {
        // Remember that the Asio library will make copies of parameters passed
        // by const reference, so it's ok to let the endpoint go out of scope
        // when this method returns.
        sock_.async_connect(
            dest, [this](std::error_code ec) {
                if (ec) {
                    std::cout << "Error connecting to " << host_ << ": "
                               << ec.message();
                    return;
                }

                std::cout << host_ << ": connected to "
                          << sock_.remote_endpoint() << std::endl;
                do_send_http_get();
            });
    }


    void do_send_http_get() {
        // At minimum, the remote server needs to know the path being fetched
        // and the host serving that path. The latter is required because a
        // single server often hosts multiple domains.
        request_ = "GET " + path_ + " HTTP/1.1\r\nHost: " + host_ + "\r\n\r\n";
        asio::async_write(
            sock_, asio::buffer(request_),
            [this](std::error_code ec, std::size_t size) {
                if (ec) {
                    std::cout << "Error sending GET " << ec;
                    return;
                }

                std::cout << host_ << ": sent " << size << " bytes";
                do_recv_http_get_header();
            });
    }


    void do_recv_http_get_header() {
        // Since HTTP/1.1 is a text based protocol, most of it is human readable
        // by design. Notice how the "double end of line" character sequence
        // ("\r\n\r\n") is used to delimit message sections.
        asio::async_read_until(
            sock_, response_, "\r\n\r\n",
            [this](std::error_code ec, std::size_t size) 
            {
                if (ec) 
                {
                    std::cout << "Error receiving GET header " << ec;
                    return;
                }

                std::cout<< host_ << ": received " << size << ", streambuf "
                          << response_.size() << '\n';

                // The asio::streambuf class can use multiple buffers
                // internally, so we need to use a special iterator to copy out
                // the header.
                std::string header(
                    asio::buffers_begin(response_.data()),
                    asio::buffers_begin(response_.data()) + size);
                response_.consume(size);

                std::cout << "----------" << std::endl << host_
                          << ": header length " << header.size() << std::endl
                          << header;

                // First we'll check for the explicit "Content-Length" length
                // field. This provides the exact body length in bytes.
                size_t pos = header.find("Content-Length: ");
                if (pos != std::string::npos) {
                    size_t len = std::strtoul(
                        header.c_str() + pos + sizeof("Content-Length: ") - 1,
                        nullptr, 10);
                    do_receive_http_get_body(len - response_.size());
                    return;
                }

                // The other alternative is a chunked transfer. There is a quick
                // way to determine the remaining length in this case.
                pos = header.find("Transfer-Encoding: chunked");
                if (pos != std::string::npos) {
                    do_receive_http_get_chunked_body();
                    return;
                }

                std::cout << "Unknown body length" << '\n';
            });
    }



    void do_receive_http_get_body(size_t len) {
        // For "Content-Length" we know exactly how many bytes are left to
        // receive.
        asio::async_read(
            sock_, response_, asio::transfer_exactly(len),
            std::bind(&HttpClient::handle_http_get_body, this, _1, _2));
    }


    void do_receive_http_get_chunked_body() {
        // For chunked transfers the final body chunk will be terminated by
        // another "double end of line" delimiter.
        asio::async_read_until(
            sock_, response_, "\r\n\r\n",
            std::bind(&HttpClient::handle_http_get_body, this, _1, _2));
    }

    
    void handle_http_get_body(std::error_code ec,
                              std::size_t size) {
        if (ec) {
            std::cout << "Error receiving GET body " << ec;
            return;
        }

        std::cout << host_ << ": received " << size << ", streambuf "
                  << response_.size();

        // We can finally consume the body and print it out if desired.
        const auto& data = response_.data();
        std::string body(asio::buffers_begin(data), asio::buffers_end(data));
        response_.consume(size);

        std::cout << "----------" << std::endl << host_ << ": body length "
                  << body.size() << std::endl;
        
        // Just console flag in program running args
        //if (FLAGS_print_body) {
            std::cout << body;
        //}
    }

    const std::string host_;
    const std::string path_;

    asio::ip::tcp::resolver& resolver_;
    asio::ip::tcp::socket sock_;

    std::string request_;
    asio::streambuf response_;
};


void fHTTPClient() 
{
	// Two expected fields: "host" and "path"
	std::string host_0("www.google.com");
	std::string path_0("/");

	std::string host_1("jservice.io");
	std::string path_1("/api/random");
	// Prepare arguments for HTTP client
	asio::io_service io_service;
	asio::ip::tcp::resolver resolver(io_service);
	std::vector<std::unique_ptr<HttpClient>> clients;

	// We are creating a new HttpClient for entry
	std::unique_ptr<HttpClient> c(new HttpClient(io_service, resolver, host_0, path_0));
	c->Start();

	clients.push_back(std::move(c));

	io_service.run();
}



//
int main()
{
	// HTTP client
	// https://www.youtube.com/watch?v=lQx5UYQRZ34&ab_channel=BrianOrr
	// https://github.com/v1bri/codecast/blob/master/006/http_client_main.cc
	fHTTPClient();

	return 0;
}