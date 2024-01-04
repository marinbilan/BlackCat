#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;


// ----
class ClientBase
{
public:
    ClientBase(int id) : m_id(id)
    {
        std::cout << "Calling ClientBase" << '\n';
    }

    virtual ~ClientBase()
    {
        std::cout << "Deleting ClientBase" << '\n';
    }
   
    int getBaseId() const
    {
        std::cout << "Calling getBaseId from ClientBase - id:" << m_id << '\n';
        return m_id;
    }

    virtual int getBaseIdVirtual() const
    {
        std::cout << "Calling getBaseIdVirtual from ClientBase - id:" << m_id << '\n';
        return m_id;
    }

    // If I have only one pure virtual method - unable to create base class
    // virtual void pureVirt() = 0;
   
private:
int m_id;
};


// ----
class ClientImpl : public ClientBase
{
public:
    ClientImpl(int id) : m_id(id), ClientBase(id)
    {
        std::cout << "Calling ClientImpl" << '\n';
    }
   
    ~ClientImpl()
    {
        std::cout << "Deleting ClientImpl" << '\n';
    }

    int getBaseIdVirtual() const
    {
        std::cout << "Calling getBaseIdVirtual from ClientImpl - id:" << m_id << '\n';
        return m_id;
    }
   
    // void pureVirt() {};
   
private:
int m_id;
};


void f(ClientBase clientBase)
{
    clientBase.getBaseIdVirtual();  
}


void ff(ClientImpl clientImpl)
{
    clientImpl.getBaseIdVirtual();  
}


void fff(ClientBase* clientBasePtr)
{
    clientBasePtr->getBaseIdVirtual();
}
