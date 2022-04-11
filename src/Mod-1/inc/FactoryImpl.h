#pragma once

#include "FactoryIf.h"
#include <iostream>


namespace Common
{

class FactoryImpl final : public FactoryIf
{
public:
	~FactoryImpl()
	{}

	void init() override 
	{
		std::cout << "init() FactoryImpl" << '\n';
	}

	void doSomething() override 
	{
		std::cout << "doSomething() FactoryImpl" << '\n';
	}

	void operator=(FactoryImpl const&) = delete;
};


FactoryIf& FactoryIf::getInstance()
{
    static FactoryImpl instance;
    return instance;
}

} // End of namespace