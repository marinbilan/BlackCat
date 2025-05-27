#pragma once

#include "cppMemMngIf.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // For std::accumulate
#include <type_traits>
#include <utility>
#include <memory>


namespace CppMemMgr
{

}



namespace managing_memory_book 
{

// Custom Unique Pointer 
// Preparation
// basic deleter types
template <class T>
struct deleter_pointer_wrapper 
{
	void (*pf)(T*);

	/*
	The odd one is deleter_pointer_wrapper<T>, which wraps a copyable state (a function pointer) but
	otherwise behaves like the other two: when called on T*, it applies some (user-supplied) function to
	that pointer.
	*/
	deleter_pointer_wrapper(void (*pf)(T*)) : pf{ pf } 
	{
	}

	void operator()(T* p) const 
	{ 
		pf(p); 
	}
};


template <class T>
struct default_deleter 
{
	void operator()(T* p) const 
	{ 
		delete p; 
	}
};


template <class T>
struct default_deleter<T[]> 
{
	void operator()(T* p) const 
	{ 
		delete[] p; 
	}
};



// pt 1
// ...
template <class T>
struct is_deleter_function_candidate : std::false_type {};

template <class T>
struct is_deleter_function_candidate<void (*)(T*)> : std::true_type {};

template <class T>
constexpr auto is_deleter_function_candidate_v = is_deleter_function_candidate<T>::value;
// ...


// Pt 2

// unique_ptr general template (not array)
template <class T, class D = default_deleter<T>>
class unique_ptr : std::conditional_t <is_deleter_function_candidate_v<D>, deleter_pointer_wrapper<T>, D> 
{
using deleter_type = std::conditional_t <is_deleter_function_candidate_v<D>, deleter_pointer_wrapper<T>, D>;
T* p = nullptr;

public:
	unique_ptr() = default;
	unique_ptr(T* p) : p{ p } 
	{
	}

	unique_ptr(T* p, void (*pf)(T*)) : deleter_type{ pf }, p{ p } 
	{
	}

	~unique_ptr() 
	{
		(*static_cast<deleter_type*>(this))(p);
	}

	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;

	void swap(unique_ptr &other) noexcept 
	{
		using std::swap;
		swap(p, other.p);
	}

	unique_ptr(unique_ptr &&other) noexcept : p{ std::exchange(other.p, nullptr) } 
	{
	}

	unique_ptr& operator=(unique_ptr &&other) noexcept 
	{
		unique_ptr{ std::move(other) }.swap(*this);
		return *this;
	}

/*
	// Check if need to implement those methods for non-array (this) case
	// Add part of the code
	bool empty() const noexcept 
	{ 
		return !p; 
	}
	
	operator bool() const noexcept 
	{ 
		return !empty(); 
	}
	
	bool operator==(const unique_ptr &other)
	
	const noexcept 
	{
		return p == other.p;
	}
	
	// inferred from operator==() since C++20
	bool operator!=(const unique_ptr &other)
	const noexcept 
	{
		return !(*this == other);
	}

	T *get() noexcept 
	{ 
		return p; 
	}
	
	const T *get() const noexcept 
	{ 
		return p; 
	}
*/


	// POINTER LIKE FUNCTIONS
	/*
	The pointer-like functions are different for the scalar case and the array case. For pointer-to-scalar, we
	will want to implement operator*() and operator->()
	*/
	T& operator*() noexcept 
	{ 
		return *p; 
	}
	
	const T& operator*() const noexcept 
	{ 
		return *p; 
	}

	T* operator->() noexcept 
	{ 
		return p; 
	}

	const T* operator->() const noexcept 
	{ 
		return p; 
	}

	// the following two are only for the non-array case
	template <class U> decltype(auto) operator*(this U && self) noexcept 
	{
		return *(self.p);
	}

	template <class U> decltype(auto) operator->(this U && self) noexcept 
	{
		return self.p;
	}
};
// End of non array unique ptr implementation



// Pt 3
// ---- unique_ptr specialization for arrays ----
template <class T, class D>
class unique_ptr<T[], D> : std::conditional_t <is_deleter_function_candidate_v<D>, deleter_pointer_wrapper<T>, D> 
{
using deleter_type = std::conditional_t <is_deleter_function_candidate_v<D>, deleter_pointer_wrapper<T>, D>;
T* p = nullptr;

public:
	unique_ptr() = default;
	unique_ptr(T* p) : p{ p } 
	{
	}

