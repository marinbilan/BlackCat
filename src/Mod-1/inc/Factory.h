#pragma once

#include <memory>
#include <map>

// #include "ServiceIf.h"
#include "Service0.h"
#include "Service1.h"


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


	// == PREPARATION STEPs ==
	// [1.1 Preparetion Step]: Register Class Constructor 
	void registerClassConstructor()
	{
		// Services
		REGISTER_CLASS(Service::Service0);
		REGISTER_CLASS(Service::Service1);
		// ...
	}

	// [1.2 Preparetion Step]: Register name of class and void pointer on Constructor
	template <class T>
	void registerClassConstructor(std::string const& constructorName)
	{
		// First argument is constructor name
		// Second argument is classConstructorPointer (&constructorNew<T>) it is generic pointer to generic class constructor
		m_classesMap.insert(std::make_pair(constructorName, &constructorPtr<T>));
	}


	// == CONSTRUCTION STEPs ==
	// [2.1 Construction Step]: Create objects from database
	void getInterfacesAndCreateInstances()
	{
		// 1] Get objects interfaces from databse

		// 2] Create objects from classes derived from that interfaces
	}

	// [2.2 Construction Step]:
	void createInstances(const std::string& interfaceName)
	{
		// Create manually for now
		std::shared_ptr<Service::ServiceIf> srvInst00((Service::ServiceIf*)constructObject("Service::Service0", "testPath", "service0_0"));
		std::shared_ptr<Service::ServiceIf> srvInst10((Service::ServiceIf*)constructObject("Service::Service1", "testPath", "service1_0"));
	}

	// [2.3 Construction Step]: Find constructor pointer for desired class
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
};

} // End of namespace