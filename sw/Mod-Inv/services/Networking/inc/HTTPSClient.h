#pragma once
#include "NetworkingIf.h"


namespace Networking
{

class HTTPSClient : public ClientIf
{
public:

	HTTPSClient(boost::asio::io_service& io_service,
                boost::asio::ssl::context& context,
                const std::string& server, 
                const std::string& path,
				std::string& content);

	~HTTPSClient();

private:
	void handle_resolve(const boost::system::error_code& err,
                        tcp::resolver::iterator endpoint_iterator);

	void handle_connect(const boost::system::error_code& err);

	void handle_handshake(const boost::system::error_code& error);

	void handle_write_request(const boost::system::error_code& err);

	void handle_read_status_line(const boost::system::error_code& err);

	void handle_read_headers(const boost::system::error_code& err);

	void handle_read_content(const boost::system::error_code& err);


// Create file to write HTML stuff
std::ofstream _HTTPSContent;
std::string& m_content;

// //// HTTPS ////
tcp::resolver resolver_;
boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

boost::asio::streambuf request_;
boost::asio::streambuf response_;
};

}