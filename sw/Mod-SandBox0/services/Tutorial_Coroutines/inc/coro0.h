#pragma once
#include <thread>
#include "coroIf.h"
#include <cstring>
#include <coroutine>


namespace Coro
{

class Coro : public TutorialIf
{
public:
	Coro(const std::string& name);

	~Coro();

	const std::string& getName() { return "CoroTestStr"};

	void preInit() {};
	void postInit() {};


private:
std::string m_name;
};