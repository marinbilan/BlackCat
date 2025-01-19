#include <gmock/gmock.h>


#include "coro0.h"


// TEST FIXTURE //
class coro_0_Test : public ::testing::Test
{
public:

	coro_0_Test()
	{
	}

	~coro_0_Test()
	{
	}
};


TEST_F(coro_0_Test, Test_0)
{
	std::vector<int> vecInt_0 = {2, 4, 6};

	double avgSum = average_score(vecInt_0);
	std::cout << "Average value: " << avgSum << '\n';	

	double multi = scores_product(vecInt_0);
	std::cout << "Multiply value: " << multi << '\n';

	char char0[6] = {'a', 'a', '\n', 'a', '\n', 'a'};
	int numOfLines = count_lines(char0);

	std::cout << "Number of lines: " << numOfLines << '\n';
}