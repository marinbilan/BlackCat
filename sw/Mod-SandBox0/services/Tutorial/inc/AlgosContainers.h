// #include <iostream>
#include <numeric>
// #include <vector>

using namespace std;


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