#pragma once

#include <memory>
#include <map>

// #include "FactoryIf.h"
#include "ServiceIf.h"
#include "Service0.h"


namespace Common
{

#define REGISTER_CLASS(ConstructorName) FactoryImpl::getInstance().registerClassConstructor<ConstructorName>(#ConstructorName)

// [1.2 Preparetion Step]: Create void Constructor prototype
template <class T> void* constructorPtr(const std::string& dbPath, const std::string& instanceName)
{
	return (void*)new T(dbPath, instanceName);
}


class FactoryImpl
{
public:

	static FactoryImpl& getInstance()
	{
		std::cout << "Creating FactoryImpl!" << '\n';
		static FactoryImpl instance;
		
		return instance;
	}

	~FactoryImpl()
	{
		std::cout << "Destroying FactoryImpl!" << '\n';
	}

	FactoryImpl(FactoryImpl const&) = delete;
	void operator=(FactoryImpl const&) = delete;


	// == PREPARATION STEPs ==
	// [1.1 Preparetion Step]: Register Class Constructor 
	void registerClassConstructor()
	{
		// Services
		REGISTER_CLASS(Service::Service0);
		// ...
	}

	// [1.3 Preparetion Step]: Register name of class and void pointer on Constructor
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

	void createInstances(const std::string& interfaceName)
	{
		// Create manually for now
		std::shared_ptr<Service::ServiceIf> srvInst((Service::ServiceIf*)constructObject("Service::Service0", "testPath", "service0_0"));
	}

	// [2.1 Construction Step]: Find constructor pointer for desired class
	void* constructObject(const std::string& constructorName, const std::string& dbPath, const std::string& instName)
	{
		// Find void ptr in map of classes ptr
		mapType::iterator it = m_classesMap.find(constructorName);

		if (it == m_classesMap.end()) return 0; // or throw or whatever you want  

		// Create instance out of that void ptr
		return it->second(dbPath, instName);
	}


private:
	// Singleton FactoryImpl - Private Constructor
	FactoryImpl() {}

	// FactoryImpl Stuff 
	//
	typedef void* (*constrPtr)(const std::string&, const std::string&);
	// Create Map<ConstructorName, void *ConstructorPtr(dbPath, instanceName)>
	typedef std::map<std::string, constrPtr> mapType;

	mapType m_classesMap;
};

} // End of namespace