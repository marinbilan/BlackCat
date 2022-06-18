#pragma once
#include "NetworkingIf.h"


namespace Networking
{

class HTTPSClient : public ClientIf
{
public:
	// Figure out how to init instance using this constructor 
	// HTTPSClient(const std::string& dbPath, const std::string& name);

	HTTPSClient(boost::asio::io_service& io_service,
                boost::asio::ssl::context& context,
                const std::string& server, 
                const std::string& path);

	~HTTPSClient();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Dummy test method
	void doSomething();


private:
	void handle_resolve(const boost::system::error_code& err,
                        tcp::resolver::iterator endpoint_iterator);

	bool verify_certificate(bool preverified,
                            boost::asio::ssl::verify_context& ctx);

	void handle_connect(const boost::system::error_code& err);

	void handle_handshake(const boost::system::error_code& error);

	void handle_write_request(const boost::system::error_code& err);

	void handle_read_status_line(const boost::system::error_code& err);

	void handle_read_headers(const boost::system::error_code& err);

	void handle_read_content(const boost::system::error_code& err);

std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

// Create file to write stuff
std::ofstream _HTTPSContent;

// //// HTTPS ////
tcp::resolver resolver_;
boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

boost::asio::streambuf request_;
boost::asio::streambuf response_;
};

}