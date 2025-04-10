#include "SrvLinuxSys.h"


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
	std::cout << "[INFO] Services::SrvLinuxSys Server Start!" << '\n';

	/*
	Standard C APIs structure
	UNIX Domain Sockets - uniquely indentified by a name
	*/
	int                masterSocketId;
	struct sockaddr_un sockAddrName;	
	int    dataSocketId;
	char   buffer[BUFFER_SIZE];
	fd_set readFds;


	unlink(SOCKET_NAME);

	// [1 STEP] Create Master Socket File Description (FD) (Connection Socket)
	//          Create socket Desrciption
	masterSocketId = socket(AF_UNIX, SOCK_STREAM, 0);
	// Error handling
	if(masterSocketId == -1)
	{
		std::cout << "[ERROR] Master connection socket creation failed" << '\n';
	}
	std::cout << "[INFO] [1st STEP] Master connection socket creation OK" << '\n';


	// [1.1 STEP] Specify the socket name (sockAddrName) - Initialize with 0 all bytes
	memset(&sockAddrName, 0, sizeof(struct sockaddr_un));
	// Specify the socket credentials
	sockAddrName.sun_family = AF_UNIX;
	// Copy socket sockAddrName to sockAddrName.sun_path
	strncpy(sockAddrName.sun_path, SOCKET_NAME, sizeof(sockAddrName.sun_path) - 1);

	/*
	// [1] Create SOCKET              [1.2] Create struct with socket DESCRIPTION
	int masterSocketId =              struct sockaddr_un - sockAddrName
	socket(AF_UNIX, SOCK_STREAM, 0);
	+------------+                    +-------------------------------+
	|AF_UNIX     |                    |sun_family - AF_UNIX           |
	|SOCK_STREAM |                    |sun_path   - "/tmp/DemoSocket" |
	+------------+                    +-------------------------------+

	[3] Bind SOCKET with Socket Description (Name - "/tmp/DemoSocket")
	+----------------------------------------------+
	|AF_UNIX      +-------------------------------+|
	|SOCK_STREAM  |sun_family - AF_UNIX           ||
	|             |sun_path   - "/tmp/DemoSocket" ||
	|             +-------------------------------+|
    +----------------------------------------------+
	*/
	// [2 STEP] Bind Master Socket (masterSocketId) and sockaddr_un (Socket Description)
	// Error handling
	if(bind(masterSocketId, (const struct sockaddr*) &sockAddrName, sizeof(struct sockaddr_un)) == -1)
	{
		std::cout << "[ERROR] Socket - description bind() failed" << '\n';
	}
	std::cout << "[INFO] [2nd STEP] Socket - description bind() OK" << '\n';

	// [3 STEP] Listen on connection (Master socket) - Max of 20 pending connections
	// Error handling
	if(listen(masterSocketId, 20) == -1)
	{
		std::cout << "[ERROR] Listen on master scoket failed" << '\n';
	}
	std::cout << "[INFO] [3th STEP] Listen on master scoket OK" << '\n';

	// [4 STEP] Add Master FD Socket to fd_set
	// Insert master FD socket in map
	m_MonitoredFdSetMap.insert({masterSocketId, "Client"});


	// ---- Infinite Server Process Loop ----
	while(true)
	{
		// EVENT Occures:
		//     [Connect / Disconnect Client] or
		//     [Receive msg from existing client] 
		// Refresh fd set from m_MonitoredFdSetMap	
		FD_ZERO(&readFds);  // Set all FD's to zero
		
		for(auto const& [key, value] : m_MonitoredFdSetMap)  // Update FD_SET with client Ids from FD_SET map
		{
			FD_SET(key, &readFds);
		}

		std::cout << "> [4th STEP] Refresh fd_set and waiting on select() ..." << '\n';
		// Blocking system call
		// Get max clientId (last in (sorted) map)
		int maxFd = m_MonitoredFdSetMap.rbegin()->first; // Get last (biggest) client Id
		select(maxFd + 1, &readFds, NULL, NULL, NULL);

		// Check is masterSocketId activated (New connection or Data recv from existing client(s))
		// CIR - Connection Init Request (Data arrives on master FD)
		if(FD_ISSET(masterSocketId, &readFds))
		{
			std::cout << "[EVENT] New connection received, accpet connection ..." << '\n';

			dataSocketId = accept(masterSocketId, NULL, NULL);

			if(dataSocketId == -1)
			{
				std::cout << "[ERROR] New client accpet() failed" << '\n';
			}
			std::cout << "[INFO] New client accept() OK" << '\n';

			m_MonitoredFdSetMap.insert({dataSocketId, "Client"});

			// After CIR OK - Wait for just connected client info
			memset(buffer, 0, BUFFER_SIZE);
			if(read(dataSocketId, buffer, BUFFER_SIZE) <= 0)
			{
				std::cout << "[ERROR] Client info rcvd failed " << '\n';
			} 

			std::string rcvdClientInfoMsgStr(buffer);  // Wrap buffer into string
			std::cout << "[INFO] Client info msg rcvd: " <<  rcvdClientInfoMsgStr << '\n';

			// Update clients map value (client info)
			m_MonitoredFdSetMap[dataSocketId] = rcvdClientInfoMsgStr;
			std::cout << "[INFO] Updated client info OK" << '\n';
		}
		else // Data arrives on some other client FD
		{				
		 	// Iterate via all items (clients) in map
		    // const auto count = std::erase_if(m_MonitoredFdSetMap, [this, &readFds, &buffer](const auto& it) {
		    std::erase_if(m_MonitoredFdSetMap, [this, &readFds, &buffer](const auto& it) {

		        auto const& [commSocketFdId, clientName] = it;
		        /*
		        Check if current (commSocketFdId) File Descriptor (FD) is set
		        Check for event:
		        	- Disconnect (if)
		           	- Message sent (else)
		        */
		        bool delItemFromMap = false;

				if(FD_ISSET(commSocketFdId, &readFds))
				{
					// Clear Buffer: Set 0 to all bytes size of BUFFER_SIZE = 128 [ buffer* - 00000...0 (128 zeros) ]
					memset(buffer, 0, BUFFER_SIZE);

					// Delete client if disconnect event occures
					if(read(commSocketFdId, buffer, BUFFER_SIZE) <= 0) // Blocking call
					{
						std::cout << "[EVENT] Disconnect clientId: " << commSocketFdId << '\n';

						close(commSocketFdId);
						// Delete client (item) if disconnect event occures on current (commSocketFdId) FD
						delItemFromMap = true;
					}
					else // [EVENT] Client send message Process message
					{
						std::string receivedMessageStr(buffer);

						// >>>> Processing Incomming Message <<<<
						receiveMessage(commSocketFdId, receivedMessageStr);
					}
				}

		        return delItemFromMap; // true / false
		    });
		}
	} // Go to select and block - wait for next event

	close(masterSocketId);
	// TODO: Remove master socket from Map

	std::cout << "Connection closed ... bye" << '\n';

	unlink(SOCKET_NAME);
}


void Services::SrvLinuxSys::postInit()
{

}


void Services::SrvLinuxSys::receiveMessage(int commSocketFdId, const std::string& rcvMsg)
{
	std::cout << "[EVENT] >>>> Received Message: " << rcvMsg << '\n';
	std::cout << "             Size: " << rcvMsg.length() << '\n';

	// ----
    // PARSE Input message
    std::regex pattern(R"(\[([^\]]+)\])");

    std::smatch matches;
    std::string::const_iterator searchStart(rcvMsg.cbegin());

    std::regex_search(searchStart, rcvMsg.cend(), matches, pattern);
    // ----

    if(!matches[1].str().compare("Broadcast"))
    {
    	std::cout << "This is broadcast call" << '\n';
    } 
    else 
    {
    	std::cout << "This is NOT broadcast call" << '\n';
    }



	// After message is processed, send msg back
	std::string msgForClient("OK");
	send(commSocketFdId, msgForClient.c_str(), strlen(msgForClient.c_str()), 0);
}