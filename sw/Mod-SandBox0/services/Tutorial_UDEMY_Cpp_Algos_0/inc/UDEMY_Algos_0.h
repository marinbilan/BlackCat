#pragma once
#include <thread>
#include "TutorialIf.h"
//#include <stdio.h>  
//#include <stdlib.h>
#include <cstring>


namespace Services
{

class Algos0 : public TutorialIf
{
public:
	Algos0(const std::string& name);

	~Algos0();

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




  class ProcessMyString {
  public:
    ProcessMyString(int id);

    void passMyString(MyString str);
    void passMyConstString(const MyString str);

    void passMyStringRef(MyString& str);
    void passMyStringConstRef(const MyString& str);

    void passMyStringRValRef(MyString&& str);
    void passMyStringConstRValRef(const MyString&& str);


  private:
        int m_id;
  };




  // Node structure
  template <typename T>
  struct Node {
    T data;
    Node* next;

    Node(T value) : data(value), next(nullptr) {
      std::cout << "2" << '\n';
    }
  };


  // LinkedList class
  template <typename T>
  class LinkedList {
  public:
    LinkedList() : head(nullptr) {
     
    }

    ~LinkedList() {
      clear();
    }

    // Add element to the end of the list
    void append(T value) { // Calling cp constructor
      std::cout << "1" << '\n';
      Node<T>* newNode = new Node<T>(value);
      std::cout << "3" << '\n';
      if (head == nullptr) {
        std::cout << "4" << '\n';
        head = newNode;
      }
      else {
        Node<T>* temp = head;
        while (temp->next != nullptr) {
          temp = temp->next;
        }
        temp->next = newNode;
      }

      std::cout << "5" << '\n';
    }

    // Print the list
    void print() const {
      Node<T>* temp = head;
      while (temp != nullptr) {
        std::cout << temp->data << " -> ";
        temp = temp->next;
      }
      std::cout << "nullptr" << std::endl;
    }

    // Clear the list
    void clear() {
      Node<T>* temp = head;
      while (temp != nullptr) {
        Node<T>* next = temp->next;
        delete temp;
        temp = next;
      }
      head = nullptr;
    }

    T& operator[](size_t index) {
      Node<T>* temp = head;
      size_t currentIndex = 0;

      while (temp != nullptr && currentIndex < index) {
        temp = temp->next;
        currentIndex++;
      }

      if (temp == nullptr) {
        throw std::out_of_range("Index out of range");
      }

      return temp->data;
    }

    class Iterator {
    public:
      Iterator(Node<T>* node) : current(node) {}

      T& operator*() {
        return current->data;
      }

      Iterator& operator++() {
        current = current->next;
        return *this;
      }

      bool operator!=(const Iterator& other) const {
        return current != other.current;
      }

    private:
      Node<T>* current;
    };

    // Begin iterator
    Iterator begin() {
      return Iterator(head);
    }

    // End iterator
    Iterator end() {
      return Iterator(nullptr);
    }
  private:
    Node<T>* head;
  };


} // End of namespace Services






template<class InputIt, class UnaryFunc>
constexpr UnaryFunc my_for_each(InputIt first, InputIt last, UnaryFunc f)
{
  for (; first != last; ++first)
    f(*first);

  return f; // implicit move since C++11
}




/*
class Widget {
  int x;

public:
  Widget(const int x) : x(x) {}

  void init() { std::cout << "Invoce Widget" << '\n'; }

};

template<typename T>
T func(T x) {

  return x;
}

template<typename T>
void invoce(T& obj) {


  obj.init();
}

template<typename T, typename Z>
T retIt(Z& vec) {

  T it = vec.end();

  return it;
}

template<typename T>
void fAdd(T arg)
{
  std::cout << "T: " << arg << '\n';
  arg++;
  std::cout << "T: " << arg << '\n';
}

class Node {
public:
  int data;
  class Node* next;
};
*/