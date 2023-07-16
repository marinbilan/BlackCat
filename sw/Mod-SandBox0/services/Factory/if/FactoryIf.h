#pragma once


namespace Common
{

class FactoryIf
{
public:
    // This will be implemented outside of (derived) FactoryImpl
    static FactoryIf& getInstance();
    
    virtual ~FactoryIf()
    {
        // std::cout << "Deleting FactoryIf base class" << '\n';
    }

    virtual void init() = 0;

    virtual void doSomething() = 0;
};

} // End of namespace