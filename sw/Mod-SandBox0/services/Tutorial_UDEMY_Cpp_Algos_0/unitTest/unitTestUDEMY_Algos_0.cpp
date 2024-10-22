#include <gmock/gmock.h>
#include <iostream>
// #include <vector>
// #include <string>

#include "UDEMY_Algos_0.h"


// TEST FIXTURE //
class UDEMY_Algos_0_Test : public ::testing::Test
{
public:

	UDEMY_Algos_0_Test()
	{
	}

	~UDEMY_Algos_0_Test()
	{
	}
};


TEST_F(UDEMY_Algos_0_Test, Recursion_0)
{

  Services::Algos0 srv0("Inst0");
  srv0.preInit();
}


TEST_F(UDEMY_Algos_0_Test, Test_MyString_0)
{

  // [1] Constructor
  // Services::MyString obj0("my_str");

 // [2] Copy Constructor

/*
  MyString obj1(obj0);
  MyString obj2 = obj1;  // Object is not initialized so copy constructor is called

  obj0.info();
  obj1.info();
  obj2.info();

  // [3] Assignment Operator (object is already initialized)
  MyString obj3("obj3");
  MyString obj4("obj4");
  obj3 = obj4;

std::cout << '\n' << " ---- Iterate ---- " << '\n';
*/

  /*
  for (auto it = obj0.begin(); it != obj0.end(); ++it) {
    std::cout << *it;
  }
  */

  /*
  template<class InputIt, class UnaryFunc>
  constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f)
  {
      for (; first != last; ++first)
          f(*first);
 
      return f; // implicit move since C++11
  }
  */

  // 1st Case
  std::vector<char> v;

  /*

  // std::for_each(obj0.begin(), obj0.end(), [&](char c) {
  my_for_each(obj0.begin(), obj0.end(), [&](char c) {
    if (c == '_') {
      std::cout << "Char _ found" << '\n';
      v.push_back(c);
    }
    std::cout << c;
  });

 // 2nd Case - Define structure
  struct Chr {
    void operator()(char ch) {
      std::cout << "Chr: " << ch << '\n';
      v.push_back(ch);
    };

    void print() {}

    char c;
    std::vector<char> v;
  };

  Chr cr = std::for_each(obj0.begin(), obj0.end(), Chr());
  cr.print();

  */
  std::cout << '\n' << " ---- Process MyString ---- " << '\n';


  Services::ProcessMyString process_0(0);
  // IMPORTANT: Moved object is no more valid

  Services::MyString obj0("my_str_0");
  Services::MyString obj1("my_str_1");
  Services::MyString obj2("my_str_2");
  Services::MyString obj3("my_str_3");

  // [ PASS BY VALUE ]
  // Non-const
  process_0.passMyString(obj0);
  process_0.passMyString(std::move(obj1));
  // const
  process_0.passMyConstString(obj2);
  process_0.passMyConstString(std::move(obj3));


  // [ PASS BY REFERENCE ]
  // Non-const
  // process_0.passMyStringRef(obj0);
  // process_0.passMyStringRef(std::move(obj0)); // NOK
  // const
  // process_0.passMyStringConstRef(obj0);
  // process_0.passMyStringConstRef(std::move(obj0));



  // [ PASS BY RVALUE REFERENCE ] (&&)
  // Non-const
  // process_0.passMyStringRValRef(obj0);  // NOK
  // process_0.passMyStringRValRef(std::move(obj0));
  // const
  // passMyStringConstRValRef(obj0);  // NOK
  // process_0.passMyStringConstRValRef(std::move(obj0));  // OK

}


TEST_F(UDEMY_Algos_0_Test, Test_LinkedList)
{
  Services::LinkedList<Services::MyString> listOfMyStrings;

  Services::MyString myStr0("my_str0");
  Services::MyString myStr1("my_str1");
  listOfMyStrings.append(myStr0);
  listOfMyStrings.append(myStr1);

  listOfMyStrings.print();

  listOfMyStrings[0].info();

  for(auto& s : listOfMyStrings) {
    // s.info();

    my_for_each(s.begin(), s.end(), [&](char c) {
      if (c == '_') {
        std::cout << "Char _ found" << '\n';
        // v.push_back(c);
      }
      std::cout << c;
    });

  }


  // listOfMyStrings.clear();
  std::cout << "6" << '\n';
}