	unique_ptr(T* p, void (*pf)(T*)) : deleter_type{ pf }, p{ p } 
	{
	}

	~unique_ptr() 
	{
		(*static_cast<deleter_type*>(this))(p);
	}

	// Added part of code
	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;

	void swap(unique_ptr &other) noexcept 
	{
		using std::swap;
		swap(p, other.p);
	}

	unique_ptr(unique_ptr &&other) noexcept : p{ std::exchange(other.p, nullptr) } 
	{
	}

	unique_ptr& operator=(unique_ptr &&other) noexcept 
	{
		unique_ptr{ std::move(other) }.swap(*this);
		return *this;
	}

	// Add part of the code
	bool empty() const noexcept 
	{ 
		return !p; 
	}
	
	operator bool() const noexcept 
	{ 
		return !empty(); 
	}
	
	bool operator==(const unique_ptr &other)
	
	const noexcept 
	{
		return p == other.p;
	}
	
	// inferred from operator==() since C++20
	bool operator!=(const unique_ptr &other)
	const noexcept 
	{
		return !(*this == other);
	}

	T *get() noexcept 
	{ 
		return p; 
	}
	
	const T *get() const noexcept 
	{ 
		return p; 
	}

	// POINTER LIKE FUNCTIONS
	/*
	For pointer-to-array (the unique_ptr<T[]> specialization), we will want to implement operator[],
	which will make more sense than either operator*() or operator->()
	*/
	T& operator[](std::size_t n) noexcept 
	{
		return p[n];
	}

	const T& operator[](std::size_t n) const noexcept 
	{
		return p[n];
	}

	// The following is for both the array and non-array cases
	template <class U> decltype(auto) get(this U && self) noexcept 
	{
		return self.p;
	}

	// The following is only for the array case
	template <class U> decltype(auto) operator[](this U && self, std::size_t n) noexcept 
	{
		return self.p[n];
	}
}; // End of array unique ptr implementation


	// SPECIAL MEMBER FUNCTIONs
	/*
	We code for the special member functions will be the same for both the scalar and the array forms of
	unique_ptr.

	- We want the type to be non-copyable
	- We want move operations to implement the transfer of ownership
	*/



}  // End of namespace






// ---- 141/381
namespace Polymorphic
{

struct X { int n; };

struct B 
{
int n;
	B(int n) : n{ n } {}
	virtual ~B() = default;
};


struct D0 : B 
{
	D0(int n) : B{ n } { /* ... */ }
	// ...
};

struct D1 : B 
{
	D1(int n) : B{ n } { /* ... */ }
	// ...
};


// Always create Test Class - no free functions
/*
B virtual ~B()
|    |
D0   D1
*/
class TestClass 
{
public:
	TestClass(int id) : m_id(id) {}

	Polymorphic::X* duplicate(Polymorphic::X* p) 
	{
		// X* p = x;
		std::cout << __FUNCTION__ << " p:" << p << '\n';
		return new Polymorphic::X{ *p }; // Ok

		// p is deleted at the end of scope
	}

	// Bad: Duplicate derived via base (ptr)
	Polymorphic::B* duplicate(Polymorphic::B *p) 
	{
		return new Polymorphic::B{ *p }; // Bad idea!
	}

private:
int m_id;
};

}  // End of namespace Polymorphic





































// Note that for parallel accumulation, use std::reduce

// SUM
/*
double average_score(const std::vector<int>& scores)
{

	return std::accumulate(
		scores.cbegin(), scores.cend(),  // Sums all the scores in collection
		0  								 // Initial value for sum
		) / (double)scores.size();       // Calculate the average score
}


// MULTIPLY
double scores_product(const std::vector<int>& scores)
{

	return std::accumulate(
		scores.cbegin(), scores.cend(),  // Calc product of elements in range
		1,  						     // Initial value for multiplication
		std::multiplies<int>()			 // Multiply all values in range
		);       
}
*/

