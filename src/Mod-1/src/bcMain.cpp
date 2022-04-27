#include <iostream>

#include "FactoryIf.h"
#include "Factory.h"
// #include "FactoryImpl.h"

//#include "ServiceIf.h"
//#include "Service0.h"

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


/*
Because we do not know how many data to get in buffer, create
buffer with some size
*/
std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), 
		[&](std::error_code ec, std::size_t length)
		{
			if(!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for(int i = 0; i < length; i++)
					std::cout << vBuffer[i];

				GrabSomeData(socket); 
			}
		}
	);
}

//
int main()
{
	// Create Factory
	Common::Factory::getInstance();

	// Move to Unit Test
	Service::Service0 srv0_0("dbPath", "instName");
	
	// Test Factroy
	srv0_0.testFactory();

	// Set FactroyImpl
	srv0_0.setFactoryImplPtr(Common::FactoryIf::getInstance());

	// Test FactoryImpl
	srv0_0.testFactoryImpl();



	// ---- NETWORKING ----
	asio::error_code ec;

	// Create a "context" - The platform specific interface
	asio::io_context context;

	// Give some fake tasks to asio so the context doesnt finish
	asio::io_context::work idleWork(context);

	// Start the context
	std::thread thrContext = std::thread([&] () { context.run(); });

	// Get address of somewhere we wish to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

	// Create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	// Tell socket to try and connect
	socket.connect(endpoint, ec);

	if(!ec)
	{
		std::cout << "Connected!" << '\n';
	} 
	else 
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << '\n';
	}

	if(socket.is_open())
	{
		// We need to wait until data is read
		// CASE 1 : This is bad - just wait for response for example purpose
		/* 
		std::string sRequest = 
		"GET /index.html HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);


		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		// CASE 2 : We can block thread and wait until some data are read
		// If we want to get more data, we will get only part of that data

		/*
		socket.wait(socket.wait_read);

		size_t bytes = socket.available();
		std::cout << "Bytes Available: " << bytes << '\n';

		if(bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			// When we use sincronius reading - we use read_some
			// socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			// In asyncronius reading - use GramSomeData - async_read_some

			for(auto c : vBuffer)
			{
				std::cout << c;
			}
		}
		*/

		// CASE 3 - Grab some data async.
		// First start reading data before any request (data) is send to server
		GrabSomeData(socket);

		std::string sRequest = 
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// Program does something else, while asio handles data transfer in background
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);

		context.stop();
		if(thrContext.joinable()) thrContext.join();
	}



	return 0;
}