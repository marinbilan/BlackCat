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
#include <memory>
#include <print>
#include <atomic>
#include <functional>
#include <utility>
#include <cstddef>
#include <initializer_list>
#include <iterator>


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






// -------- Chapter: 11 Deferred Reclamation -------- 
// 231/381


// 01 - GC finalize all at end
// use-case : destroy all GcNodes when GC object is destroyed, calling the proper dtors
namespace Chapter11_01  // no templates - namespace ok
{
	//
	class GC {

		class GcRoot {
			void *p;

		public:
			auto get() const noexcept { return p; }
			GcRoot(void *p) : p{ p } {
			}
			GcRoot(const GcRoot &) = delete;
			GcRoot& operator=(const GcRoot &) = delete;
			virtual void destroy(void *) const noexcept = 0;
			virtual ~GcRoot() = default;
		};

		//   GcRoot
		// |        |
		// GcNode   GcNode(Args...)

		template <class T> class GcNode : public GcRoot {

			void destroy(void* q) const noexcept override {
				delete static_cast<T*>(q);
		}

		public:
		template <class ... Args>
		GcNode(Args &&... args) : GcRoot(new T(std::forward<Args>(args)...)) {}

		~GcNode() {
			destroy(get());
		}
		};

		std::vector<std::unique_ptr<GcRoot>> roots;

		GC() = default;

		static auto &get() {
			static GC gc;
			return gc;
		}

		template <class T, class ... Args>
		T *add_root(Args &&... args) {
			std::cout << __FUNCTION__ << " [4] " << '\n';

			return static_cast<T*>(roots.emplace_back(
				std::make_unique<GcNode<T>>(std::forward<Args>(args)...))->get());
		}

		template <class T, class ... Args>
		friend T* gcnew(Args&&...);

		public:
			GC(const GC &) = delete;
			GC& operator=(const GC &) = delete;
	};
	// 


	template <class T, class ... Args>
	T *gcnew(Args &&...args) {

		std::cout << __FUNCTION__ << " [3] " << '\n';
		return GC::get().add_root<T>(std::forward<Args>(args)...);
	}


	struct NamedThing {
		const char *name;

		NamedThing(const char *name) : name{ name } {
			std::print("{} ctor\n", name);
		}

		~NamedThing() {
			std::print("{} dtor\n", name);
		}
	};

	inline void g() {
		std::cout << __FUNCTION__ << " [2] " << '\n';
		[[maybe_unused]] auto p = gcnew<NamedThing>("hi");
		[[maybe_unused]] auto q = gcnew<NamedThing>("there");
	}

	inline auto h() {
		struct X {
			int m() const { return 123; }
		};

		return gcnew<X>();
	}

	inline auto f() {
		std::cout << __FUNCTION__ << " [1] " << '\n';
		g();
		return h();
	}	

} // End of namespace Chapter11_01



// 02 - GC finalize all at end thread safe
// 238/381
namespace Chapter11_02
{
	class GC {

	std::mutex m;

	class GcRoot {
		void *p;
	public:
		auto get() const noexcept { return p; }
		GcRoot(void *p) : p{ p } {
		}
		GcRoot(const GcRoot &) = delete;
		GcRoot& operator=(const GcRoot &) = delete;
		virtual void destroy(void *) const noexcept = 0;
		virtual ~GcRoot() = default;
	};

	template <class T> class GcNode : public GcRoot {
		void destroy(void* q) const noexcept override {
			delete static_cast<T*>(q);
		}
	public:
		template <class ... Args>
			GcNode(Args &&... args) : GcRoot(new T(std::forward<Args>(args)...)) {
			}
		~GcNode() {
			destroy(get());
		}
	};

	std::vector<std::unique_ptr<GcRoot>> roots;
	GC() = default;
	static auto &get() {
		static GC gc;
		return gc;
	}

