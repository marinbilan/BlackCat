#include "ServiceBaseDerv.h"
// #include "Factory.h"
// #include "FactoryImpl.h"


Service::ServiceBaseDerv::ServiceBaseDerv(const std::string& dbPath, const std::string& name) : 
m_name(name),
ServiceBase(dbPath, name)
{
}


Service::ServiceBaseDerv::~ServiceBaseDerv()
{
}


void Service::ServiceBaseDerv::preInit()
{
	// Get params from DB for this instance
	std::cout << "ServiceBaseDerv preInit() called!" << '\n';
}


void Service::ServiceBaseDerv::postInit()
{

}


void Service::ServiceBaseDerv::testVirtualMethod()
{
	std::cout << "Call from ServiceBaseDerv" << '\n';


}


template<typename FileCallback>
void Service::ServiceBaseDerv::readAllFiles(FileCallback& funcObj)
{
	std::cout << "Call readAllFiles ..." << '\n';

    // Foreach path call generic funcObj
    std::string path1("...path1");
    // std::string path1("...path1"); // To test two same confs in map
    std::string path2("...path2");
    std::string path3("...path3");
       
    funcObj(path1);
    funcObj(path2);
    funcObj(path3);	
}


void Service::ServiceBaseDerv::initAllConfs()
{
	std::cout << "Call initAllConfs ..." << '\n';

    bool valsOK = false;
       
    // Important, because we pass 'this' as capture variable,
    // we are able to call m_XMLReader and m_mapConfs in other object
    // when we pass lambda further!
    std::function<int(std::string&)> readConf = [this, &valsOK](const std::string& fullPath) -> int
    {
        std::cout << "Parsing : " << fullPath << '\n';
           
        // Do some reading using member parser ex:
        // Configuration cfg;
        // m_XMLReader->readConf(cfg); // Get cfg object via reference and insert in map
           
        /*
        IMPORTATNT: It is not possible to insert two pairs with
        the same second into map. This is good check that configuration already
        exists
        */
        m_mapConfs.insert(std::make_pair(fullPath, 0));
           
        // This is how to check if configuration already exists in map - use this
        // if(!m_mapConfs.insert(std::make_pair(fullPath, 0)).second) std::cout << "Conf already exists!" << '\n';
        // This is how it should look like
        // if(!m_mapConfs.insert(std::make_pair(cfg.getId(), cfg).second) std::cout << "Conf already exists!" << '\n';
           
        valsOK = true;
           
        // Trigger error handling
        // if(throwExc) throw MARException(ErrorCodes::FILE_NOT_FOUND);

        /*
        // Handle exception in production code
        try
	    {
	        w1.throwExc(true);
	    }
	    catch(MARException ex)
	    {
	        std::cout << ErrorCodes::GetErrorDesc(ex.getErrorCode()) << '\n';
	    }
    */

        return 0; // Return some ok/error info
    };
       
    readAllFiles(readConf);
}


// Optional version - Get copy of map iterator
std::optional<std::map<std::string,int>::iterator> Service::ServiceBaseDerv::getIterConfigOpt()
{
    // Desired config
    std::pair<std::string, int> desiredConfig("...path2", 0);
    auto desiredConfigFunc = [&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; };
       
    // Iterate via map
    auto foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), desiredConfigFunc);
    if(foundConfigInMap != m_mapConfs.end())
    {
        std::cout << "Value is found" << '\n';
        return foundConfigInMap;
    }

    return std::nullopt;
}


// Non Optional version - Get copy of map iterator
// Compact longer if
std::map<std::string,int>::iterator Service::ServiceBaseDerv::getIterConfigNoOpt(std::pair<std::string, int> const& desiredConfig)
{
    // Iterate via map
    if(auto foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), 
    	[&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; });
	    foundConfigInMap != m_mapConfs.end() )
	    {
	        std::cout << "foundConfigInMap: " << foundConfigInMap->second << '\n';
	        std::cout << "foundConfigInMap->second address: " << &(foundConfigInMap->second) << '\n';
	        return foundConfigInMap;
	    }

    return m_mapConfs.end();
}


// std::optional<int> getConfigFromMap(std::pair<std::string, int> const& desiredConfig)
// Compact longer if
std::optional<std::reference_wrapper<int>> Service::ServiceBaseDerv::getConfigFromMap(std::pair<std::string, int> const& desiredConfig)
{
    // Iterate via map - this iterator always copy value in itself
    if(const auto& foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), 
    	[&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; });
		foundConfigInMap != m_mapConfs.end())
	    {
	        std::cout << "found ConfigInMap: " << foundConfigInMap->second << '\n';
	        return foundConfigInMap->second;
	    }

    return std::nullopt;
}