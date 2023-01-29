#include <gmock/gmock.h>
#include "ServiceIf.h"
//
#include "MultiThSrv0.h"
#include "MultiThSrv1.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class Mod_7_0_UnitTest : public ::testing::Test
{
public:

	Mod_7_0_UnitTest()
	{
		// 
	}

	~Mod_7_0_UnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_0)
{
	Service::MultiThSrv0 multiThSrv0("Test", "Test");
	multiThSrv0.preInit();
}


TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_1)
{
	std::thread driver_thread(keep_moving);
  	std::thread passenger_thread_first(ask_driver_to_wake_u_up_at_right_time_first);
  	std::thread passenger_thread_second(ask_driver_to_wake_u_up_at_right_time_second);

  	// Join all threads
  	passenger_thread_first.join();
  	passenger_thread_second.join();
  	driver_thread.join();
}


TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_2)
{
	thread_safe_queue<int> queueTest;
	queueTest.test();
}

// [32] Introduction to futures and async tasks
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_3)
{
	// Main Thread (Creator)
	std::future<int> the_answer_future = std::async(find_the_answer_how_old_universe);

	do_other_calculations();

	std::cout << "The answer is: " << the_answer_future.get() << '\n';
}


// [33] Async tasks detailed discussion
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_4)
{
	// Main Thread (Creator)
	std::cout << "Main Thread id - " << std::this_thread::get_id() << '\n';

	int x = 100;
	int y = 50;

	std::future<void> f1 = std::async(std::launch::async, printing);
	std::future<int> f2 = std::async(std::launch::deferred, addition, x, y);
	std::future<int> f3 = std::async(std::launch::async | std::launch::deferred, substraction, x, y);


	f1.get();
	std::cout << "Value received using f2 future - " << f2.get() << '\n';
	std::cout << "Value received using f3 future - " << f3.get() << '\n';
}


// [34] Parallel accumulate algorithm implementation with async task
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_5)
{
	// Main Thread (Creator)
	// This is vector of 10000 - one's (1, 1, 1, 1, ...). Sum is 10000
	std::vector<int> v(10000, 1);

	std::cout << "Parallel accumulate: " << parallel_accumulate(v.begin(), v.end()) << '\n';
	
}


// [35] Introduction to package_task
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_6)
{
	task_thread();
	task_normal();

	std::cout << "Main Thread id: " << std::this_thread::get_id() << '\n';

}


// [36] Communication between threads using std::promises
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_7)
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	// Creating new thread which will execute printing function
	std::thread print_thread(print_int, std::ref(fut));

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "Setting the value in main thread" << '\n';

	prom.set_value(10);

	print_thread.join();
}


// [37] Retrieving exception using std::futures
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_8)
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	// Creating new thread which will execute printing function
	// Here, we are waiting (get()) until calculation thread is ready to
	// set result (future) - when result is set, from get() line will 
	// execute further
	std::thread printing_thread(print_result, std::ref(fut));

	std::thread calculating_thread(calculate_square_root, std::ref(prom));

	printing_thread.join();
	calculating_thread.join();
}


// [38] std::shared_futures
TEST_F(Mod_7_0_UnitTest, DISABLED_Mod_7_Test_9)
{
	std::promise<int> prom;
	// std::shared_future<int> fut = prom.get_future();
	std::shared_future<int> fut(prom.get_future());

	std::thread th1(print_result2, std::ref(fut));
	std::thread th2(print_result2, std::ref(fut));

	prom.set_value(5);

	th1.join();
	th2.join();
}


// [42] Parallel STL introduction
TEST_F(Mod_7_0_UnitTest, Mod_7_Test_10)
{

}