#include <string>
#include <iostream>


auto greeter(const std::string& salutation) {
	// In return method name is argument
	return [salutation](const std::string& name) {
		return salutation + ", " + name; 
	};
}


class CompareTest {
public:
	CompareTest(int id) : m_id(id) {}


	bool operator==(const CompareTest& rhs) {
		return m_id == rhs.m_id;
	}

// public:
int m_id;
};