	template <class T, class ... Args>
		T *add_root(Args &&... args) {
			std::lock_guard _ { m };
			return static_cast<T*>(roots.emplace_back(
				std::make_unique<GcNode<T>>(std::forward<Args>(args)...)
			)->get());
		}
	template <class T, class ... Args>
		friend T* gcnew(Args&&...);
	public:
	GC(const GC &) = delete;
	GC& operator=(const GC &) = delete;
	};

	template <class T, class ... Args>
	T *gcnew(Args &&...args) {
		return GC::get().add_root<T>(std::forward<Args>(args)...);
	}

	struct NamedThing {
	const char *name;
	NamedThing(const char *name) : name{ name } {
		std::print("{} ctor\n", name);
	}
	~NamedThing() {
		std::print("{} dtor\n", name);
	}
	};

	inline void g() {
	[[maybe_unused]] auto p = gcnew<NamedThing>("hi");
	[[maybe_unused]] auto q = gcnew<NamedThing>("there");
	}

	inline auto h() {
	struct X {
		int m() const { return 123; }
	};
	return gcnew<X>();
	}

	inline auto f() {
		g();
		return h();
	}

}


// 03 - GC finalize with scoped collect
// use-case : destroy all GcNodes when collecting, calling the proper dtors
// using counting_ptr to know which objects to collect

//namespace Chapter11_2 // - Check issues with namespace in this case
// 243/381
//{
# if 1
template <class T>
   class counting_ptr {
      using count_type = std::atomic<int>;
      T *p;
      count_type *count;
      std::function<void()> mark;
   public:
      template <class M>
         constexpr counting_ptr(T *p, M mark) try : p{ p }, mark{ mark } {
            count = new count_type{ 1 };
         } catch(...) {
            delete p;
            throw;
         }
      T& operator*() noexcept {
         return *p;
      }
      const T& operator*() const noexcept {
         return *p;
      }
      T* operator->() noexcept {
         return p;
      }
      const T* operator->() const noexcept {
         return p;
      }
      constexpr bool operator==(const counting_ptr &other) const {
         return p == other.p;
      }
      constexpr bool operator!=(const counting_ptr &other) const {
         return !(*this == other);
      }
      template <class U>
         constexpr bool operator==(const counting_ptr<U> &other) const {
            return p == &*other;
         }
      template <class U>
         constexpr bool operator!=(const counting_ptr<U> &other) const {
            return !(*this == other);
         }
      template <class U>
         constexpr bool operator==(const U *q) const {
            return p == q;
         }
      template <class U>
         constexpr bool operator!=(const U *q) const {
            return !(*this == q);
         }
      void swap(counting_ptr &other) {
         using std::swap;
         swap(p, other.p);
         swap(count, other.count);
         swap(mark, other.mark);
      }
      constexpr operator bool() const noexcept {
         return p != nullptr;
      }
      counting_ptr(counting_ptr &&other) noexcept
         : p{ std::exchange(other.p, nullptr) },
           count{ std::exchange(other.count, nullptr) },
           mark{ other.mark } {
      }
      counting_ptr &operator=(counting_ptr &&other) noexcept {
         counting_ptr{ std::move(other) }.swap(*this);
         return *this;
      }
      counting_ptr(const counting_ptr &other)
         : p{ other.p }, count{ other.count }, mark{ other.mark } {
         if (count) ++(*count);
      }
      counting_ptr &operator=(const counting_ptr &other) {
         counting_ptr{ other }.swap(*this);
         return *this;
      }
      ~counting_ptr() {
         if (count) {
            if ((*count)-- == 1) {
               // delete p;
               mark();
               delete count;
            }
         }
      }
   };
namespace std {
   template <class T, class M>
      void swap(counting_ptr<T> &a, counting_ptr<T> &b) {
         a.swap(b);
      }
}

