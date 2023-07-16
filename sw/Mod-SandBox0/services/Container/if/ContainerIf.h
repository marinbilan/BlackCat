#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "InvDevIf.h"
#include "ServiceIf.h"



namespace Services
{

class ContainerIf
{
public:
	virtual ~ContainerIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

	// Devs
	virtual void setInvDevIf(const std::shared_ptr<Services::InvDevIf>& invDevIf) = 0;

	virtual std::vector<std::shared_ptr<Services::InvDevIf>>& getInvDevIfVec() = 0;

	//
	virtual void setServiceIf(const std::shared_ptr<Services::ServiceIf>& serviceIf) = 0;

	virtual std::shared_ptr<Services::ServiceIf> getServiceIf(const std::string& instName) = 0;

private:
};

} // End of namespace