#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "AlgosContainers.h"


// TEST FIXTURE //
class AlgosContainersTest : public ::testing::Test
{
public:

	AlgosContainersTest()
	{
	}

	~AlgosContainersTest()
	{
	}
};


/*
Learn Advanced Modern C++ - Algorithms, Containers
*/
// [65] Algorithms overview
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosOverview)
{

    // testF();
    std::string str{"Hello World"};
    std::cout << "String: ";

    for(auto c : str) {
        std::cout << c << ", ";
    }
    std::cout << "\n";

    auto res = std::find(std::cbegin(str), std::cend(str), 'l');

    std::cout << "res: " << *res << '\n';

    if(res != std::cend(str)) {
        std::cout << "Found a matching element at index: " << res - str.begin() << '\n';
    }
}


// [66] Algorithms with predicates
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosWithPredicates)
{
    Element el1(11);
    Element el2(13);
    Element el3(12);

    std::vector<Element> vecOfElems = {el1, el2, el3};

    // Sort using std::sort
    std::sort(std::begin(vecOfElems), std::end(vecOfElems));

    for(auto s : vecOfElems) {
        std::cout << s.getId() << '\n';
    }

    // Sorting using std::sort with predicate
    std::cout << "Sort using predicate" << '\n';
    std::sort(std::begin(vecOfElems), std::end(vecOfElems), isBiggerMethod);

    // Sorting using std::sort using functor
    std::cout << "Sort using functor" << '\n';
    std::sort(std::begin(vecOfElems), std::end(vecOfElems), IsBiggerFunctor());
}


// [67] Algorithms with _if versions
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosWith_ifVersions)
{
    std::vector<std::string> names = {"PHB", "Dilbert", "Asok", "Dogbert", "Ted", "Alice", "Wally"};

    auto res = find_if(std::cbegin(names), std::cend(names), ge_5());
    // First name with less than 5 characters
    // auto res = find_if_not(std::cbegin(names), std::cend(names), ge_5());

    if(res != cend(names)) {
        std::cout << "First name with more than 5 charachters: " << *res << '\n';
    }

    // General case (n defined in constructor)
    // auto res_gen = find_if(std::cbegin(names), std::cend(names), ge_n(5));
}


// [68] Lambda Expression Introduction
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionIntro)
{
    std::vector<int> vec{3, 1, 4, 1, 5, 9};

    // Pass Lambda Expression
    auto odd_it = std::find_if(std::cbegin(vec), std::cend(vec),
        [](int n) {
            return (n % 2 == 1);
        });

    // Iterator of the first element true with predicate
    if(odd_it != std::cend(vec)) {
        std::cout << "First odd element is: " << *odd_it << '\n';
    }
}


// [69] Lambda Expression Practical
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionPractical)
{

}


// [70] Lambda Expression Capture
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionCapture)
{

}


// [79] Numeric Algorithms
TEST_F(AlgosContainersTest, DISABLED_Test_NumericAlgorithms)
{
/*
    // [1]
    // std::iota
    // Operators: ++, = (compiler generated)
    Test t3(3);
    Test t4(4);
   
    std::vector<Test> vec = {t3, t4};
 
    // Start value from start_value
    Test start_value(133);
    // Start adding objects t(m_id = 113), t(m_id = 114), t(m_id = 115) ...
    // Note that iterators are not const because value of vector needs to be modify
    std::iota(std::begin(vec), std::end(vec), start_value);

    for(auto s : vec)
    {
        std::cout << s.m_id << '\n';
    }
   
    // Check operators: +, ++
    */

    // [2]
    // std::accumulate
    // Operators: +, = (compiler generated), ++ (does not need to be implemented)
   
    TestClass t3(3);
    TestClass t4(4);
   
    /*
    TestClass t_0(0);  // Create default constructor
    t_0 = t3 + t4;
    std::cout << "t_0 = " << t_0.m_id << '\n';
    */
   
    std::vector<TestClass> vec = {t3, t4};
   
    // Accumulate from custom object (IMPORTANT) (Not possible from 0)
    TestClass start_point(117);

    TestClass accumulated_value = std::accumulate(cbegin(vec), cend(vec), start_point);  
    std::cout << "Accumulated value = " << accumulated_value.m_id << '\n';
   
    // Accumulate with extra argument
    TestClass accumulated_value2 = std::accumulate(cbegin(vec), cend(vec), start_point,
        [](TestClass start_point_sum, TestClass val_of_each_iter_elem) {
            // std::cout << "> sum: " << start_point_sum.m_id << '\n';
            // std::cout << "> elem: " << val_of_each_iter_elem.m_id << '\n';
           
            TestClass temp(118);
           
            // Do some logic here for summing elements ...
            // If this is even add if not do not add element to sum
            // return (n % 2 == 1) ? sum + n : sum;
           
            // return temp;

            // % operator is not implemented so we need to take .m_id
            return (val_of_each_iter_elem.m_id % 2 == 1) ?
                start_point_sum + val_of_each_iter_elem : start_point_sum;
        });
       
    std::cout << "Temp value: " << accumulated_value2.m_id << '\n';
}


