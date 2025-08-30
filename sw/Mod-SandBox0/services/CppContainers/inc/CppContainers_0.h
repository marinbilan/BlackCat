#pragma once
#include <thread>
#include "TutorialIf.h"
//#include <stdio.h>  
//#include <stdlib.h>
#include <cstring>


namespace Services
{

class CppContainers0 : public TutorialIf
{
public:
	CppContainers0(const std::string& name);

	~CppContainers0();

	const std::string& getName();

	void preInit();
	void postInit();


private:
std::string m_name;
};




class MyString
{
public:

  // Rule of three
  // CONSTRUCTOR
  MyString(const char* src);

  ~MyString() {
    std::cout << "Calling Destructor. Address: " << this << " m_data: " << m_data << '\n';
    free(m_data);
  }

  // COPY CONSTRUCTOR
  MyString(const MyString& other);

  // ASSIGNMENT OPERATOR
  MyString& operator=(const MyString& other);

 // Rule of five

  void info();


  friend std::ostream& operator<<(std::ostream& os, const MyString& str);


  class Iterator {
  public:
    Iterator(char* p) : m_ptr(p) {}

    char& operator*() const { return *m_ptr; }

    Iterator& operator++() {
      ++m_ptr;
      return *this;
    }

    bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }

  private:
    char* m_ptr;
  };

  Iterator begin() {
    return Iterator(&m_data[0]);
  }
 
  Iterator end() {
    return Iterator(&m_data[strlen(m_data)]);
  }


private:
  char* m_data{ nullptr };
};


};  // End of Services





