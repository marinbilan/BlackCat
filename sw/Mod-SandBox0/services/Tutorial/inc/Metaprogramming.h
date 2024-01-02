#include <iostream>
#include <vector>
#include <tuple>
#include <type_traits>

using namespace std;


template<bool condition, typename THEN, typename ELSE>
struct if_;


template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE> {
    using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE> {
    using type = ELSE;
};


template<typename SEARCH, typename TUPLE, size_t start_from = 0>
struct contains_type :
/*

If first element is true. Ignore ELSE. Init struct if_ with THEN (2nd argument) = true_type // DONE
If first element is not true:
    Check if elem in TUPLE is last elem. If yes, init with true and set false_type
        if elem is not last go to next element in TUPLE
*/
    if_<
        std::is_same<std::tuple_element_t<start_from, TUPLE>, SEARCH>::value, // 1st argument
        std::true_type, // 2nd argument
        typename if_<   // 3th argument (typename is needed here)
            (start_from == std::tuple_size<TUPLE>::value - 1),  // Is this last element? If true, go back init to false type
            std::false_type,
            contains_type<SEARCH, TUPLE, start_from + 1>
        >::type
    >::type
{};