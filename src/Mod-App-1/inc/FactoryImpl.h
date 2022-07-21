#pragma once

#include "FactoryIf.h"
#include <iostream>


// https://codereview.stackexchange.com/questions/147506/unit-testing-friendly-singleton
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