// ---- CONTAINERS ----

// ---- MAP ----
// Map in combinations wiht Vector
// Ex 0
TEST_F(AlgosContainersTest, DISABLED_Test_Map_0)
{
    std::vector<int> vec = {3, 2, 1};
    std::map<int, std::vector<int>> mapVec;
   
    mapVec.insert(std::pair(1, vec));
    mapVec.insert(std::pair(4, vec));
    mapVec.insert(std::pair(2, vec));
    mapVec.insert(std::pair(3, vec));
   
    std::map<int, std::vector<int>>::iterator mapIt = mapVec.begin();

    //std::cout << "First: " << mapIt->first << " Second: " << mapIt->second.at(0) << '\n';
    mapIt++;
    //std::cout << "First: " << mapIt->first << " Second: " << mapIt->second.at(0) << '\n';
    mapIt++;
    //std::cout << "First: " << mapIt->first << " Second: " << mapIt->second.at(0) << '\n';
    mapIt++;
    //std::cout << "First: " << mapIt->first << " Second: " << mapIt->second.at(0) << '\n';
   
    // ----
    for(auto s : mapIt->second) {
        //std::cout << s << '\n';
    }

    //
    Widget w(0);
   
    w.set(0, 0);
    w.set(0, 1);
    w.set(0, 2);
    w.set(0, 3);
   
    for(auto s : w.getVec(0)) {
        std::cout << s << '\n';
    }
}


// Ex 1
TEST_F(AlgosContainersTest, DISABLED_Test_Map_1)
{
    /*
    std::map<std::string, int> mapTest;
    mapTest.insert(std::pair("Str0", 0));
    mapTest.insert(std::pair("Str1", 1));
    mapTest.insert(std::pair("Str2", 2));
    mapTest.insert(std::pair("Str3", 3));
    mapTest.insert(std::pair("Str4", 4));
   
    // ---- Iterate via map C++-17 style----
    for(auto& [it0, it1] : mapTest)
    {
        std::cout << it0 << " " << it1 << '\n';
    }

    // ----
    auto iter = mapTest.find("Str1");
   
    std::cout << "----" << '\n';
    std::cout << iter->first << " " << iter->second << '\n';
   
   
    // ----
    auto conf = mapTest["Str3"];
    std::cout << "Str3: " << conf << '\n';

    // ---- Find desired pair using lambda expression
    std::pair<std::string, int> desiredValue("Str2", 2);
   
    auto findIf = std::find_if(mapTest.begin(), mapTest.end(), [&desiredValue](const std::pair<std::string, int>& mapValue) { return desiredValue == mapValue; });
    std::cout << "----" << '\n';
    std::cout << findIf->second << '\n';
    if(mapTest.insert(std::pair("Str5", 5)).second) std::cout << "OK" << '\n';
   
   
    // ---- TODO - Implement function object lambda and pass it to method (string, func_obj)
    */

    /*
    auto readConf = [](std::string& confStr)
    {
        std::cout << "This is call from readConf lambda with param: " << confStr  << '\n';
        return 0;
    };

    Widget0 w0("widget0");
   
    w0.initConf0(readConf);
    w0.initConf1(readConf);
    */
    Widget0 w0("widget0");
    Widget1 w1("widget1");
   
    w1.setWidget0(w0);
   
    w1.initConfs();
    w1.initSomeOtherConfs();

    w1.printConfMap();
    std::cout << "----" << '\n';
    w1.printSomeOtherConfMap();
   
    // ----

    try
    {
        w1.throwExc(true);
    }
    catch(MARException ex)
    {
        std::cout << ErrorCodes::GetErrorDesc(ex.getErrorCode()) << '\n';
    }

    // ----
    /*
    std::map<std::string, int> mapTest;
    mapTest.insert(std::pair("Str0", 0));
    mapTest.insert(std::pair("Str1", 1));
    mapTest.insert(std::pair("Str2", 2));
    mapTest.insert(std::pair("Str3", 3));
    mapTest.insert(std::pair("Str4", 4));  

    std::pair<std::string, int> desiredConf("Str2", 2);
    // First step - prepare lambda function
    auto findDesiredConf = [&desiredConf](const std::pair<std::string, int>& mapVals) { return desiredConf == mapVals; };
    // Second step - iterate via map using lambda function
    // auto it = std::find_if(mapTest.begin(), mapTest.end(), findDesiredConf);
    auto it = std::find_if(mapTest.begin(), mapTest.end(), [&desiredConf](const std::pair<std::string, int>& mapVals) { return desiredConf == mapVals; });
   
    // If iterator (it) is one elem after last in map -
   
    std::cout << "Elem key: " << it->first << '\n';
    std::cout << "Elem value: " << it->second << '\n';
    */

    std::cout << "-- TEST --" << '\n';
   
    // Get Iterator from map and chage value of map entry
    w1.printConfMap();
    std::pair<std::string, int> desiredValue2("...path2", 0);
    auto it = w1.getConfigIter(desiredValue2);
    std::cout << "In main: &(it->second): " << &(it->second) << '\n';
    it->second = 42;
    w1.printConfMap();
   
   
    // ----
    // Get config from map
    std::pair<std::string, int> desiredValue3("...path3", 0);
    std::optional<int> desiredValue3Opt = w1.getConfigFromMap(desiredValue3);
   
    if(desiredValue3Opt.has_value()) std::cout << "Has value!" << '\n';
   
    desiredValue3Opt.value() = 34;
    // confVal = 34;
    w1.printConfMap();

}


