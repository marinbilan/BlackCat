#pragma once
#include "ServiceIf.h"

#include <vector>


namespace Services
{

class ServiceX : public ServiceIf
{
public:
	ServiceX(const std::string& dbPath, const std::string& name);

	~ServiceX();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};


// Modern C++ 11 (Bo Qian) - YouTube
class Widget
{
public:
	Widget(int id) : m_id(id)
	{
		std::cout << "Widget created!" << '\n';
	}

private:
int m_id;
};



// YT (Bo Qian): Learn C++ 11 in 20 Minutes - Part I
class Dog
{
public:
	Dog()
	{
		std::cout << "Default constructor" << '\n';
	}

	// Delegating constructor
	// Note - first default constructor will be called Dog()
	Dog(int a) : Dog() 
	{
		std::cout << "Calling constructor with argument a" << '\n';
		// Call some other member function
	}
};

// Override for virtual function
// To avoid inadvertently create new function in derived classes

// C++ 03
/*
class Dog1
{
	virtual void A(int);
	virtual void B() const;
}

class yellowdog : public Dog1
{
	// Accidentally
	// virtual void A(float);  // Created a new function
	// virtual void B();  // Created a new function

	// By adding override - error will occure (wrong arg type and no const)
	virtual void A(float) override;  // Compile error (wrong argument type)
	virtual void B() override;  // Compile error (no const)
}
*/


// [ITEM 11] - Compiler will generate defaut constructor
class Dog2
{
public:
	Dog2(int age) {}
	Dog2() = default;  // C++ 11 will force for default constructor
};


// [ITEM 12] - delete
/*
If we do not want implicit conversion, use keyword delete
*/
class Dog3
{
public:
	Dog3(int age) {}
	Dog3(double) = delete;  // Dog3(3.14) - No implicit conversion to 3 allowed
	Dog3& operator=(const Dog3) = delete;  // No assign operator allowed

private:
};


// [ITEM 13] - constexpr
/*
// In C++ 03 this will not work because compiler does not know that A() return constant
int arr[6];  // OK
int A() { return 3; }
int arr[A() + 3];  // compile error

// C++ 11
constexpr int A() { return 3; }  // Forces computation at compile time
int arr[A() + 3];  // Create an array of size 6

// Write faster programs with constexpr
constexpr int cubed(int x) { return x * x * x; }

int y = cubed(1789);  // Computed at compile time
*/



// [ITEM 15] - Lambda function
template<typename func>
void filter(func f, std::vector<int> arr)
{
	// Perform function f on each item of array
	for(auto i : arr)
	{
		std::cout << "Perform function on item: " << i << '\n';
		// If function returns true - print out the vector item
		if(f(i))
		{
			std::cout << "Item: " << i << '\n';
		}
	}
}



// C++ 11: Rvalue Reference -- Move Semantics
// Because of multiple definition issue, methods defines 
// needs to be in cpp
/*
void printInt(int& i) 
{
	std::cout << "lvalue reference: " << i << '\n';
}

void printInt(int&& i) 
{
	std::cout << "rvalue reference: " << i << '\n';
}
*/

// This will not work
// void printInt(int i) {}

// Temp class for cp / move constructor experiment
class boVector
{
public:

	boVector(int n) : size(n)
	{
		std::cout << "Calling boVector constructor" << '\n';
		arr_ = new double[size];

		for(int i = 0; i < size; i++)
		{
			arr_[i] = i;
		}
	}

	boVector(const boVector& rhs)  // Copy constructor
	{
		std::cout << "Calling boVector copy constructor" << '\n';
		
		size = rhs.size;
		arr_ = new double[size];

		for(int i = 0; i < size; i++)
		{
			arr_[i] = rhs.arr_[i];
		}
	}

	// Move constructor will take rvalue ref as param
	// no const because in move constructor we change value of rvalue param
	boVector(boVector&& rhs)  // Move constructor
	{
		std::cout << "Calling boVector move constructor" << '\n';
		
		size = rhs.size;
		// arr_ = new double[size];
		arr_ = rhs.arr_;  // Just use rhs array
		/*
		for(int i = 0; i < size; i++)
		{
			arr_[i] = rhs.arr_[i];
		}
		*/
		// Also clear rhs array
		rhs.arr_ = nullptr;
	}

	~boVector() { delete arr_; }

private:
int m_id;
int size;
double* arr_;  // A big array
};


// Test rvalue boVector
void foo(boVector v);


// Creates boVector
// If no implemented move constructor - compiler will call default implicitly
boVector createBoVector();  



// ---- PERFECT FORWARDING ----
// boVector has move constructor and copy constructor
// void foo(boVector arg);  // Already defined above

/*
template<typename T>
void relay(T arg)
{
    // foo(arg);
}
*/


// Solution:
template<typename T>
void relay(T&& arg)
{
    foo(std::forward<T>(arg));
}


/*
Reference Collapsing Rules for Type Deduction (C++ 11) - Summary
 f(T  param)

// lvalue reference (universal)
1. T& &   ==> T&
2. T& &&  ==> T&

// rvalue reference (universal)
3. T&& &  ==> T&
4. T&& && ==> T&&
*/


// Remove reference
/*
template<class T>
struct remove_reference;  // If removes reference on type T

// T is int&
remove_reference<int&>::type i;  // int i;

// T is int
remove_reference<int>::type i;  // int i;
*/


} // End of namespace Services