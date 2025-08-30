#include "CppContainers_0.h"



Services::CppContainers0::CppContainers0(const std::string& name) : 
	m_name(name)
{
	std::cout << "Services::CppContainers0 constructor called!" << '\n';
}

Services::CppContainers0::~CppContainers0()
{
	std::cout << "Services::CppContainers0 destructor called!" << '\n';
}

const std::string& Services::CppContainers0::getName()
{

	return m_name;
}

void Services::CppContainers0::preInit()
{
	// Get params from DB for this instance
	std::cout << "----> CppContainers0 preInit() called!" << '\n';
}

void Services::CppContainers0::postInit()
{

}




Services::MyString::MyString(const char* src) : m_data( (char*)malloc((strlen(src) + 1) * sizeof(char)) )
{
	std::cout << "Calling Constructor. const char* src: " << src << '\n';
	memcpy(m_data, src, strlen(src) + 1);
}

// COPY CONSTRUCTOR
Services::MyString::MyString(const MyString& other) : m_data( (char*)malloc((strlen(other.m_data) + 1) * sizeof(char)) )
{
	std::cout << "Calling Copy Constructor. const MyString& other: " << other << '\n';
	memcpy(m_data, other.m_data, strlen(other.m_data) + 1);
}

// ASSIGNMENT OPERATOR
Services::MyString& Services::MyString::operator=(const MyString& other) {
	std::cout << '\n' << "Calling Assignment Operator. const MyString& other: " << other << '\n';

	if (this != &other) {
	  free(m_data);
	  m_data = (char*)malloc((strlen(other.m_data) + 1) * sizeof(char));
	  memcpy(m_data, other.m_data, strlen(other.m_data) + 1);
	}

	return *this;
}

// Rule of five

void Services::MyString::info()
{
	std::cout << '\n' << "[OBJ INFO] " << '\n' << "Address: " << this << " m_data: " << m_data << " Length: " << strlen(m_data) << '\n';

	for (size_t i = 0; i < strlen(m_data); ++i) {
	  std::cout << static_cast<const void*>(m_data + i)  << " m_data[" << i << "] = " << m_data[i] << '\n';
	}
}


std::ostream& Services::operator<<(std::ostream& os, const MyString& str) {
  os << str.m_data;
  return os;
}