// Ex 2
TEST_F(AlgosContainersTest, Test_Map_2)
{
    // ---- Basic iterator operations ----
    std::map<std::string, int> myMap;
 
    // insert elements in random order
    myMap.insert(std::pair<std::string, int>("First", 40));
    myMap.insert(std::pair<std::string, int>("Second", 30));
    myMap.insert(std::pair<std::string, int>("Third", 60));
    myMap.insert(std::pair<std::string, int>("Forth", 20));

    std::map<std::string, int>::iterator it0 = myMap.begin();
    for(; it0 != myMap.end(); it0++)
    {
    // Iterator address is always the same - only value is increased
            // Access underlying value in vector: *it
            // Access underlying value in map: it->first, it->second
    std::cout << "it0 address: " << &it0 << " first vec elem address: " << &(it0->first) << " second vec elem value: " << it0->second << '\n';
    std::cout << " first map elem value: " << it0->first << " second map elem value: " << it0->second << '\n';
    }    

    std::cout << " ---- " << '\n';
    for(const auto& [s0, s1] : myMap) // Important: auto is int NOT iterator!
    {
    std::cout << " first map elem value: " << s0 << " second map elem value: " << s1 << '\n';
    }

    // Return iterator on first element greather than 10
    // it1 will have own address, but underlying elem of map will have the same address and value
    // Iterator is returned but as argument we have pair (not pointer type) - so we have access elemInMap.second
    std::map<std::string, int>::iterator it1 = std::find_if(myMap.begin(), myMap.end(),
    [=](const auto& elemInMap){ return  elemInMap.second >= 10; });

    std::cout << '\n' << "it1 address: " << &it1 << " mape elem address: " << &(it1->first) << " map elem value: " << it1->second << '\n';


    // ---- Basic filtering ----
    // TODO

    // ---- Test vector of shared pointers ----
    std::shared_ptr<WidgetEx2> p0 = std::make_shared<WidgetEx2>(17);
    std::shared_ptr<WidgetEx2> p1 = std::make_shared<WidgetEx2>(213);
    std::shared_ptr<WidgetEx2> p2 = std::make_shared<WidgetEx2>(19);
    std::shared_ptr<WidgetEx2> p3 = std::make_shared<WidgetEx2>(1);

    std::map<std::string, std::shared_ptr<WidgetEx2>> mySharedMap; //  {p0, p1, p2, p3};

    mySharedMap.insert(std::pair<std::string, std::shared_ptr<WidgetEx2>>("First",  p0));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<WidgetEx2>>("Second", p1));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<WidgetEx2>>("Third",  p2));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<WidgetEx2>>("Fourth", p3));

    // By dereferencing iterator (*) - access underlying shared pointer in vector
    std::map<std::string, std::shared_ptr<WidgetEx2>>::iterator it2 = mySharedMap.begin();
    for(; it2 != mySharedMap.end(); it2++)
    {
        std::cout << "Map of shared ptrs: " << it2->first << " " << it2->second->getId() << '\n';
    }

    // Filter something
    // Filter - do something with filtered shared ptr Widgets
    int param = 15;
    //
    std::cout << '\n';

    for_each(mySharedMap.begin(),mySharedMap.end(),[=](std::pair<std::string, std::shared_ptr<WidgetEx2>> const& p)
    {
    // Here p is shared_ptr Widget - NOT iterator - std::pair! - Access using .
        if( p.second->getId() > param ) std::cout << "Filter print:" << p.first << " " << " id: " << p.second->getId() << '\n';
    });
   
}