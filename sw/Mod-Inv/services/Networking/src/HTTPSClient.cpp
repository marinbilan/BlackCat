#include "HTTPSClient.h"



Networking::HTTPSClient::HTTPSClient(boost::asio::io_service& io_service,
                boost::asio::ssl::context& context,
                const std::string& server, 
                const std::string& path,
                std::string& content) : 
                resolver_(io_service),
                socket_(io_service, context),
                _HTTPSContent("_HTTPSContent.txt"),
                m_content(content)
                // Use app flag when want to add at the end of file
                // _HTTPSContent("_HTTPSContent.txt", std::ofstream::app)
{
	// Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // std::cout << "GET " << path << " HTTP/1.0\r\n";
    // std::cout << "Host: " << server << "\r\n";
    // std::cout << "Accept: */*\r\n";
    // std::cout << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, "https");

    resolver_.async_resolve(query,
                            boost::bind(&HTTPSClient::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));                                       
}


Networking::HTTPSClient::~HTTPSClient()
{
    // std::cout << "HTTPSClient destructor called!" << '\n';
    _HTTPSContent.close();
}


void Networking::HTTPSClient::handle_resolve(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // std::cout << "Resolve OK" << "\n";
        socket_.set_verify_mode(boost::asio::ssl::verify_peer);

        boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
                                    boost::bind(&HTTPSClient::handle_connect, this,
                                                boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error resolve: " << err.message() << "\n";
    }
}


void Networking::HTTPSClient::handle_connect(const boost::system::error_code& err)
{
    if (!err)
    {
        // std::cout << "Connect OK " << "\n";
        socket_.async_handshake(boost::asio::ssl::stream_base::client,
                                boost::bind(&HTTPSClient::handle_handshake, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Connect failed: " << err.message() << "\n";
    }
}


void Networking::HTTPSClient::handle_handshake(const boost::system::error_code& error)
{
    if (!error)
    {
        // std::cout << "Handshake OK " << "\n";
        // std::cout << "Request: " << "\n";
        const char* header=boost::asio::buffer_cast<const char*>(request_.data());
        std::cout << header << "\n";

        // The handshake was successful. Send the request.
        boost::asio::async_write(socket_, request_,
                                 boost::bind(&HTTPSClient::handle_write_request, this,
                                 boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Handshake failed: " << error.message() << "\n";
    }
}


void Networking::HTTPSClient::handle_write_request(const boost::system::error_code& err)
{
    if (!err)
    {
        // Read the response status line. The response_ streambuf will
        // automatically grow to accommodate the entire line. The growth may be
        // limited by passing a maximum size to the streambuf constructor.
        boost::asio::async_read_until(socket_, response_, "\r\n",
                                      boost::bind(&HTTPSClient::handle_read_status_line, this,
                                      boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error write req: " << err.message() << "\n";
    }
}


void Networking::HTTPSClient::handle_read_status_line(const boost::system::error_code& err)
{
    // std::cout << "Handle handle_read_status_line before" << '\n';
    if (!err)
    {
        // Check that response is OK.
        std::istream response_stream(&response_);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return;
        }
        if (status_code != 200)
        {
            std::cout << "Response returned with status code ";
            std::cout << status_code << "\n";
            return;
        }
        // std::cout << "Status code: " << status_code << "\n";

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                      boost::bind(&HTTPSClient::handle_read_headers, this,
                                                  boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}


void Networking::HTTPSClient::handle_read_headers(const boost::system::error_code& err)
{
    if (!err)
    {
        // Process the response headers.
        std::istream response_stream(&response_);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r");
               // std::cout << header << "\n";
               // std::cout << "\n";

        // Write whatever content we already have to output.
        if (response_.size() > 0) {
            std::ostringstream ss;
        
            ss << &response_;
            m_content += ss.str();  // Save first response
        }

        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&HTTPSClient::handle_read_content, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }

}


void Networking::HTTPSClient::handle_read_content(const boost::system::error_code& err)
{
    if (!err)
    {
        std::ostringstream ss;
        
        ss << &response_;
        m_content += ss.str();

        // Continue reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&HTTPSClient::handle_read_content, this,
                                            boost::asio::placeholders::error));     
    }
    else if (err != boost::asio::error::eof)
    {
        std::cout << "Error: " << err << "\n";
    }
}
