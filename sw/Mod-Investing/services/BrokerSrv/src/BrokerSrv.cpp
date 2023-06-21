#include <memory>
#include "BrokerSrv.h"

#include "Factory.h"
#include "Database.h"

#include "ContainerIf.h"
#include "ObjectsManager.h"

#include "ClientServerSrvIf.h"
#include "ClientServerSrv.h"



Services::BrokerSrv::BrokerSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "/")
{
	std::cout << "[MasterSrv][InvDev] Services::BrokerSrv constructor" << '\n';
}


Services::BrokerSrv::~BrokerSrv()
{
	std::cout << "[MasterSrv][InvDev] Services::BrokerSrv destructor" << '\n';
}


const std::string& Services::BrokerSrv::getName()
{
	return m_name;
}


void Services::BrokerSrv::preInit()
{
	std::cout << "[MB][MasterSrv][InvDev] Services::BrokerSrv preInit()" << '\n';


	// Create Network Client
	std::shared_ptr<Services::ClientServerSrvIf> clientServerSrv = std::make_shared<Services::ClientServerSrv>("client", "client");
	Common::Factory::Factory::getInstance().setClientServerSrv(clientServerSrv);

	// Create data socket and connect
	Common::Factory::Factory::getInstance().getClientServerSrv()->preInit();


	// Get here all configuration info from xml file



	// [ FACTORY ] - Create other instances and devs

    // Create and Set Database in Factory (get this info from config xml)
    std::unique_ptr<Common::Database> database = std::make_unique<Common::Database>("Database/database_0.txt");
    Common::Factory::Factory::getInstance().setDatabase(database);

    // Create and Set Objects Manager in Factory (get name infro from config xml)
    // Create this as normal object in Factory
    std::shared_ptr<Services::ContainerIf> objectsManager = std::make_shared<Services::ObjectsManager>("objMgr", "objMgr");
    Common::Factory::Factory::getInstance().setObjectsManager(objectsManager);

    // Register and create instances from database
    Common::Factory::Factory::getInstance().registerClassConstructor();
    Common::Factory::Factory::getInstance().getInterfacesAndCreateInstances();

    // MasterSrv
    Common::Factory::Factory::getInstance().getMasterSrv()->preInit();

    // // ----==== START CLIENT SERVER ====----
    // [ CLIENT MESSAGE DISPATCHER SERVICE ]
	// Separate in different thread
	// Connect to MasterBrokerMod

    // m_clientServerSrv = Common::Factory::Factory::getInstance().getClientServerSrv();    
    // m_clientServerSrv->preInit();


}


void Services::BrokerSrv::postInit()
{

}


void Services::BrokerSrv::runClientServer()
{

}