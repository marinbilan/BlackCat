#include "Database.h"
#include "Factory.h"


Common::Database::Database(const std::string& name) : m_name(name)
{
	std::string trace_0 = "[MB][MasterSrv][InvDev] Common::Database constructor";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace_0);

	std::string trace_1 = "[MB][MasterSrv][InvDev] " + m_name + " database created";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace_1);
}


Common::Database::~Database()
{
	std::cout << "[MB][MasterSrv][InvDev] Common::Database destructor called!" << '\n';
}

void Common::Database::preInit()
{
}


void Common::Database::postInit()
{
}

void Common::Database::getStringsFromDB(const std::string& dbPath, std::vector<std::string>& vectorOfStrings)
{
	// dBFile - m_name: database text file name
	std::ifstream dBFile(m_name);
	// dB Line: each line in dBFile
	std::string dBLine;
	// DB ex:
	//                   dbPath                       "\\s+(.+)"
	//                   models             string     staticModel dynamicModels  otherModels
	std::regex regexLine("^" + dbPath + "\\s+" + "string" + "\\s+(.+)");

	std::smatch match;
	std::string wantedString;
	
	while (std::getline(dBFile, dBLine))
	{
		if (std::regex_search(dBLine, match, regexLine))
		{
			// Line is found
			wantedString = match.str(1); // match.str(0) - Whole matched string

			// Find and append all other lines		
			while(wantedString.back() == '\\')
			{
				// Remove last char \ from wanted string
				wantedString.pop_back();

				// Move to next line and concatinate
				std::getline(dBFile, dBLine);
				wantedString = wantedString.append(" " + dBLine);
			}

			// break;  //  If line is found, break loop
		}
	}

	// std::cout << "[MB] wanted string: " << wantedString << '\n';

	// wantedString: "staticModel dynamicModels  otherModels"
	std::istringstream stringOfElements(wantedString);

	// separate wantedString "staticModel dynamicModels  otherModels" in tokens (vector of strings)
	std::vector<std::string> vectorOfLocalStrings((std::istream_iterator<std::string>(stringOfElements)),
		std::istream_iterator<std::string>());

	vectorOfStrings = vectorOfLocalStrings;

	if (!wantedString.compare("")) // Raise error if no param in DB
	{
		std::string falseStr("N/A");
		vectorOfStrings.push_back(falseStr);
	}
}

/*
void Common::Database::getFloat(const std::string& dbPath, GLfloat& getFloat)
{
	// dB File
	std::ifstream dBFile(m_name);
	// dB Line
	std::string dBLine;
	// db Regex
	std::regex regexLine(dbPath + "\\s+" + "float" + "\\s+([-]?\\d+)(\\.\\d+)?");
	std::smatch match;
	while (std::getline(dBFile, dBLine))
	{
		if (std::regex_search(dBLine, match, regexLine))
		{
			getFloat = std::stof(match.str(1) + match.str(2)); // match.str(0) // Whole matched string
		}
	}
}

void Common::Database::getVec3(const std::string& dbPath, glm::vec3& vec3)
{
	// dB File
	std::ifstream dBFile(m_name);
	// dB Line
	std::string dBLine;
	// dB Regex
	std::regex regexLine(dbPath + "\\s+" + "vec3" + "\\s+([-]?\\d+)(\\.\\d+)?\\s+([-]?\\d+)(\\.\\d+)?\\s+([-]?\\d+)(\\.\\d+)?");

	std::smatch match;
	while (std::getline(dBFile, dBLine))
	{
		if (std::regex_search(dBLine, match, regexLine))
		{
			vec3 = glm::vec3(std::stof(match.str(1) + match.str(2)), // match.str(0) - whole string
				std::stof(match.str(3) + match.str(4)),
				std::stof(match.str(5) + match.str(6)));
		}
	}
}
*/