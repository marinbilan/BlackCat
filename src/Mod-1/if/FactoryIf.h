#pragma once


namespace Common
{
class FactoryIf
{
public:
    // This will be implemented outside of FactoryImpl
    static FactoryIf& getInstance();
    
    virtual ~FactoryIf()
    {
        // std::cout << "Deleting FactoryIf base class" << '\n';
    }
       
    // virtual void init() = 0;
    // virtual void do_something() = 0;
    // virtual void set_something(int i) = 0;
};
}