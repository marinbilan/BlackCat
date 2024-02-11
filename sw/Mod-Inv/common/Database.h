#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>


namespace Common
{
class Database
{
public:
    Database(const std::string& name);
	// Database(Common::Error& err, const std::string& name);

	~Database();

	void preInit();
	void postInit();

	void getStringsFromDB(const std::string& dbPath, std::vector<std::string>& vectorOfStrings);

private:
std::string m_name;
};

} // End of Namespace Common