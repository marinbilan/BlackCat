#include "SrvLinuxSys.h"


// HELPER METHODs
/*
An array of active File descriptors which the server process is maintaining in order
to talk with the connected clients. Master FD is also member of this array.
Master FD plus all client's file descriptors
*/
int m_MonitoredFdSet[MAX_CLIENT_SUPPORTED];

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
		m_MonitoredFdSet[i] = -1;
	}

	// Init map - no need
}


static void add_to_monitored_fd_set(int skt_fd)
{
	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(m_MonitoredFdSet[i] != -1) continue;
		m_MonitoredFdSet[i] = skt_fd;
		break;
	}

	// Insert in map
	// m_MonitoredFdSetMap.insert({"TestClient", skt_fd});
}


static void remove_from_monitored_fd_set(int skt_fd)
{
	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(m_MonitoredFdSet[i] != skt_fd) continue;
		m_MonitoredFdSet[i] = -1;
		break;
	}
}

/*
Clone all FDs in m_MonitoredFdSet array into 
fd_set data structure
fd_set - standard C structure
Remove all fd's from fd_set and copy the FD's to 
m_MonitoredFdSet structure
Refresh the standard fd_set structure
*/
static void refresh_fd_set(fd_set* fd_set_ptr)
{
	FD_ZERO(fd_set_ptr);

	int i = 0;
	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(m_MonitoredFdSet[i] != -1) 
		{
			FD_SET(m_MonitoredFdSet[i], fd_set_ptr);
		}
	}
}


static int get_max_fd()
{
	int i = 0;
	int max = -1;


	for(; i < MAX_CLIENT_SUPPORTED; i++)
	{
		if(m_MonitoredFdSet[i] > max)
		{
			max = m_MonitoredFdSet[i];
		}
	}

	return max;
}


// SrvLinuxSys
Services::SrvLinuxSys::SrvLinuxSys(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::SrvLinuxSys constructor called!" << '\n';
}


Services::SrvLinuxSys::~SrvLinuxSys()
{
	std::cout << "Services::SrvLinuxSys destructor called!" << '\n';
}


const std::string& Services::SrvLinuxSys::getName()
{
	

	return m_name;
}


void Services::SrvLinuxSys::preInit()
{
	// Get params from DB for this instance
	// std::cout << "Services::SrvLinuxSys preInit() called!" << '\n';
	std::cout << "Services::SrvLinuxSys Server Start!" << '\n';


	// ---- SERVER ----

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
	std::cout << "[1st STEP] Master Multiplex Socket Creation Success" << '\n';


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
	std::cout << "[2nd STEP] Server Multiplex Socket Bind Success" << '\n';


	ret = listen(connection_socket, 20);
	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Listen Multiplex Failed" << '\n';
	}
	std::cout << "[3th STEP] Server Multiplex Listen Started" << '\n';


	// ALL STEPS SO FAR THE SAME AS SINGLE CLIENT 

	// 1 STEP: Add Master FD Socket to fd_set
	add_to_monitored_fd_set(connection_socket);

	// ---- Infinite server process Loop ----
	for(;;)
	{
		refresh_fd_set(&readfds);
		std::cout << "[4th STEP] Refresh fd_set and waiting on select() system call" << '\n';
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
			std::cout << "Accept() Client Success" << '\n';

			add_to_monitored_fd_set(data_socket);
		}
		else // Data arrives on some other client FD
		{
			i = 0; comm_socket_fd = -1;

			for(; i < MAX_CLIENT_SUPPORTED; i++)
			{
				if(FD_ISSET(m_MonitoredFdSet[i], &readfds))
				{
					comm_socket_fd = m_MonitoredFdSet[i];
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


void Services::SrvLinuxSys::postInit()
{

}