class GC {
   class GcRoot {
      void *p;
   public:
      auto get() const noexcept { return p; }
      GcRoot(void *p) : p{ p } {
      }
      GcRoot(const GcRoot&) = delete;
      GcRoot& operator=(const GcRoot&) = delete;
      virtual void destroy(void*) const noexcept = 0;
      virtual ~GcRoot() = default;
   };
   template <class T> class GcNode : public GcRoot {
      void destroy(void *q) const noexcept override {
         delete static_cast<T*>(q);
      }
   public:
      template <class ... Args>
         GcNode(Args &&... args) : GcRoot(new T(::std::forward<Args>(args)...)) {
         }
      ~GcNode() {
         destroy(get());
      }
   };
   ::std::vector<::std::pair<::std::unique_ptr<GcRoot>, bool>> roots;
   GC() = default;
   static auto &get() {
      static GC gc;
      return gc;
   }
   void make_collectable(void *p) {
      for (auto &[q, coll] : roots)
         if (static_cast<GcRoot*>(p) == q.get())
            coll = true;
   }
   void collect() {
      for (auto p = std::begin(roots); p != std::end(roots); ) {
         if (auto &[ptr, collectible] = *p; collectible) {
            ptr = nullptr;
            p = roots.erase(p);
         } else {
            ++p;
         }

      }
   }
   template <class T, class ... Args>
      auto add_root(Args &&... args) {
         auto q = static_cast<T*>(roots.emplace_back(
            std::make_unique<GcNode<T>>(std::forward<Args>(args)...), false
         ).first->get());
         return counting_ptr{
            q, [&,q]() {
               for (auto &[p, coll] : roots)
                  if (static_cast<void*>(q) == p.get()->get()) {
                     coll = true;
                     return;
                  }
            }
         };
      }
   template <class T, class ... Args>
      friend counting_ptr<T> gcnew(Args&&...);
   friend struct scoped_collect;
public:
   GC(const GC &) = delete;
   GC& operator=(const GC &) = delete;
};

struct scoped_collect {
   scoped_collect() = default;
   scoped_collect(const scoped_collect &) = delete;
   scoped_collect(scoped_collect &&) = delete;
   scoped_collect& operator=(const scoped_collect &) = delete;
   scoped_collect &operator=(scoped_collect &&) = delete;
   ~scoped_collect() {
      GC::get().collect();
   }
};


template <class T, class ... Args>
   counting_ptr<T> gcnew(Args &&... args) {
      return GC::get().add_root<T>(std::forward<Args>(args)...);
   }

struct NamedThing {
   const char *name;
   NamedThing(const char *name) : name{ name } {
      std::cout << name << " ctor" << std::endl;
   }
   ~NamedThing() {
      std::cout << name << " dtor" << std::endl;
   }
};

inline auto g() {
   auto _ = scoped_collect{};
   [[maybe_unused]] auto p = gcnew<NamedThing>("hi");
   auto q = gcnew<NamedThing>("there");
   return q;
}

inline auto h() {
   struct X {
      int m() const { return 123; }
   };
   return gcnew<X>();
}

inline auto f() {
   auto _ = scoped_collect{};
   auto p = g();
   std::cout << '\"' << p->name << '\"' << std::endl;
}

//}
# endif



// 04 - GC finalize with scoped collect thread safe

