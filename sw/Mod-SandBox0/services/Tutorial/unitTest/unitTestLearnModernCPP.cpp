#include <gmock/gmock.h>
#include <vector>

#include "UDEMY_Learn_Modern_CPP_Sec_6.h"


// TEST FIXTURE //
class LearningModernCPPUnitTest : public ::testing::Test
{
public:

	LearningModernCPPUnitTest()
	{
	}

	~LearningModernCPPUnitTest()
	{
	}
};


TEST_F(LearningModernCPPUnitTest, Sec_6_72)
{
	// Create lambda and store in variable
	// Call later with name (string) argument
	auto greet = greeter("Hello");

	// Call object (stored lambda) with arg name ("Students")
	std::cout << greet("Students") << '\n';

}


TEST_F(LearningModernCPPUnitTest, Disabled_Sec_7_1)
{
	CompareTest Tmp_10(10);
	CompareTest Tmp_11(11);
	CompareTest Tmp_12(12);
	CompareTest Tmp_13(13);  // <----
	CompareTest Tmp_14(14);
	CompareTest Tmp_15(15);

	std::vector<CompareTest> vec1 = { 
		Tmp_10, Tmp_11, Tmp_12, Tmp_13, Tmp_14, Tmp_15 };

	std::vector<CompareTest> vec2 = { 
		Tmp_13, Tmp_14 };

	// Find iterator - note that elements are comparing ==
	auto itt = std::find_first_of(std::begin(vec1), std::end(vec1), std::begin(vec2),
		std::end(vec2));

	std::cout << "itt: " << (*itt).m_id << '\n';

	std::vector<CompareTest>::iterator it = vec1.begin();
	std::vector<CompareTest>::iterator it2 = vec1.begin();
	
	if(*it == *it2) { std::cout << "ok" << '\n'; }

	// Find next
	auto itt2 = std::find_first_of(std::next(itt), std::end(vec1), std::begin(vec2),
		std::end(vec2));

	std::cout << "itt2: " << (*itt2).m_id << '\n';

}


TEST_F(LearningModernCPPUnitTest, Sec_7_81)
{
	std::string str{"A test string"};

	std::cout << "for_each" << '\n';
	std::for_each(std::cbegin(str), std::cend(str), 
		[](const char c) {
			std::cout << c << " ";
		});

	std::cout << '\n';

	// The same 
	std::cout << "for loop" << '\n';
	for(auto s : str)
	{
		std::cout << s << " ";
	}

	std::cout << '\n';
}


TEST_F(LearningModernCPPUnitTest, Sec_7_82)
{
	// Copy elemets with predicate - if true is returned
	std::vector<int> vec{1, 2, 3, 4, 5};

	std::vector<int> vec2;

	std::copy_if(cbegin(vec), cend(vec), std::back_inserter(vec2), [](int n) { 

		// Do some logic with n and copy only elems which returns true
		// Only odd numbers for example using modulo arithm
		return true; 
	});
}