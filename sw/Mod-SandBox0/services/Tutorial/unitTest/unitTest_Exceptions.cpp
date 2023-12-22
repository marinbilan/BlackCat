#include <gmock/gmock.h>
#include <iostream>
#include <vector>

#include "Exceptions.h"


// TEST FIXTURE //
class ExceptionsTest : public ::testing::Test
{
public:

	ExceptionsTest()
	{
	}

	~ExceptionsTest()
	{
	}
};


/*
Learn Advanced Modern C++ - Exceptions
*/
TEST_F(ExceptionsTest, Test_1)
{
    testExcMethod();


}