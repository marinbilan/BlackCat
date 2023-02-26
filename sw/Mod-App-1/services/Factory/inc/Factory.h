#pragma once

#include <memory>
#include <map>


// Common
#include "Database.h"
#include "ContainerIf.h"
// #include "ObjectsManager.h"

#include "ServiceIf.h"
//#include "Service0.h"
//#include "Service1.h"
#include "ServiceX.h"




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
		std::cout << "Creating Factory!" << '\n';
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
		std::cout << "Factory::preInit() method called!" << '\n';

	}

	/*! @brief  Set database object created from init object (during init phase)
	*   @param  database
	*   @return void
	*/
	void setDatabase(std::unique_ptr<Common::Database>& database)
	{
		m_database = std::move(database);
	}

	/*! @brief  Set objects manager object created from init object (during init phase)
	*   @param  database
	*   @return void
	*/
	void setObjectsManager(std::shared_ptr<Services::ContainerIf>& objectsManager)
	{
		m_objectsManager = objectsManager;
	}

	/*! @brief  Get reference on global database object
     *  @return reference on database object
     */
	std::unique_ptr<Common::Database>& getDatabase()
	{
		return m_database;
	}
	// ---- ----


	// == PREPARATION STEPs ==
	// [1.1 Preparetion Step]: Register Class Constructor 
	void registerClassConstructor()
	{
		// Services
		REGISTER_CLASS(Services::ServiceX);
		//REGISTER_CLASS(Services::Service0);
		//REGISTER_CLASS(Service::Service1);
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
			std::string instanceDbPath = interfaceDBPath + "_" + s + "_";

			// Foreach class
			// ServiceIf_ServiceX_constructorName - Service::ServiceX (Constructor name)
			std::string constructorNameDbPath = interfaceDBPath + "_" + s + "_" + "constructorName";
			// ServiceIf_ServiceX_instanceNames - serviceX_0 serviceX_1 ... (Instance names)
			std::string instanceNameDbPath = interfaceDBPath + "_" + s + "_" + "instanceNames";


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
			if (!interfaceDBPath.compare("ServiceIf"))
			{
				// std::cout << "Factory::createInstances() Creating instance: " << s << '\n';
				// CONSTRUCT OBJECT
				std::shared_ptr<Services::ServiceIf> serviceIfPtr((Services::ServiceIf*)constructObject(vecOfConstructorString[0], instanceDbPath, s));

				
				std::cout << "Factory inst name: " << serviceIfPtr->getName() << '\n';
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


private:
	// Singleton Factory - Private Constructor
	Factory() {}

	// Factory Stuff 
	//
	typedef void* (*constrPtr)(const std::string&, const std::string&);
	// Create Map<ConstructorName, void *ConstructorPtr(dbPath, instanceName)>
	typedef std::map<std::string, constrPtr> mapType;

	mapType m_classesMap;

	// DataBase
	std::unique_ptr<Common::Database> m_database;
	// Objects Manager
	std::shared_ptr<Services::ContainerIf> m_objectsManager;
};

} // End of namespace