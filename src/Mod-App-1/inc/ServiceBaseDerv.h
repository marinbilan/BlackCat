#pragma once
#include "ServiceBase.h"
#include <exception>
#include <optional>


/*
// Put error handling in common file
// //// ERROR HANDLING ////
struct ErrorCodes
{
    static const short FILE_NOT_FOUND = 21;
   
    static std::string GetErrorDesc(short const code);
};


static const std::map<short, char const* const> descriptions =
{
    { ErrorCodes::FILE_NOT_FOUND, "ERROR: File not found" }
};

std::string ErrorCodes::GetErrorDesc(short const code)
{
    const auto it = descriptions.find(code);
    if(descriptions.cend() == it)
    {
        return "Unknown error";
    }
    return it->second;
}

class MARException : public std::exception
{
public:
    MARException(short code) : code(code) {}
   
    short getErrorCode() const
    {
        return code;
    }

private:
short code;
};
// //// ////
*/


namespace Service
{

class ServiceBaseDerv : public ServiceBase
{
public:
	ServiceBaseDerv(const std::string& dbPath, const std::string& name);
	~ServiceBaseDerv();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Call Factory singleton here
	void testVirtualMethod();

	template<typename FileCallback>
	void readAllFiles(FileCallback& funcObj);

	void initAllConfs();

	std::optional<std::map<std::string,int>::iterator> getIterConfigOpt();

	std::map<std::string,int>::iterator getIterConfigNoOpt(std::pair<std::string, int> const& desiredConfig);

	std::optional<std::reference_wrapper<int>> getConfigFromMap(std::pair<std::string, int> const& desiredConfig);

private:
// std::string m_dbPath;
// std::string m_dbPathWithName;
std::string m_name;

std::map<std::string, int> m_mapConfs;
// Common::FactoryIf* m_factoryImplSingleton;
};

}