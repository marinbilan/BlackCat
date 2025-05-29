#pragma once

#include "cppMemMngIf.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // For std::accumulate
#include <type_traits>
#include <utility>
#include <memory>
#include <cstdlib>
#include <new>


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
	B(int n) : n{ n } 
	{
		std::cout << "Calling B(int n) constructor. this: " << this << '\n';
	}
	virtual ~B() = default;
};


struct D0 : B 
{
	D0(int n) : B{ n } 
	{ 
		std::cout << "Calling D0(int n) constructor. this: " << this << '\n';
	}
	// ...
};

struct D1 : B 
{
	D1(int n) : B{ n } { /* ... */ }
	// ...
};


// Always create Test Class - no free functions
/*

X - Free object

B virtual ~B()
|    |
D0   D1
*/
class TestClass 
{
public:
	TestClass(int id) : m_id(id) {}

	/*
	Can safely create and object of the X type since x has no virtual member function
	*/
	Polymorphic::X* duplicate(Polymorphic::X* p) 
	{
		// X* p = x;
		std::cout << __FUNCTION__ << " p:" << p << '\n';
		return new Polymorphic::X{ *p }; // Ok

		// p is deleted at the end of scope
	}

	// Bad: Duplicate derived via base (ptr) - slicing
	Polymorphic::B* duplicate(Polymorphic::B *p) 
	{
		/*
		Calling new B{ *p }; only constructs the base part, slicing away any state
		from the pointed-to object and resulting in a probably incorrect program.
		This calls copy constructor. 
		In safe case implement virtual clone() method. 
		*/
		return new Polymorphic::B{ *p }; // Bad idea!
	}

private:
int m_id;
};

}  // End of namespace Polymorphic



// ---- 142/381
namespace Cloneable 
{

// ... type cloneable
struct X { int n; };

struct B { // every B is cloneable
int n;
B(int n) : n{ n } {}
virtual ~B() = default;
B * clone() { return new B(*this); };
protected: // cloneable types are meaningfully copied

// in a subjective manner
B(const B&) = default;
};

struct D0 : B {
D0(int n) : B{ n } { /* ... */ }
D0* clone() const  { return new D0{ *this }; }

// ...
};
struct D1 : B {
D1(int n) : B{ n } { /* ... */ }
D1* clone() const  { return new D1{ *this }; }
// ...
};



}




// ---- C++ Memory Management: 143/381
namespace Cloneable_0
{

// Base class with virtual clone
struct Cloneable 
{
    virtual Cloneable* clone() const = 0;
    virtual ~Cloneable() = default;
};


// Copier: uses copy constructor
struct Copier 
{
    template <class T>
    T* operator()(const T* p) const 
	{
		std::cout << __FUNCTION__ << " Copier p: " << p << '\n';
        return new T{ *p };
    }
};


// Cloner: uses virtual clone
struct Cloner 
{
    template <class T>
    T* operator()(const T* p) const 
	{
		std::cout << __FUNCTION__ << " Cloner p: " << p << '\n';
        return static_cast<T*>(p->clone());
    }
};


template <class T,
          class Dup = std::conditional_t<
              std::is_base_of_v<Cloneable, T>,
              Cloner,
              Copier>>
class dup_ptr 
{
    T* p{};

public:
    dup_ptr() = default;

    dup_ptr(const T* raw_ptr) : p(raw_ptr ? Dup{}(raw_ptr) : nullptr) 
	{
		std::cout << __FUNCTION__ << " Constructor ... raw_ptr: " << raw_ptr << " p: " << p << '\n';
	}

    dup_ptr(const dup_ptr& other) : p(other.p ? Dup{}(other.p) : nullptr) 
	{
		std::cout << __FUNCTION__ << " Copy Constructor ... other: " << other << " p: " << p << '\n';
	}

    dup_ptr& operator=(const dup_ptr& other) 
	{
        if (this != &other) 
		{
            delete p;
            p = other.p ? Dup{}(other.p) : nullptr;
        }
        return *this;
    }

    ~dup_ptr() 
	{
        delete p;
    }

    T* get() const { return p; }
    bool empty() const { return p == nullptr; }
};



struct A 
{
    int x = 5;
};


struct B : public Cloneable 
{
    int y = 10;

	// Must be implemented (inh from inteface) in order to call Clonner
    Cloneable* clone() const override 
	{
		std::cout << __FUNCTION__ << " B" << '\n';
        return new B(*this);
    }
};





} // End of namespace Cloneable_0



// ---- C++ Memory Management: 143/381
namespace Cloneable_1
{

// Additionally ... another way

// types Cloner and Copier (see above)
template <class, class = void>
struct has_clone : std::false_type 
{ 

};

template <class T>
struct has_clone <T, std::void_t<decltype(std::declval<const T*>()->clone())>> : std::true_type 
{ 

};

template <class T>
constexpr bool has_clone_v = has_clone<T>::value;
template <class T, class Dup = std::conditional_t<has_clone_v<T>, Cloneable_0::Cloner, Cloneable_0::Copier>> class dup_ptr 
{
	T *p{};

public:
	// ...
	dup_ptr(const dup_ptr &other) : p{ other.empty()? nullptr : Dup{}(other.p) } 
	{
	}
	// ...
};

} // End of namespace Cloneable_1


