#include <gmock/gmock.h>
#include <iostream>
#include <vector>

#include "UDEMY_Learn_Modern_CPP_Exceptions.h"


// TEST FIXTURE //
class LearnModernCPPExceptionsTest : public ::testing::Test
{
public:

	LearnModernCPPExceptionsTest()
	{
	}

	~LearnModernCPPExceptionsTest()
	{
	}
};


TEST_F(LearnModernCPPExceptionsTest, Test_1)
{
    testExcMethod();


}