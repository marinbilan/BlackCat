#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
#define BUFFER_SIZE 128
// 31 Clients (descriptors) and one Master descriptor
#define MAX_CLIENT_SUPPORTED 32


void server0()
{
	/*
	Standard C APIs structure
	UNIX Domain Sockets - uniquely indentified by a name
	To hold name - define structure - google this - 2 members:
	AF_UNIX and pathname (Name of socket)
	*/
	struct sockaddr_un name;

	int ret;
	int connection_socket;
	int data_socket;
	int result;
	int data;
	// Take local memory to receive msgs from client and send message back
	char buffer[BUFFER_SIZE];

	/*
	In case the program exited on the last run,
	remove the socket
	It can be case that we are running another process 
	with the same socket name this process will kill that socket
	and create new 
	*/
	unlink(SOCKET_NAME);


	// [2 STEP] CREATE MASTER SOCKET File Description - Connection Socket
	/*
	AF_UNIX - Type of Socket we are creating 
	SOCK_STREAM - Stream based communication (not Datagram based)
	*/
	connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	// Error handling
	if(connection_socket == -1)
	{
		std::cout << "[ERROR] Master Socket Creation Failed" << '\n';
	}

	std::cout << "Master Socket Creation Success" << '\n';


	// [3 STEP] Specify the socket name - Initialize with 0 all bytes
	// Fill the structure 
	memset(&name, 0, sizeof(struct sockaddr_un));
	// Specify the socket credentials
	name.sun_family = AF_UNIX;
	// Copy socket name to name.sun_path
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

	/*
	IMPORTANT so far:
	UNIX Domain Sockets are defined with two params:
	Socket Family and Socket Name
	*/


	// [4 STEP] bind()
	/*
	Purpose of bind() system call is that application() dictate the 
	underlying OS system the criteria of receiving the data. Here, bind()
	system call is telling the OS that if sender process sends the data destiantion
	to socket "/tmp/DemoSocket", then such data needs to be delivered to this process 
	(the server process)
	In 2 STEP we created Socket (Family and Name)
	1st arg - Created Socket Id
	2nd arg - pointer (casted) to already created socket info (Family and Name)
	3th arg - size od struct
	*/
	ret = bind(connection_socket, (const struct sockaddr*) &name, sizeof(struct sockaddr_un));

	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Bind Failed" << '\n';
	}

	std::cout << "Server Socket Bind Success" << '\n';


	// [5 STEP] listen()
	/*
	Prepare for accepting connection. The backlog size is set
	to 20. So while one request is being precessed other requests
	can be waiting.
	In this example we can listen up to 20 clients at the same time.
	OS will queue the request and deliver those requests one by one to
	our server process one by one for processing.
	*/
	ret = listen(connection_socket, 20);

	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Listen Failed" << '\n';
	}

	std::cout << "Server Listen Success" << '\n';


	// [6 STEP] accept()
	/*
	This is the main loop for handling connections.
	All server process usually runs 24 x 7.
	*/
	for(;;)
	{
		std::cout << "Waiting ... for accpet() system call ..." << '\n';

		/*
		data_socket - communicate file descriptor (clients handler)
		With this data_socket (int) server will communicate to client
		IMPORTANT - accpet() is blocking system call and it will stay 
		blocked until server receives CIR from client
		Wait for clients to connect ... 
		*/
		data_socket = accept(connection_socket, NULL, NULL);

		// Error handling
		if(data_socket == -1)
		{
			std::cout << "[ERROR] Accept Failed" << '\n';
		}

		std::cout << "Connection accepted from client Success" << '\n';

		// PROCESS CLIENT MESSAGE
		result = 0;
		for(;;)
		{
			memset(buffer, 0, BUFFER_SIZE);

			std::cout << "Waiting for data from the clients" << '\n';
			// BLOCK until client sends new data
			// ret is number of bytes sends from client
			ret = read(data_socket, buffer, BUFFER_SIZE);

			if(ret == -1)
			{
				std::cout << "[ERROR] Read Failed" << '\n';
			}

			// Add received summand
			memcpy(&data, buffer, sizeof(int));

			// Sum everything until client sends 0
			if(data == 0) break;

			result += data;
		}

		// Send result back to client
		memset(buffer, 0, BUFFER_SIZE);
		// std::cout << "Result = " << result << '\n';
		sprintf(buffer, "Result = %d", result);

		std::cout << "Sending final resutl to client" << '\n';
		// write() is not blocking system call
		ret = write(data_socket, buffer, BUFFER_SIZE);

		// Error handling
		if(ret == -1)
		{
			std::cout << "[ERROR] Write Failed" << '\n';
		}

		close(data_socket);

	}

	close(connection_socket);
	unlink(SOCKET_NAME);
}




/*
An array of active File descriptors which the server process is maintaining in order
to talk with the connected clients. Master FD is also member of this array.
Master FD plus all client's file descriptors
*/
int monitored_fd_set[MAX_CLIENT_SUPPORTED];

// Each connected client's intermediate result is maint. in this array
int client_result[MAX_CLIENT_SUPPORTED] = {0};

// API's related to fd_set
/*
Remove all FD's (if any), from the array
No FD at start
*/
static void initialize_monitor_fd_set()
{
	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		monitored_fd_set[i] = -1;
	}
}