// C++ Memory Management: 145/381 - Detection through concepts ...

/*
So we have standard smart pointers, such as unique_ptr<T> (single ownership) and shared_ptr<T>
(shared ownership), and we can write our own for more exotic situations (we examined dup_ptr<T>
where we have single ownership but duplication of the pointee when the pointer is duplicated). Are
there other common semantics we might want to ensconce in the type system of our program?

1] A non_null_ptr type - non_null_ptr<T>

*/
namespace Cloneable_2
{
	class invalid_pointer {};


	template <class T>
	class non_null_ptr 
	{
	T *p;

	public:
		explicit non_null_ptr(T *p) : p{ p } 
		{
			if (!p) throw invalid_pointer{};
		}

		T* get() const 
		{ 
			return p; 
		}

		constexpr operator bool() const noexcept 
		{
			return true;
		}

		T& operator*() const { return *p; }
		T* operator->() const { return p; }
	};

	// definition of the non_null_ptr type (omitted)
	struct X { int n; };

	/*
	Free function must be inline. When multiple .cpp files include this header, 
	the linker sees multiple definitions of the same function, which violates 
	the One Definition Rule (ODR).
	*/
	inline int extract_value(const non_null_ptr<X>& p) 
	{
		// In order this to work, operator -> need to be implemented in non_null_ptr
		return p->n; // no need for validation as it stems from the type system itself
	}

} // End of namespace Cloneable_2


// C++ Memory Management: 148/381 - An observer_ptr type ...



// C++ Memory Management: 151/381
// ======== Part 3: Taking Control (of Memory ManagementMechanisms) ========
// Overloading Memory Allocation Operators - Brief overview of the C language allocation functions

// Use the C++ version of these functions (from <cstdlib> instead of <stdlib.h>)

/*
void* malloc(size_t n);
void free(void* p);

Memory allocation functions come in groups of four: 
	operator new()
	operator new[]() 
	operator delete()
	operator delete[]().


The signatures we want to overload are the following:
	void *operator new(std::size_t);
	void *operator new[](std::size_t);
	void operator delete(void *) noexcept;
	void operator delete[](void *) noexcept;

	// since C++14
	void operator delete(void *, std::size_t) noexcept;
	void operator delete[](void *, std::size_t) noexcept;

	// Note that we need to overload all functions

That’s the thing: new does not create objects. What new does is find the location where an object will be
constructed. It’s the constructor that turns the raw memory found by new into an object.

1] allocate enough space to put an X object
2] construct an X object at that location (apply args on buffer)

Call operator new[](N*sizeof(X)) to find a place to put the array that will
be constructed, then call X::X() on each of the N blocks of size sizeof(X) in that array.
*/

namespace Pt3_0
{
	// ...

}


// Can not be defined inside namespace
// See later for updated version of this functions
/*
inline void *operator new(std::size_t n) 
{
	std::cout << "operator new(" << n << ")\n";
	auto p = std::malloc(n);

	if(!p) throw std::bad_alloc{};
	return p;
}

inline void operator delete(void *p) noexcept 
{
	std::cout << "operator delete(...)\n";
	std::free(p);
}

inline void operator delete(void *p, std::size_t n) noexcept 
{
	std::cout << "operator delete(..., " << n << ")\n";
	::operator delete(p);
}

inline void *operator new[](std::size_t n) 
{
	std::cout << "operator new[](" << n << ")\n";
	auto p = std::malloc(n);
	if(!p) throw std::bad_alloc{};
	return p;
}

inline void operator delete[](void *p) noexcept 
{
	std::cout << "operator delete[](...)\n";
	std::free(p);
}

inline void operator delete[](void *p, std::size_t n) noexcept 
{
	std::cout << "operator delete[](..., " << n << ")\n";
	::operator delete[](p);
}
*/


/*
	Check also: Non-throwing versions of the allocation operators (return nullptr instead of exception)
	can be used in embedded systems
	Type std::nothrow_t is what is called a tag type:

	X *p = new (nothrow) X{ ... args ... };
	if(p) {
		// ... use *p
		// note: this is not the nothrow version of delete
		delete p; // would be Ok even if !p
	}

	
	Remember that memory allocation through operator new() is a two-step operation:
		Find the location to place the object, then construct the object at that location. 
		Thus, even if operator new() does not throw, we do not know whether the constructor 
		that will be called will throw.

		For that reason, it falls on the C++ runtime to perform the deallocation if an exception 
		is thrown by the constructor, and this is true for all versions of operator new(), not 
		just the nothrow ones.


		// step 1, try to perform the allocation for some T object
			p = operator new(n, ... maybe additional arguments ...)

		// the following line is only for a nothrow new
			if(!p) return p
		
			try {
		// step 2, construct the object at address p apply the constructor of T at address p might throw
			} catch(...) { // construction threw an exception deallocate p 
		// this is what concerns us here re-throw the exception, whatever it was
			}

			return p // p points to a fully constructed object
		// only after this point does client code see p
*/


