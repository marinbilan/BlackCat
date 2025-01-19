#pragma once

#include "coroIf.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // For std::accumulate

namespace Coro
{


}


// Note that for parallel accumulation, use std::reduce

// SUM
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


/*
The std::accumulate algorithm is an implementation of a general concept called FOLDING (check)
(or reduction). This is a higher-order function that abstracts the process of iterating over
recursive structures such as vectors, lists, trees, and so on and lets you gradually build the
result you need.
*/


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


/*

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