#pragma once

#include <memory>
#include <map>
#include <ctime>


// Common
#include "Database.h"
#include "Log.h"
#include "ContainerIf.h"
// #include "ObjectsManager.h"

#include "MasterSrvIf.h"
#include "MasterSrv.h"
#include "InvDevIf.h"
#include "InvDev0.h"

#include "ClientServerSrvIf.h"
#include "ClientServerSrv.h"
#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"
#include "ServiceIf.h"
//#include "Service0.h"
//#include "Service1.h"
#include "ServiceX.h"


/*
Adding new service procedure:
1] cp (folders) and rename ServiceX (files)
2] Add folder path in Makefile
3] Add service in database
In factory:
	4] REGISTER_CLASS (constructor)
	5] Include .h files
	6] Add interface cmpr in createInstances(...)
	7] Add container in object manager
*/


#define REGISTER_CLASS(ConstructorName) Factory::getInstance().registerClassConstructor<ConstructorName>(#ConstructorName)

namespace Common
{

// [1.3 Preparetion Step]: Create void Constructor prototype
template <class T> void* constructorPtr(const std::string& dbPath, const std::string& instanceName)
{
	return (void*)new T(dbPath, instanceName);
}

class Factory
{
public:

	static Factory& getInstance()
	{
		static Factory instance;
		
		return instance;
	}

	~Factory()
	{
		std::cout << "Destroying Factory!" << '\n';
	}

	Factory(Factory const&) = delete;
	void operator=(Factory const&) = delete;


	// ---- INIT ----
	void preInit()
	{
		// CREATE LOG NAME AS TIME STAMP
		time_t curr_time;
		tm* curr_tm;
	
		char date_string[100]; 
		char time_string[100];	

		time(&curr_time);
	
		curr_tm = localtime(&curr_time);
	
		strftime(date_string, 50, "%d%Y", curr_tm);
		strftime(time_string, 50, "_%T", curr_tm);

		std::string str1(date_string);
		std::string str2(time_string);
		
		std::string strLogTimeName = str1 + str2;
		std::replace(strLogTimeName.begin(), strLogTimeName.end(), ':', '_'); // replace all ':' to '_'

		m_log = std::make_unique<Common::Log>(strLogTimeName);
	}

	// ---- ----


	// == PREPARATION STEPs ==
	// [1.1 Preparetion Step]: Register Class Constructor 
	void registerClassConstructor()
	{
		std::string trace_0 = "[MB][MasterSrv][InvDev] Common::Factory registerClassConstructor()";
		getClientServerSrv()->TRACE(trace_0);

		// Services
		REGISTER_CLASS(Services::MasterSrv);
		REGISTER_CLASS(Services::InvDev);
		REGISTER_CLASS(Services::ClientServerSrv);
		REGISTER_CLASS(Services::HTTPSProxySrv);
		REGISTER_CLASS(Services::ServiceX);
		
		// ...
	}

	// [1.2 Preparetion Step]: Register name of class and void pointer on Constructor
	template <class T>
	void registerClassConstructor(std::string const& constructorName)
	{
		// First argument is constructor name
		// Second argument is classConstructorPointer (&constructor<T>) it is generic pointer to generic class constructor
		m_classesMap.insert(std::make_pair(constructorName, &constructorPtr<T>));
	}


	// == CONSTRUCTION STEPs ==
	// [2.1 Construction Step]: Create objects from database
	void getInterfacesAndCreateInstances()
	{
		std::string trace_0 = "[MB][MasterSrv][InvDev] Common::Factory getInterfacesAndCreateInstances()";
		getClientServerSrv()->TRACE(trace_0);

		// Get objects interfaces from databse
		std::vector<std::string> vecOfInterfacesStrings;
		m_database->getStringsFromDB("Interfaces", vecOfInterfacesStrings);

		// ServiceIf ControlIf ...
		for (auto s : vecOfInterfacesStrings)
		{
			// Create objects from classes derived from that interfaces
			createInstances(s);
		}
	}


	// [2.2 Construction Step]:
	void createInstances(const std::string& interfaceDBPath)
	{
		
		std::vector<std::string> vecOfDerivedClassesStrings;
		m_database->getStringsFromDB(interfaceDBPath, vecOfDerivedClassesStrings);

		// ServiceX ServiceY ServiceZ
		for (auto s : vecOfDerivedClassesStrings)
		{
			// CREATE DATABASE PATHs
			// Constructor argument - Not used for creation
			std::string instanceDbPath = interfaceDBPath + "/" + s + "/";

			// Foreach class
			// ServiceIf_ServiceX_constructorName - Service::ServiceX (Constructor name)
			std::string constructorNameDbPath = interfaceDBPath + "/" + s + "/" + "constructorName";
			// ServiceIf_ServiceX_instanceNames - serviceX_0 serviceX_1 ... (Instance names)
			std::string instanceNameDbPath = interfaceDBPath + "/" + s + "/" + "instanceNames";


			// GET DATA FROM DATABASE
			// Service::ServiceX (vector with one element)
			std::vector<std::string> vecOfConstructorString;
			m_database->getStringsFromDB(constructorNameDbPath, vecOfConstructorString);

			// serviceX_0 serviceX_1 ... (All ServiceX instances)
			std::vector<std::string> vecOfInstancesString;
			m_database->getStringsFromDB(instanceNameDbPath, vecOfInstancesString);


			createInstances(vecOfInstancesString, interfaceDBPath, vecOfConstructorString, instanceDbPath);
		}

	}


