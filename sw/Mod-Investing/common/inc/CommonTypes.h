#pragma once
#include <string>
#include <vector>

// Factory stuff
#define FACTORY Common::Factory::Factory::getInstance()

// DB stuff
// #define DB      Common::Factory::getInstance() // .getDatabase() // ->get

// Log stuff
#define LOG __DATE__, __TIME__, __FILENAME__, __LINE__,
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)


/*
void vecToString(std::string& trace, std::vector<double>& vec) {
    for(const auto& s : vec) {
		trace += std::to_string(s) + " ";
	}
}
*/

// Active Object stuff
// typedef std::string Message;