// use-case : destroy all GcNodes when collecting, calling the proper dtors
// using counting_ptr to know which objects to collect
#if 0
template <class T>
   class counting_ptr {
      using count_type = std::atomic<int>;
      T *p;
      count_type *count;
      std::function<void()> mark;
   public:
      template <class M>
         constexpr counting_ptr(T *p, M mark) try : p{ p }, mark{ mark } {
            count = new count_type{ 1 };
         } catch(...) {
            delete p;
            throw;
         }
      T& operator*() noexcept {
         return *p;
      }
      const T& operator*() const noexcept {
         return *p;
      }
      T* operator->() noexcept {
         return p;
      }
      const T* operator->() const noexcept {
         return p;
      }
      constexpr bool operator==(const counting_ptr &other) const {
         return p == other.p;
      }
      constexpr bool operator!=(const counting_ptr &other) const {
         return !(*this == other);
      }
      template <class U>
         constexpr bool operator==(const counting_ptr<U> &other) const {
            return p == &*other;
         }
      template <class U>
         constexpr bool operator!=(const counting_ptr<U> &other) const {
            return !(*this == other);
         }
      template <class U>
         constexpr bool operator==(const U *q) const {
            return p == q;
         }
      template <class U>
         constexpr bool operator!=(const U *q) const {
            return !(*this == q);
         }
      void swap(counting_ptr &other) {
         using std::swap;
         swap(p, other.p);
         swap(count, other.count);
         swap(mark, other.mark);
      }
      constexpr operator bool() const noexcept {
         return p != nullptr;
      }
      counting_ptr(counting_ptr &&other) noexcept
         : p{ std::exchange(other.p, nullptr) },
           count{ std::exchange(other.count, nullptr) },
           mark{ other.mark } {
      }
      counting_ptr &operator=(counting_ptr &&other) noexcept {
         counting_ptr{ std::move(other) }.swap(*this);
         return *this;
      }
      counting_ptr(const counting_ptr &other)
         : p{ other.p }, count{ other.count }, mark{ other.mark } {
         if (count) ++(*count);
      }
      counting_ptr &operator=(const counting_ptr &other) {
         counting_ptr{ other }.swap(*this);
         return *this;
      }
      ~counting_ptr() {
         if (count) {
            if ((*count)-- == 1) {
               mark();
               delete count;
            }
         }
      }
   };
namespace std {
   template <class T, class M>
      void swap(counting_ptr<T> &a, counting_ptr<T> &b) {
         a.swap(b);
      }
}

class GC {
   std::mutex m;
   class GcRoot {
      void *p;
   public:
      auto get() const noexcept { return p; }
      GcRoot(void *p) : p{ p } {
      }
      GcRoot(const GcRoot&) = delete;
      GcRoot& operator=(const GcRoot&) = delete;
      virtual void destroy(void*) const noexcept = 0;
      virtual ~GcRoot() = default;
   };
   template <class T> class GcNode : public GcRoot {
      void destroy(void *q) const noexcept override {
         delete static_cast<T*>(q);
      }
   public:
      template <class ... Args>
         GcNode(Args &&... args) : GcRoot(new T(std::forward<Args>(args)...)) {
         }
      ~GcNode() {
         destroy(get());
      }
   };
   std::vector<std::pair<std::unique_ptr<GcRoot>, bool>> roots;
   GC() = default;
   static auto &get() {
      static GC gc;
      return gc;
   }
   void make_collectable(void *p) {
      std::lock_guard _ { m };
      for (auto &[q, coll] : roots)
         if (static_cast<GcRoot*>(p) == q.get())
            coll = true;
   }
   void collect() {
      std::lock_guard _ { m };
      for (auto p = std::begin(roots); p != std::end(roots); ) {
         if (auto &[ptr, collectible] = *p; collectible) {
            ptr = nullptr;
            p = roots.erase(p);
         } else {
            ++p;
         }

      }
   }
   template <class T, class ... Args>
      auto add_root(Args &&... args) {
         std::lock_guard _ { m };
         auto q = static_cast<T*>(roots.emplace_back(
            std::make_unique<GcNode<T>>(std::forward<Args>(args)...), false
         ).first->get());
         return counting_ptr{
            q, [&,q]() {
               std::lock_guard _ { m };
               for (auto &[p, coll] : roots)
                  if (static_cast<void*>(q) == p.get()->get()) {
                     coll = true;
                     return;
                  }
            }
         };
      }
   template <class T, class ... Args>
      friend counting_ptr<T> gcnew(Args&&...);
   friend struct scoped_collect;
public:
   GC(const GC &) = delete;
   GC& operator=(const GC &) = delete;
};

struct scoped_collect {
   scoped_collect() = default;
   scoped_collect(const scoped_collect &) = delete;
   scoped_collect(scoped_collect &&) = delete;
   scoped_collect& operator=(const scoped_collect &) = delete;
   scoped_collect &operator=(scoped_collect &&) = delete;
   ~scoped_collect() {
      GC::get().collect();
   }
};