/*
The std::accumulate algorithm is an implementation of a general concept called FOLDING (check)
(or reduction). This is a higher-order function that abstracts the process of iterating over
recursive structures such as vectors, lists, trees, and so on and lets you gradually build the
result you need.
*/

/*
// FOLDING
int f(int previous_count, char c)
{
	return (c != '\n') ? previous_count : previous_count + 1;
}


int count_lines(const std::string& s)
{
	return std::accumulate(
		s.cbegin(), s.cend(),  // Range (accumulate foreach item in collection)
		0,                     // Start value and return value (check this)
		f                      // Predicate
);
}


// Trim from left side

std::string trim_left(std::string s)
{
	s.erase(s.begin(),
		    std::find_if(s.begin(), s.end(), is_not_space));

	return s;
}

// Trim from right side
std::string trim_right(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), is_not_space).base(),
	s.end());

	return s;
}

// Combination gives trim from both sides
std::string trim(std::string s)
{
	return trim_left(trim_right(std::move(s)));
}




// Partition - Move elements to the left side of list
// Moving selected items in UI editor

std::partition(
	people.begin(), people.end(), is_female
);

*/

// Check std::stable_partition


// ---- FILTERING AND TRANSFORMING ----

/*
[ People ]
|
| Filter (females) (std::filter)
|
[ [ Females ]
|
| getName ((std::transfor))
| Transform (Iterate via Females and populate another collection with names)


Filtering (Two ways)
1st - erase + remove_if
2nd - copy_if(begin, end, std::back_inserter(females), is_female)

Transform
*/

// copy_if
// Check ChatGPT: std::any_of, std::all_of, and std::find_if
// std::stable_partition, std::partition, std::stable_sort, std::sort




// ======== Ch 3: FUNCTION OBJECTS ========
// 46 / 321

/*
// Ex 1
template <typename Object, typename Function>
decltype(auto) call_on_object(Object&& object, Function function)
{
	return function(std::forward<Object>(object));
}


// Ex 2
// Check if person is older than 42
bool older_than_42(const person_t& person)
{
	return person.age > 42;
}

std::count_if(persons.cbegin(), persons.cend(), older_than_42);


// Ex 3
// Better create custom object with callable
class older_than {
public:
older_than(int limit) : m_limit(limit)
{
}

bool operator()(const person_t& person) const
{
	return person.age() > m_limit;
}

private:
int m_limit;
};


// count_if will call operator() on each object in persons collection
std::count_if(persons.cbegin(), persons.cend(), older_than(42));
std::count_if(persons.cbegin(), persons.cend(), older_than(16));


// 3.1.4 Creating generic function objects


// Ex 4
// We can try with class template aproach, but this has some issues ... check

template <typename T>
class older_than {
	...

bool operator()(const T& object) const
{
	return object.age()> m_limit;
}

	...


// Ex 5
class older_than {
public:
older_than(int limit) : m_limit(limit)
{
}

template <typename T>
bool operator()(T&& object) const
{
	return std::forward<T>(object).age()> m_limit;
}

private:
int m_limit;
};


older_than predicate(5);

std::count_if(persons.cbegin(), persons.cend(), predicate);
std::count_if(cars.cbegin(), cars.cend(), predicate);
std::count_if(projects.cbegin(), projects.cend(), predicate);

// This is ok, but syntax is verbose so lambra is better approach


// 3.2 LAMBDAS AND CLOSURES


// You can use a lambda to achieve the same effect while keeping the code
// localized and not polluting the program namespace:

std::copy_if(people.cbegin(), people.cend(), std::back_inserter(females),
	[](const person_t& person) {
		return person.gender() == person_t::female;
	}
);


// std::sort(numbers.begin(), numbers.end(), std::greater<int>());
// Check all operators

// Check Boost Phoenix lib
// std::sort(numbers.begin(), numbers.end(), arg1 > arg2);  // Phoenix example
*/



/*
// CHAPTER 4: CREATING NEW FUNCTIONS FROM OLD ONES



*/