static void add_to_monitored_fd_set(int skt_fd)
{
	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(monitored_fd_set[i] != -1) continue;
		monitored_fd_set[i] = skt_fd;
		break;
	}
}


static void remove_from_monitored_fd_set(int skt_fd)
{
	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(monitored_fd_set[i] != skt_fd) continue;
		monitored_fd_set[i] = -1;
		break;
	}
}

/*
Clone all FDs in monitored_fd_set array into 
fd_set data structure
fd_set - standard C structure
Remove all fd's from fd_set and copy the FD's to 
monitored_fd_set structure
Refresh the standard fd_set structure
*/
static void refresh_fd_set(fd_set* fd_set_ptr)
{
	FD_ZERO(fd_set_ptr);

	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(monitored_fd_set[i] != -1) 
		{
			FD_SET(monitored_fd_set[i], fd_set_ptr);
		}
	}
}


static int get_max_fd()
{
	int i = 0;
	int max = -1;


	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(monitored_fd_set[i] > max)
		{
			max = monitored_fd_set[i];
		}
	}

	return max;
}

// MULTIPLEXING SERVER
void serverMultiplex()
{
	/*
	Standard C APIs structure
	UNIX Domain Sockets - uniquely indentified by a name
	To hold name - define structure - google this - 2 members
	*/
	struct sockaddr_un name;

	int ret;
	int connection_socket;
	int data_socket;
	// int result;
	int data;
	char buffer[BUFFER_SIZE];
	fd_set readfds;
	int comm_socket_fd, i; 

	// Set all values to -1
	initialize_monitor_fd_set();

	unlink(SOCKET_NAME);

	// Create Master Socket File Description (FD) (Connection Socket)
	connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	// Error handling
	if(connection_socket == -1)
	{
		std::cout << "[ERROR] Master Multiplex Socket Creation Failed" << '\n';
	}
	std::cout << "Master Multiplex Socket Creation Success" << '\n';


	// [3 STEP] Specify the socket name - Initialize with 0 all bytes
	memset(&name, 0, sizeof(struct sockaddr_un));
	// Specify the socket credentials
	name.sun_family = AF_UNIX;
	// Copy socket name to name.sun_path
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);


	ret = bind(connection_socket, (const struct sockaddr*) &name, sizeof(struct sockaddr_un));
	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Bind Multiplex Failed" << '\n';
	}
	std::cout << "Server Multiplex Socket Bind Success" << '\n';


	ret = listen(connection_socket, 20);
	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Listen Multiplex Failed" << '\n';
	}
	std::cout << "Server Multiplex Listen Success" << '\n';


	// ALL STEPS SO FAR THE SAME AS SINGLE CLIENT 

	// 1 STEP: Add Master FD Socket to fd_set
	add_to_monitored_fd_set(connection_socket);

	// ---- Infinite server process Loop ----
	for(;;)
	{
		refresh_fd_set(&readfds);
		std::cout << "Waiting on select() system call" << '\n';
		// Blocking system call - Block on this line (Wait ... for new client or new msg)
		select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);

		// Is FD activated or not (New client or receive msg from connected client)
		// CIR - Connection Init Request
		if(FD_ISSET(connection_socket, &readfds))
		{
			std::cout << "New Connection received, accpet connection ..." << '\n';

			data_socket = accept(connection_socket, NULL, NULL);
			// Error handling
			if(data_socket == -1)
			{
				std::cout << "[ERROR] Accpet() Multiplex Failed" << '\n';
			}
			std::cout << "Accept() Multiplex Listen Success" << '\n';

			add_to_monitored_fd_set(data_socket);
		}
		else // Data arrives on some other client FD
		{
			i = 0; comm_socket_fd = -1;

			for(; i < MAX_CLIENT_SUPPORTED; i++)
			{
				if(FD_ISSET(monitored_fd_set[i], &readfds))
				{
					comm_socket_fd = monitored_fd_set[i];
					memset(buffer, 0, BUFFER_SIZE);

					std::cout << "Waiting for data from clients ... " << '\n';

					ret = read(comm_socket_fd, buffer, BUFFER_SIZE);
					if(ret == -1)
					{
						std::cout << "[ERROR] Read() Multiplex Failed" << '\n';
					}
					std::cout << "Read() Multiplex Success" << '\n';

					// Add received summand (data)
					memcpy(&data, buffer, sizeof(int));		
					if(data == 0) 
					{
						// Send result
						memset(buffer, 0, BUFFER_SIZE);
						sprintf(buffer, "Result = %d", client_result[i]);

						std::cout << "Sending final result back to client ... " << '\n';

						ret = write(comm_socket_fd, buffer, BUFFER_SIZE);
						if(ret == -1)
						{
							std::cout << "[ERROR] Write() Multiplex Failed" << '\n';
						}
						std::cout << "Write() Multiplex Success" << '\n';

						// Close Socket
						close(comm_socket_fd);
						client_result[i] = 0;
						remove_from_monitored_fd_set(comm_socket_fd);
						continue;
					}

				client_result[i] += data;
				}				
			}
		}
	} // Go to select and block

	close(connection_socket);
	remove_from_monitored_fd_set(connection_socket);

	std::cout << "Connection closed ..." << '\n';

	unlink(SOCKET_NAME);
}