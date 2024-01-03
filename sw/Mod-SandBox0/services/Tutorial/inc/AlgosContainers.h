// #include <iostream>
#include <numeric>

// #include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <exception>
#include <optional>
#include <memory>


using namespace std;


// ---- ALGORITHMS ----

class Element {
public:
    Element(int id) : m_id(id) {}

    int getId() const {
        return m_id;
    }

    bool operator==(const Element& el) {
        if(m_id == el.getId()) {
            return true;
        }

        return false;
    }

    bool operator<(const Element& el) {
        if(m_id < el.getId()) {
            return true;
        }

        return false;
    }

private:
int m_id;
};


// Helper method
bool isBiggerMethod(const Element& lhs, const Element& rhs) {
    std::cout << "... sorting ... lhs: " << lhs.getId() << " rhs: " << rhs.getId() << '\n';
    return lhs.getId() < rhs.getId();
}


// Helper functor
class IsBiggerFunctor {
public:
    bool operator()(const Element& lhs, const Element& rhs) {
        return lhs.getId() < rhs.getId();
    }
};


void testF() {

    std::cout << "__Algos Containers" << '\n';
}


// _if() helper Functor - Hardcoded
class ge_5 {
public:

    bool operator()(const std::string& s) const {

        // return true for operation we want to perform
        return s.size() > 5;
    }
};


// _if() helper Functor - General
class ge_n {
public:
    ge_n(int n) : n(n) {}

    bool operator()(const std::string& s) const {

        // return true for operation we want to perform
        return s.size() > n;
    }

private:
// const int n;
long unsigned int n;
};


// Numeric Algorithms
class TestClass {
public:
    TestClass(int id) : m_id(id) {}

    // std::iota
    // ++
    // = (compiler generated operator)
    TestClass& operator++() {
        std::cout << "Calling operator++()" << '\n';
       
        // Increase value of this object for ++
        ++m_id;
       
        return *this;
    }

    // std::accumulate
    TestClass operator+(const TestClass& obj)
    {
        std::cout << "TestClass operator+(const TestClass& obj)" << '\n';
       
        TestClass temp(0);
        // m_id = test0
        temp.m_id = m_id + obj.m_id;
       
        //std::cout << "m_id = " << m_id << '\n';
        //std::cout << "obj m_id = " << obj.m_id << '\n';
       
        return temp;
    }


int m_id;
};



// ---- CONTAINERS ----

// ---- ARRAY ----



// ---- MAP ----
// Map Ex 0
class Widget {
public:
    Widget(int id) : m_id(id) {
       
        // Create some vectors
        std::vector<int> vec0;
        std::vector<int> vec1;
       
        mapVec.insert(std::pair(0, vec0));
        mapVec.insert(std::pair(1, vec1));
    }
   
    // set
    void set(int id, int value) {
        // Find vec and add value
        (mapVec.find(id)->second).push_back(value);
    }

    // get
    const std::vector<int>& getVec(int id) const {
       
        return mapVec.find(id)->second;
    }

private:
int m_id;
std::map<int, std::vector<int>> mapVec;
std::map<int, std::vector<int>>::iterator mapIt;
};



// Map Ex 1
// //// ERROR HANDLING ////
struct ErrorCodes
{
    static const short FILE_NOT_FOUND = 21;
   
    static std::string GetErrorDesc(short const code);
};


static const std::map<short, char const* const> descriptions =
{
    { ErrorCodes::FILE_NOT_FOUND, "ERROR: File not found" }
};


std::string ErrorCodes::GetErrorDesc(short const code)
{
    const auto it = descriptions.find(code);
    if(descriptions.cend() == it)
    {
        return "Unknown error";
    }
    return it->second;
}


class MARException : public std::exception
{
public:
    MARException(short code) : code(code) {}
   
    short getErrorCode() const
    {
        return code;
    }

private:
short code;
};
// //// ////


// ----
class Widget0
{
public:
    Widget0()
    {
    }

    Widget0(std::string const& name) : m_name(name)
    {
    }

    int initConf0(std::function<int(std::string&)>& funcObj)
    {
        std::string tempStr("normalParam");
        funcObj(tempStr);
       
        return 0;
    }
   
    template<typename FileCallback>
    int initConf1(FileCallback& funcObj)
    {
        std::cout << "widget0 - read some xml files paths ..." << '\n';
        // Foreach path call generic funcObj
        std::string path1("...path1");
        // std::string path1("...path1"); // To test two same confs in map
        std::string path2("...path2");
        std::string path3("...path3");
       
        funcObj(path1);
        funcObj(path2);
        funcObj(path3);
       
        return 0;
    }
   
private:
std::string m_name;
};


class Widget1
{
public:
    Widget1(std::string const& name) : m_name(name) // , m_widget(name)
    {
    }
   