	// [2.3 Construction Step]:
	void createInstances(const std::vector<std::string>& vecOfInstancesString, const std::string& interfaceDBPath,
		const std::vector<std::string>& vecOfConstructorString, const std::string& instanceDbPath)
	{
		// Pass DB data to method
		// ex: service0_0, service0_1 ...
		for (auto s : vecOfInstancesString)
		{
			if (!interfaceDBPath.compare("MasterSrvIf"))
			{
				// CONSTRUCT OBJECT
				std::shared_ptr<Services::MasterSrvIf> serviceIfPtr((Services::MasterSrvIf*)constructObject(vecOfConstructorString[0], instanceDbPath, s));

				m_masterSrv = serviceIfPtr;
			}
			if (!interfaceDBPath.compare("InvDevIf"))
			{
				// CONSTRUCT OBJECT
				std::shared_ptr<Services::InvDevIf> serviceIfPtr((Services::InvDevIf*)constructObject(vecOfConstructorString[0], instanceDbPath, s));

				m_objectsManager->setInvDevIf(serviceIfPtr);
			}
			if (!interfaceDBPath.compare("HTTPSProxySrvIf"))
			{
				// CONSTRUCT OBJECT
				std::shared_ptr<Services::HTTPSProxySrvIf> serviceIfPtr((Services::HTTPSProxySrvIf*)constructObject(vecOfConstructorString[0], instanceDbPath, s));

				// std::cout << "Factory inst name: " << serviceIfPtr->getName() << '\n';
				// STORE INTERFACE OBJECT POINTER
				// m_objectsManager->setServiceIf(serviceIfPtr);
			}
			if (!interfaceDBPath.compare("ServiceIf"))
			{
				// CONSTRUCT OBJECT
				std::shared_ptr<Services::ServiceIf> serviceIfPtr((Services::ServiceIf*)constructObject(vecOfConstructorString[0], instanceDbPath, s));

				// std::cout << "Factory inst name: " << serviceIfPtr->getName() << '\n';
				// STORE INTERFACE OBJECT POINTER
				m_objectsManager->setServiceIf(serviceIfPtr);
			}
		}
	}


	// [2.4 Construction Step]: Find constructor pointer for desired class and create instance
	void* constructObject(const std::string& constructorName, const std::string& dbPath, const std::string& instName)
	{
		// Find void ptr in map of classes ptr
		mapType::iterator it = m_classesMap.find(constructorName);

		if (it == m_classesMap.end()) return 0; // or throw or whatever you want  

		// Create instance out of that void ptr
		return it->second(dbPath, instName);
	}

	void testFactoryPrint()
	{
		std::cout << "Print from Common::Factory testPrint" << '\n';
	}


	// ---- GETTERs ----
		/*! @brief  Get reference on global database object
     *  @return reference on database object
     */
	std::unique_ptr<Common::Database>& getDatabase()
	{
		return m_database;
	}

	/*! @brief  Get reference on global log object
	 *  @param  -
	 *  @return reference on log object
	 */
	std::unique_ptr<Common::Log>& getLog()
	{
		return m_log;
	}

	std::shared_ptr<Services::ContainerIf> getObjectsManager()
	{
		if(m_objectsManager != nullptr)
		{
			return m_objectsManager;
		}

		return nullptr;
	}

	std::shared_ptr<Services::MasterSrvIf> getMasterSrv()
	{
		if(m_masterSrv != nullptr)
		{
			return m_masterSrv;
		}

		return nullptr;
	}

	std::shared_ptr<Services::ClientServerSrvIf> getClientServerSrv()
	{
		if(m_clientServerSrv != nullptr)
		{
			return m_clientServerSrv;
		}

		return nullptr;
	}

	// ---- SETTERs ----
	/*! @brief  Set database object created from init object (during init phase)
	*   @param  database
	*   @return void
	*/
	void setDatabase(std::unique_ptr<Common::Database>& database)
	{
		std::string trace_0 = "[MB][MasterSrv][InvDev] Common::Factory setDatabase()";
		getClientServerSrv()->TRACE(trace_0);

		m_database = std::move(database);
	}

	/*! @brief  Set objects manager object created from init object (during init phase)
	*   @param  database
	*   @return void
	*/
	void setObjectsManager(std::shared_ptr<Services::ContainerIf>& objectsManager)
	{
		std::string trace_0 = "[MB][MasterSrv][InvDev] Common::Factory setObjectsManager()";
		getClientServerSrv()->TRACE(trace_0);

		m_objectsManager = objectsManager;
	}

	void setClientServerSrv(std::shared_ptr<Services::ClientServerSrvIf>& clientServer)
	{
		m_clientServerSrv = clientServer;
	}


private:
	// Singleton Factory - Private Constructor
	Factory() {}

	// Factory Stuff 
	//
	typedef void* (*constrPtr)(const std::string&, const std::string&);
	// Create Map<ConstructorName, void *ConstructorPtr(dbPath, instanceName)>
	typedef std::map<std::string, constrPtr> mapType;

	mapType m_classesMap;


	// Client Server
	std::shared_ptr<Services::ClientServerSrvIf> m_clientServerSrv;
	// DataBase
	std::unique_ptr<Common::Database> m_database;
	std::unique_ptr<Common::Log> m_log;
	// Objects Manager
	std::shared_ptr<Services::ContainerIf> m_objectsManager;

	// Master Service
	std::shared_ptr<Services::MasterSrvIf> m_masterSrv;

};

} // End of namespace