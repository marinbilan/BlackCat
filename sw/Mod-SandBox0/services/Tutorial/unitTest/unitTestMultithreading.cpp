#include <gmock/gmock.h>

// #include "TutorialIf.h"
#include "MultithreadingHelp.h"
#include "Multithreading.h"


// using ::testing::AtLeast;

// TEST FIXTURE //
class MultithreadingUnitTest : public ::testing::Test
{
public:

	MultithreadingUnitTest()
	{
	}

	~MultithreadingUnitTest()
	{
	}
};


/*
UDEMY COURSE
Ch 1 - [2] Introduction to parallel computing
Ch 1 - [3] Heterogeneous computing
Ch 1 - [4] How to launch thread
*/
TEST_F(MultithreadingUnitTest, Ch_1_4_How_To_Launch_Thread_UT)
{
	// #include <thread>  // Include thread header

	std::cout << "foo() address: " << foo << '\n';

	// [1] Launch thread (free function)
	std::thread thread1(foo);  // fooMulti is free function
	std::thread thread2(foo);
	// do something in the main thread (in this case this test unit

	// join function will force main thread to wait (blocking) until thread execution is finished
	thread1.join();  // Blocking
	thread2.join();  // Blocking

	// Thread can be executed as a free function, lambda function and callable object …


	// [2] Call via callable object
	// Call via operator() overload
	callable_class obj;
	// obj();  // Standard operator call
	std::thread thread3(obj);
	thread3.join();


	// [3] Run thread via lambda expression
	// Using lambda expression
	std::thread thread4([]()
	{
	  std::cout << "lambda operator thead ID: " << std::this_thread::get_id() << '\n';
	});
	
	thread4.join();
}

// Ch 1 - [5] Excersise

// Ch 1 - [6] Joinability of threads
TEST_F(MultithreadingUnitTest, Ch_1_6_Joinability_of_threads_UT)
{

}

// Ch 1 - [7] Join and detach function
TEST_F(MultithreadingUnitTest, Ch_1_7__UT)
{

}

// Ch 1 - [8] How to handle join, in exception scenarios
TEST_F(MultithreadingUnitTest, Ch_1_8_How_to_handle_join_in_exception_scenarios_UT)
{

}


// Ch 1 - [9] Programming exercise: Trivial sale a ship model


// Ch 1 - [10] How to pass parameters to a thread
TEST_F(MultithreadingUnitTest, Ch_1_10_How_to_pass_parameters_to_a_thread_UT)
{

}