// Updated naive implementation
// Uncomment to test (Impact all tests)
/*
inline void* operator new(std::size_t n, const std::nothrow_t&) noexcept 
{
	return std::malloc(n);
}

inline void* operator new(std::size_t n) 
{
	auto p = operator new(n, std::nothrow);
	if (!p) throw std::bad_alloc{};

	return p;
}

inline void operator delete(void* p, const std::nothrow_t&) noexcept 
{
	std::free(p);
}

inline void operator delete(void* p) noexcept 
{
	operator delete(p, std::nothrow);
}

inline void operator delete(void* p, std::size_t) noexcept 
{
	operator delete (p, std::nothrow);
}

inline void* operator new[](std::size_t n, const std::nothrow_t&) noexcept 
{
	return std::malloc(n);
}

inline void* operator new[](std::size_t n) 
{
	auto p = operator new[](n, std::nothrow);
	if (!p) throw std::bad_alloc{};

	return p;
}

inline void operator delete[](void* p, const std::nothrow_t&) noexcept 
{
	std::free(p);
}

inline void operator delete[](void* p) noexcept 
{
	operator delete[](p, std::nothrow);
}

inline void operator delete[](void* p, std::size_t) noexcept 
{
	operator delete[](p, std::nothrow);
}
*/

// C++ Memory Management: 161/381
// The most important operator new: placement new

/*
	// note: these exist, you can use them but you cannot replace them
	// mostly known as placement new by the programming community.

	void *operator new(std::size_t, void *p) { return p; }
	void *operator new[](std::size_t, void *p) { return p; }
	void operator delete(void*, void*) noexcept { }
	void operator delete[](void*, void*) noexcept { }


“What new does is find the location where an object will be constructed.” 
This does not necessarily mean that new will allocate memory, and indeed, 
placement new does not allocate; it simply yields back the address it has 
been given as argument. is allows us to place an object wherever we want 
in memory... as long as we have the right to write the memory at that location.


Note that string will allocate memory (but string_view will not) - Do experiment
auto string_length(const char *p) 
{
	return std::string{ p }.size(); // augh! But it works...
}


*/


// PREREQUISITEs

/*
std::string is a class that internally stores pointers and size fields, 
and possibly supports small string optimization (SSO)

struct string {
    char* ptr;        // pointer to the buffer (or inline buffer) A pointer = 8 bytes
    size_t size;      // current length                           A size/length = 8 bytes
    size_t capacity;  // allocated capacity                       Capacity = 8 bytes
    // maybe an inlined buffer for SSO (e.g., 15 chars + null)
};

So, total size = 3 × 8 bytes = 24 bytes, plus padding or small buffer = ~32 bytes.

// Create buffer
	alignas(std::string) char buf[sizeof(std::string)];
// Emplace string inside buffer (string internally points to memory where data is stored)
// The actual character data — "Hello" — is typically stored on the heap, and the std::string object points to it.
	std::string* s = new (buf) std::string("Hello");
*/


inline auto string_length(const char *p) {
using std::string;
	// A) make a local buffer of the right size and alignment for a string object
	/*
	char buf[sizeof(std::string)]
		Creates a raw buffer of 32 bytes (assuming sizeof(std::string) is 32).
		This memory is uninitialized, but large enough to hold a std::string object.
	*/
	std::cout << "sizeof(string): " << sizeof(string) << '\n';
	// We do not need to delete buf (it is on stack)
	alignas(string) char buf[sizeof(string)];  // sizeof(string) = 32

	// B) "paint" a string object in that buffer (note: that object might allocate its
	// own data externally, but that's not our concern here)

	/*
	new (buf) std::string(p);
		This is placement new: it constructs a std::string object in the buffer.
		It uses p (a const char*) to initialize the string.

	std::string{p} constructs a new string from that C-string
	The constructor copies the characters from p into its own internal buffer
	*/
	string* s = new (static_cast<void*>(buf)) string{ p };

	std::cout << "buf[0]: " << buf[0] << '\n';  // Not underlying char "M","y".. but object string first byte!

	// C) use that object to compute the size
	const auto sz = s->size();
	std::cout << "size of string (internal value) : " << sz << '\n';
	
	// D) destroy the object without releasing the memory for the buffer 
	// (it's not dynamically allocated, it's just local storage)
	s->~string(); // yes, you can do this

	return sz;
}


// A note on make_shared<T>(args...)


// C++ Memory Management: 164/381
// Member versions of the allocation operators


// Alignment-aware versions of the allocation operators
// struct alignas(16) Float4 { float vals[4]; };

// Destroying delete - C++20 brings a novel and highly specialized feature called destroying delete.



// C++ Memory Management: 169/381
// -------- 8 Writing a Naive Leak Detector --------



















































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