template <class T, class ... Args>
   counting_ptr<T> gcnew(Args &&... args) {
      return GC::get().add_root<T>(std::forward<Args>(args)...);
   }

struct NamedThing {
   const char *name;
   NamedThing(const char *name) : name{ name } {
      std::cout << name << " ctor" << std::endl;
   }
   ~NamedThing() {
      std::cout << name << " dtor" << std::endl;
   }
};

inline auto g() {
   auto _ = scoped_collect{};
   [[maybe_unused]] auto p = gcnew<NamedThing>("hi");
   auto q = gcnew<NamedThing>("there");
   return q;
}

inline auto h() {
   struct X {
      int m() const { return 123; }
   };
   return gcnew<X>();
}

inline auto f() {
   auto _ = scoped_collect{};
   auto p = g();
   std::cout << '\"' << p->name << '\"' << std::endl;
}
#endif



// 05 GC frees all at end no finalization
// use-case : destroy all GcNodes when GC object is destroyed, no finalization 
// (restricted to trivially destructible types)

#if 0
class GC {
   std::vector<void*> roots;
   GC() = default;
   static auto &get() {
      static GC gc;
      return gc;
   }
   template <class T, class ... Args>
      T *add_root(Args &&... args) {
         // there will be no finalization
         static_assert(std::is_trivially_destructible_v<T>);
         return static_cast<T*>(
            roots.emplace_back(
               new T(std::forward<Args>(args)...)
            )
         );
      }
   template <class T, class ... Args>
      friend T* gcnew(Args&&...);
public:
   ~GC() {
      std::print("~GC with {} objects to deallocate", std::size(roots));
      for(auto p : roots) std::free(p);
   }
   GC(const GC &) = delete;
   GC& operator=(const GC &) = delete;
};

template <class T, class ... Args>
   T *gcnew(Args &&...args) {
      return GC::get().add_root<T>(std::forward<Args>(args)...);
   }

// note: non trivially destructible
struct NamedThing {
   const char *name;
   NamedThing(const char *name) : name{ name } {
      std::print("{} ctor\n", name);
   }
   ~NamedThing() {
      std::print("{} dtor\n", name);
   }
};

struct Identifier {
   int value;
};

// would not compile
/*
void g() {
   [[maybe_unused]] auto p = gcnew<NamedThing>("hi");
   [[maybe_unused]] auto q = gcnew<NamedThing>("there");
}
*/

inline void g() {
   [[maybe_unused]] auto p = gcnew<Identifier>(2);
   [[maybe_unused]] auto q = gcnew<Identifier>(3);
}

inline auto h() {
   struct X {
      int m() const { return 123; }
   };
   return gcnew<X>();
}

inline auto f() {
   g();
   return h();
}
#endif



// 06-1 GC frees all at the end no finalization thread safe
// use-case : destroy all GcNodes when GC object is destroyed, no finalization (restricted to trivially destructible types)
#if 0
class GC {
   std::mutex m;
   std::vector<void*> roots;
   GC() = default;
   static auto &get() {
      static GC gc;
      return gc;
   }
   template <class T, class ... Args>
      T *add_root(Args &&... args) {
         std::lock_guard _ { m };
         // there will be no finalization
         static_assert(std::is_trivially_destructible_v<T>);
         return static_cast<T*>(
            roots.emplace_back(
               new T(std::forward<Args>(args)...)
            )
         );
      }
   template <class T, class ... Args>
      friend T* gcnew(Args&&...);
public:
   ~GC() {
      std::print("~GC with {} objects to deallocate", std::size(roots));
      std::lock_guard _ { m }; // redundant, but on principle...
      for(auto p : roots) std::free(p);
   }
   GC(const GC &) = delete;
   GC& operator=(const GC &) = delete;
};

template <class T, class ... Args>
   T *gcnew(Args &&...args) {
      return GC::get().add_root<T>(std::forward<Args>(args)...);
   }