    void setWidget0(const Widget0& widget)
    {
        m_widget = widget;
    }
    void initConfs()
    {
        bool valsOK = false;
       
        // Important, because we pass 'this' as capture variable,
        // we are able to call m_XMLReader and m_mapConfs in other object
        // when we pass lambda further!
        std::function<int(std::string&)> readConf = [this, &valsOK](const std::string& fullPath) -> int
        {
            std::cout << "Parsing : " << fullPath << '\n';
           
            // Do some reading using member parser ex:
            // Configuration cfg;
            // m_XMLReader->readConf(cfg); // Get cfg object via reference and insert in map
           
            /*
            IMPORTATNT: It is not possible to insert two pairs with
            the same second into map. This is good check that configuration already
            exists
            */
            m_mapConfs.insert(std::make_pair(fullPath, 0));
           
            // This is how to check if configuration already exists in map - use this
            // if(!m_mapConfs.insert(std::make_pair(fullPath, 0)).second) std::cout << "Conf already exists!" << '\n';
            // This is how it should look like
            // if(!m_mapConfs.insert(std::make_pair(cfg.getId(), cfg).second) std::cout << "Conf already exists!" << '\n';
           
            valsOK = true;
            return 0; // Return some ok/error info
        };
       
        m_widget.initConf1(readConf);
    }

    void initSomeOtherConfs()
    {
        bool valsOK = false;
       
        auto readSomeOtherConf = [this, &valsOK](const std::string& fullPath) -> int
        {
            std::cout << "Parsing : " << fullPath  << '\n';
            // Do some reading using member parser ex:
            // Configuration cfg;
            // m_XMLReader->readSomeOtherConf(cfg); // Get cfg object via reference and insert in map
            m_mapSomeOtherConfs.insert(std::make_pair(fullPath, 0));

            valsOK = true;
           
            return 0; // Return some ok/error info
        };
        m_widget.initConf1(readSomeOtherConf);
    }
   
    void printConfMap()
    {
        // Iterate via map (C++ 17 style)
        for(auto& [confName, conf] : m_mapConfs)
        {
            std::cout << "Configuration name: " << confName << " conf: " << conf << '\n';
        }
    }
   
    void printSomeOtherConfMap()
    {
        // Iterate via map (C++ 17 style)
        for(auto& [confName, conf] : m_mapSomeOtherConfs)
        {
            std::cout << "Configuration name: " << confName << " conf: " << conf << '\n';
        }
    }

    bool throwExc(bool throwExc)
    {
        if(throwExc) throw MARException(ErrorCodes::FILE_NOT_FOUND);
       
        return 1;
    }

    // Longer version - Check down for shorter version
    std::optional<std::map<std::string,int>::iterator> getConfig()
    {
        // Desired config
        std::pair<std::string, int> desiredConfig("...path2", 0);
        auto desiredConfigFunc = [&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; };

        // Iterate via map
        auto foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), desiredConfigFunc);
        if(foundConfigInMap != m_mapConfs.end())
        {
            std::cout << "Value is found" << '\n';
            return foundConfigInMap;
        }

        return std::nullopt;
    }

    // Shorter version
    // This method returns copy of iterator
    // const std::optional<std::map<std::string,int>::iterator> getConfig(std::pair<std::string, int> const& desiredConfig)
    std::map<std::string,int>::iterator getConfigIter(std::pair<std::string, int> const& desiredConfig)
    {
        // Iterate via map
        if(auto foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), [&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; });
        foundConfigInMap != m_mapConfs.end() )
        {
            std::cout << "foundConfigInMap: " << foundConfigInMap->second << '\n';
            std::cout << "foundConfigInMap->second address: " << &(foundConfigInMap->second) << '\n';
            return foundConfigInMap;
        }
        return m_mapConfs.end();
    }

    // ****
    // std::optional<int> getConfigFromMap(std::pair<std::string, int> const& desiredConfig)
    std::optional<std::reference_wrapper<int>> getConfigFromMap(std::pair<std::string, int> const& desiredConfig)
    {
        // Iterate via map - this iterator always copy value in itself
        if(const auto& foundConfigInMap = std::find_if(m_mapConfs.begin(), m_mapConfs.end(), [&desiredConfig](std::pair<std::string, int> const& confInMap) { return desiredConfig == confInMap; });
        foundConfigInMap != m_mapConfs.end() )
        {
            std::cout << "found ConfigInMap: " << foundConfigInMap->second << '\n';
            return foundConfigInMap->second;
        }

        return std::nullopt;
    }

private:
std::string m_name;
Widget0 m_widget; // Important: Widget0() - default constructor needs to be declared
std::map<std::string, int> m_mapConfs;
std::map<std::string, int> m_mapSomeOtherConfs;
};


// Ex 2
class WidgetEx2
{
public:
    WidgetEx2(int id) : m_id(id) {}

    int getId() const
    {
    return m_id;
    }

private:
int m_id;
};