// note: non trivially destructible
struct NamedThing {
   const char *name;
   NamedThing(const char *name) : name{ name } {
      std::print("{} ctor\n", name);
   }
   ~NamedThing() {
      std::print("{} dtor\n", name);
   }
};

struct Identifier {
   int value;
};

// would not compile
/*
void g() {
   [[maybe_unused]] auto p = gcnew<NamedThing>("hi");
   [[maybe_unused]] auto q = gcnew<NamedThing>("there");
}
*/

inline void g() {
   [[maybe_unused]] auto p = gcnew<Identifier>(2);
   [[maybe_unused]] auto q = gcnew<Identifier>(3);
}

inline auto h() {
   struct X {
      int m() const { return 123; }
   };
   return gcnew<X>();
}

inline auto f() {
   g();
   return h();
}
#endif



// 06-2 GC frees all at the end no finalization thread safe
// use-case : destroy all GcNodes when GC object is destroyed, no finalization 
// (restricted to trivially destructible types)
#if 0
class GC {
   std::mutex m;
   std::vector<void*> roots;
   GC() = default;
   static auto &get() {
      static GC gc;
      return gc;
   }
   template <class T, class ... Args>
      T *add_root(Args &&... args) {
         std::lock_guard _ { m };
         // there will be no finalization
         static_assert(std::is_trivially_destructible_v<T>);
         return static_cast<T*>(
            roots.emplace_back(
               new T(std::forward<Args>(args)...)
            )
         );
      }
   template <class T, class ... Args>
      friend T* gcnew(Args&&...);
public:
   ~GC() {
      std::print("~GC with {} objects to deallocate", std::size(roots));
      std::lock_guard _ { m }; // redundant, but on principle...
      for(auto p : roots) std::free(p);
   }
   GC(const GC &) = delete;
   GC& operator=(const GC &) = delete;
};

template <class T, class ... Args>
   T *gcnew(Args &&...args) {
      return GC::get().add_root<T>(std::forward<Args>(args)...);
   }

// note: non trivially destructible
struct NamedThing {
   const char *name;
   NamedThing(const char *name) : name{ name } {
      std::print("{} ctor\n", name);
   }
   ~NamedThing() {
      std::print("{} dtor\n", name);
   }
};

struct Identifier {
   int value;
};

// would not compile
/*
void g() {
   [[maybe_unused]] auto p = gcnew<NamedThing>("hi");
   [[maybe_unused]] auto q = gcnew<NamedThing>("there");
}
*/

inline void g() {
   [[maybe_unused]] auto p = gcnew<Identifier>(2);
   [[maybe_unused]] auto q = gcnew<Identifier>(3);
}

inline auto h() {
   struct X {
      int m() const { return 123; }
   };
   return gcnew<X>();
}

inline auto f() {
   g();
   return h();
}
#endif



// -------- Chapter: 12 ... -------- 

// The implementation of Vector<T>
// 253/381
namespace Chapter12_01
{
	template <class T>
	class Vector {
	public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	private:
	pointer elems{};
	size_type nelems{},
		cap{};
	// ...
	public:
	size_type size() const { return nelems; }
	size_type capacity() const { return cap; }
	bool empty() const { return size() == 0; }
	private:
	bool full() const { return size() == capacity(); }
	// ...
	public:
	using iterator = pointer;
	using const_iterator = const_pointer;
	iterator begin() { return elems; }
	const_iterator begin() const { return elems; }
	const_iterator cbegin() const { return begin(); }
	iterator end() { return begin() + size(); }
	const_iterator end() const { return begin() + size(); }
	const_iterator cend() const { return end(); }
	// ...
	Vector() = default;
	Vector(size_type n, const_reference init)
		: elems{ new value_type[n] }, nelems{ n }, cap{ n } {
		try {
			std::fill(begin(), end(), init);
		} catch (...) {
			delete[] elems;
			throw;
		}
	}
	// ...
	Vector(const Vector& other)
		: elems{ new value_type[other.size()] }, nelems{ other.size() }, cap{ other.size() } {
		try {
			std::copy(other.begin(), other.end(), begin());
		} catch (...) {
			delete[] elems;
			throw;
		}
	}
	// ...
	Vector(Vector&& other) noexcept
		: elems{ std::exchange(other.elems, nullptr) },
		nelems{ std::exchange(other.nelems, 0) },
		cap{ std::exchange(other.cap, 0) } {
	}
	// ...
	Vector(std::initializer_list<T> src)
		: elems{ new value_type[src.size()] }, nelems{ src.size() }, cap{ src.size() } {
		try {
			std::copy(src.begin(), src.end(), begin());
		} catch (...) {
			delete[] elems;
			throw;
		}
	}
	// ...
	~Vector() {
		delete[] elems;
	}
	// ...
	void swap(Vector& other) noexcept {
		using std::swap;
		swap(elems, other.elems);
		swap(nelems, other.nelems);
		swap(cap, other.cap);
	}
	Vector& operator=(const Vector& other) {
		Vector{ other }.swap(*this);
		return *this;
	}
	Vector& operator=(Vector&& other) {
		Vector{ std::move(other) }.swap(*this);
		return *this;
	}
	// ...
	reference operator[](size_type n) { return elems[n]; }
	const_reference operator[](size_type n) const { return elems[n]; }
	// precondition: !empty()
	reference front() { return (*this)[0]; }
	const_reference front() const { return (*this)[0]; }
	reference back() { return (*this)[size() - 1]; }
	const_reference back() const { return (*this)[size() - 1]; }
	// ...
	bool operator==(const Vector& other) const {
		return size() == other.size() &&
			std::equal(begin(), end(), other.begin());
	}
	// can be omitted since C++20
	bool operator!=(const Vector& other) const {
		return !(*this == other);
	}
	// ...
	void push_back(const_reference val) {
		if (full())
			grow();
		elems[size()] = val;
		++nelems;
	}
	void push_back(T&& val) {
		if (full())
			grow();
		elems[size()] = std::move(val);
		++nelems;
	}
	template <class ... Args>
		reference emplace_back(Args &&...args) {
			if (full())
				grow();
			elems[size()] = value_type(std::forward<Args>(args)...);
			++nelems;
			return back();
		}
	private:
	void grow() { resize(capacity() * 2); }
	public:
	void resize(size_type new_cap) {
		if(new_cap <= capacity()) return;
		auto p = new T[new_cap];
		if constexpr(std::is_nothrow_move_assignable_v<T>) {
			std::move(begin(), end(), p);         
		} else try {
			std::copy(begin(), end(), p);
		} catch (...) {
			delete[] p;
			throw;
		}
		delete[] elems;
		elems = p;
		cap = new_cap;
	}
	// etc.
	// two small examples, one that inserts elements
	// at a given position in the container and one
	// that erases an element at a given position
	// in the container
	template <class It>
	iterator insert(const_iterator pos, It first, It last) {
		iterator pos_ = const_cast<iterator>(pos);
		// deliberate usage of unsigned integrals
		const std::size_t remaining = capacity() - size();
		const std::size_t n = std::distance(first, last);
		if (remaining < n) {
			auto index = std::distance(begin(), pos_);
			resize(capacity() + n - remaining);
			pos_ = std::next(begin(), index);
		}
		std::copy_backward(pos_, end(), end() + n);
		std::copy(first, last, pos_);
		nelems += n;
		return pos_;
	}
	iterator erase(const_iterator pos) {
		iterator pos_ = const_cast<iterator>(pos);
		if (pos_ == end()) return pos_;
		std::copy(std::next(pos_), end(), pos_);
		*std::prev(end()) = {};
		--nelems;
		return pos_;
	}
	};



	template <class T>
	std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
	if (v.empty()) return os;
	os << v.front();
	for (auto p = std::next(v.begin()); p != v.end(); ++p)
		os << ',' << *p;
	return os;
	}

}  // End of namespace Chapter_12_01











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