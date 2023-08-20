#include <gmock/gmock.h>

// #include "TutorialIf.h"
#include "Multithreading.h"
#include "MultithreadingHelp.h"
#include "MultithreadingHelp1.h"
#include "MultithreadingHelp2.h"


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
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_4_How_To_Launch_Thread_UT)
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

	// Id's of all three threads will be the same because they are executing sequentially
	// if they join all at the end of method, thread's Id's would be different
}

// Ch 1 - [5] Excersise

// Ch 1 - [6] Joinability of threads
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_6_Joinability_of_threads_UT)
{
	std::thread thread0(foo);

	// Thread thread0 is joinable
	if(thread0.joinable())
	{
		std::cout << "Thread0 is joinable" << '\n';
	}

	thread0.join();
	// Thread thread0 is not joinable
}

// Ch 1 - [7] Join and detach function
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_7__UT)
{
	std::thread fooo_thread(fooo);
	std::thread bar_thread(bar);

	// This is not blocking calling (main) thread
	bar_thread.detach();
	std::cout << "This is after bar thread detach" << '\n';

	// This blocks main thread until child (fooo) thread finishes executions
	fooo_thread.join();
	std::cout << "This is after fooo thread join" << '\n';
}

// Ch 1 - [8] How to handle join, in exception scenarios
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_8_How_to_handle_join_in_exception_scenarios_UT)
{
	// How this works?
	/*
	When program runs, it will throw exception in Other operations, and main (current)
	thread will go out of scope.
	When main thread goes out of scope, objects which are created inside main thread will
	be destructed in reverse order that they are created.
	Before main destructor, destructor of thread_guard object will be called and destructor
	will call join function on foo_thread! So there is no any std::terminate call.
	*/
	std::thread foo_thread(foo);
	thread_guard tg(foo_thread);

	try
	{
		std::cout << "Throw exception in Other operations..." << '\n';
	} catch(...)
	{
	}
}


// Ch 1 - [9] Programming exercise: Trivial sale a ship model


// Ch 1 - [10] How to pass parameters to a thread
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_10_How_to_pass_parameters_to_a_thread_UT)
{
	int p = 9;
	int q = 8;

	std::thread thread_1(func_1, p, q);

	thread_1.join();

    // How to pass ref to function?
    int x = 9;
    // Always explicit note reference
    std::thread thread_2(func_2, std::ref(x));

    // Put some sleep to test that var is cahnged from main thread via std::ref
    x = 15;  // This will change value inside methdod

    thread_2.join();
}


// Ch 1 - [11] Problematic situations may arise when passing params to thread
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_11_Problematic_situations_passing_params_UT)
{
    // This will throw exception
    std::thread thread_1(func_11);
    thread_1.join();
}


// Ch 1 - [12] Transfering ownership of thread
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_12_Transfering_ownership_of_thread_UT)
{
	// 
    // 1st case - let's assign thread_1 to thread_2
    std::thread thread_1(foo12);

    // Not possible to use copy constructor / assignment operator
    // std::thread thread_2 = thread_1;

    // Move thread from one to another ok (Call move constructor)
    /*
    When we use std::move, move operator will transfer ownership of the tread which is owned by thread_1
    to take thread variable. Owner of the thread object is responsible for managing the thread object lifecycle.
    Important: After move operation - thread_1 does not own any thread object now. Also calling join method on thread_1
    will cause compilation error
    */
    std::thread thread_2 = std::move(thread_1);

    // rvalue on right side - implicit move call OK (thread_1 did not have any thread object)
    thread_1 = std::thread(bar12);

    // Move operation to object that already has thread object will cause error exception

    thread_1.join();
    thread_2.join();
}


// Ch 1 - [13] Some useful operations on thread
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_13_Some_useful_operations_on_thread_UT)
{
	// 
}


// Ch 1 - [14] Programming excersise
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_14_Programming_Excersise_UT)
{
	// 
}


// Ch 1 - [15] Parallel accumulate algorithm explanation
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_15_Parallel_accumulate_algorithm_explanation_UT)
{
	// 
    std::cout << "Number of cores: " << std::thread::hardware_concurrency() << '\n';
    sequential_accumulate_test();
}


// Ch 1 - [16] Parallel accumulate algorithm implementation
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_16_Parallel_accumulate_algorithm_implementation_UT)
{
    const int size = 8000;
    int* my_array = new int[size];
    int ref = 0;

    srand(0);

    for (size_t i = 0; i < size; i++)
    {
      my_array[i] = rand() % 10;
    }

    int rer_val = parallel_accumulate<int*, int>(my_array, my_array + size, ref);
    std::cout << "Accumulated val = " << rer_val << '\n';	
}


// Ch 1 - [17] Thread local storage
TEST_F(MultithreadingUnitTest, DISABLED_Ch_1_17_Thread_local_storage_UT)
{
    std::thread t1(foo17);
    std::thread t2(foo17);
    std::thread t3(foo17);

    t1.join();
    t2.join();
    t3.join();
}


// CHAPTER 2


// Ch 2 - [19] Introduction to locking mechanisms
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_19_Introduction_to_locking_mechanisms_UT)
{
}

// Ch 2 - [20] Concept of invariants
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_20_Concept_of_invariants_UT)
{
}

// Ch 2 - [21] Mutexes
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_21_Mutexes_UT)
{
	std::thread thread_1(add_to_list, 4);
	std::thread thread_2(add_to_list, 11);

	thread_1.join();
	thread_2.join();
}

// Ch 2 - [22] Things to remember when using Mutexes
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_22_Mutexes_Pitfalls_UT)
{
}

// Ch 2 - [23] Thread safe stack intro	
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_23_Thread_safe_stack_intro_UT)
{
}

// Ch 2 - [24] Thread safe stack impl
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_24_Thread_safe_stack_impl_UT)
{
}

// Ch 2 - [25] Thread safe stack impl
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_25_Thread_safe_stack_impl_UT)
{
}

// Ch 2 - [26] Deadlocks
TEST_F(MultithreadingUnitTest, DISABLED_Ch_2_26_Deadlocks_UT)
{
  bank_account account;

  bank_account account_1(1000, "James");
  bank_account account_2(2000, "Mathew");
 
  // Call method on concrete object - with args ...
  std::thread thread_1(&bank_account::transfer, &account, std::ref(account_1), std::ref(account_2), 500);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::thread thread_2(&bank_account::transfer, &account, std::ref(account_2), std::ref(account_1), 300);

  thread_1.join();
  thread_2.join();
}

// Ch 2 - [27] Uniquelock


// CHAPTER 3 - Communication Between Threads

// Ch 3 - [28] Introduction to condition variables
// Ch 3 - [29] Details Condition Variables
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_29_Details_Condition_Variables_UT)
{
 	std::thread driver_thread(keep_moving);
  	std::thread passenger_thread_first(ask_driver_to_wake_u_up_at_right_time_first);
  	std::thread passenger_thread_second(ask_driver_to_wake_u_up_at_right_time_second);

  	// Join all threads
  	passenger_thread_first.join();
  	passenger_thread_second.join();
  	driver_thread.join();
}

// Ch 3 - [30] Thread safe queue - Intro
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_30_Thread_Safe_Queue_Intro_UT)
{
}

// Ch 3 - [31] Thread safe queue - Impl
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_31_Thread_Safe_Queue_Impl_UT)
{
}

// Ch 3 - [32] Introduce to future and async tasks
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_32_Intro_to_futures_and_async_tasks_UT)
{
	// Main Thread (Creator)
	std::future<int> the_answer_future = std::async(find_the_answer_how_old_universe);

	do_other_calculations();

	std::cout << "The answer is: " << the_answer_future.get() << '\n';
}

// Ch 3 - [33] Async tasks detailed discussion
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_33_Async_detailed_UT)
{
	// Main Thread (Creator)
	std::cout << "Main Thread id - " << std::this_thread::get_id() << '\n';

	int x = 100;
	int y = 50;

	std::future<void> f1 = std::async(std::launch::async, printing);
	// Note that addition function runs on same thread as master thread
	/*
	deferred - will waint in main thread untill get() method is called and then it will be 
	run (in main thread).
	*/
	std::future<int> f2 = std::async(std::launch::deferred, addition, x, y);
	std::future<int> f3 = std::async(std::launch::async | std::launch::deferred, substraction, x, y);

	f1.get();
	std::cout << "Value received using f2 future - " << f2.get() << '\n';
	std::cout << "Value received using f3 future - " << f3.get() << '\n';
}


// Ch 3 - [34] Parallel accumulate with async task
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_34_Parralel_accumulate_async_UT)
{
	// Main Thread (Creator)
	// This is vector of 10000 - one's (1, 1, 1, 1, ...). Sum is 10000
	std::vector<int> v(10000, 1);

	std::cout << "Parallel accumulate: " << parallel_accumulate(v.begin(), v.end()) << '\n';
}


// Ch 3 - [35] Intro_to_package_task
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_35_Intro_to_package_task_UT)
{
	task_thread();
	task_normal();

	std::cout << "Main Thread id: " << std::this_thread::get_id() << '\n';
}


// Ch 3 - [36] Communication between threads using std::promises
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_36_Comm_using_promises_UT)
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


// Ch 3 - [37] Retrieving exception using std::futures
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_37_Exception_using_futures_UT)
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


// Ch 3 - [38] std::shared_futures
TEST_F(MultithreadingUnitTest, DISABLED_Ch_3_38_Shared_futures_UT)
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


// CHAPTER 4 - Intro to lock based thread safe data structures and algorithms
// Ch 4 - [39] Intro to locked based algos
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_39_Intro_to_lock_based_UT)
{
	// Thread safe Queue review
}


// Ch 4 - [40] - Queue data structure impl using linked list data structure
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_40_Queue_data_struct_impl_linked_list_UT)
{
	// Thread safe Queue review
}


// Ch 4 - [41] - Thread safe queue - Final
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_41_Thread_safe_queue_final_UT)
{
	// Thread safe Queue final implementation
}


// Ch 4 - [42] - Parallel STL introduction
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_42_Parallel_STD_Introduction_UT)
{
	// Thread safe Queue final implementation
}


// Ch 4 - [43] - Parallel Quick Sort
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_43_Parallel_QuickSort_UT)
{
  /*
  random_device rd;

  // Generate some radnom doubles:
  std::cout << "Testing doubles ..." << '\n';

  vector<double> doubles(testSize);
  for (auto& d : doubles)
  {
    d = static_cast<double>(rd());
  }	

  // Time how long does it take to sort them (Normal (not par) sort)
  vector<double> sorted_0(doubles);
  sort(sorted_0.begin(), sorted_0.end());

  // Time how long does it take to paraller sort)
  vector<double> sorted_1(doubles);
  sort(std::execution::par, sorted_1.begin(), sorted_1.end());
  */
}


// Ch 4 - [44] - Parallel for_each implementation
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_44_Parallel_for_each_impl_UT)
{

}


// Ch 4 - [45] - Parallel find algo implemented with package task
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_45_Parallel_Find_Package_task_UT)
{

}


// Ch 4 - [46] Parallel find algorithm implementation with async
// TODO

// Ch 4 - [47] Partial sum algorithm introduction - seq
// Just explanation


// Ch 4 - [48] Partial sum algorithm parallel impl
TEST_F(MultithreadingUnitTest, DISABLED_Ch_4_48_Partial_sum_algo_parallel_impl_UT)
{
	// TODO: Check experiment
}


// Ch 4 - [49] Introduction to Matrix
// Ch 4 - [50] Parallel Matrix multiplication
// Ch 4 - [51] Parallel Matrix transpose
// Ch 4 - [52] Factors Affecting the Performance of concurrent code


// -- CHAPTER 5 --


// -- CHAPTER 6 --
// Ch 6 - [59] Introduction to Atomic Operations

// Ch 6 - [60] Functionality of std::atomic_flag
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_60_atomic_flag_UT)
{
	// TODO: Check experiment
  // std::atomic_flag flag1 = true;
  std::atomic_flag flag2 = ATOMIC_FLAG_INIT;

  // Set the flag value to true and return previous value
  std::cout << "1. previous flag value: " << flag2.test_and_set() << '\n'; // previous value 0
  std::cout << "2. previous flag value: " << flag2.test_and_set() << '\n'; // previous value 1

  // Clear the flag value
  flag2.clear();
  std::cout << "3. previous flag value: " << flag2.test_and_set() << '\n'; // previous value 0
}


// Ch 6 - [61] Functionality of std::atomic_bool
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_61_atomic_bool_UT)
{
  // Pt 1

  std::atomic<bool> flag_1;
  std::cout << "flag 1 = " << flag_1 << '\n'; // flag_1 is true by default

  // Can not copy construct
  // std::atomic<bool> flag_2(flag_1);

  // Can not copy assignable
  // std::atomic<bool> flag_2 = flag_1;

  // Construct using non atomic boolean value
  /*
  // Check wyh does not work (Solution: Added C++ 20 falg)
  bool non_atomic_bool = true;

  std::atomic<bool> flag_4(non_atomic_bool);
  std::cout << "flag 4 = " << flag_4 << '\n';

   // Assign non atomic boolean value
  std::atomic<bool> flag_5 = non_atomic_bool;
  std::cout << "flag 5 = " << flag_5 << '\n';
  */

  // Pt 2
  
  std::atomic<bool> x(false);
  std::cout << "Atomic boolean is implemented lock free - " <<
    (x.is_lock_free() ? "yes" : "no") << '\n';

  std::atomic<bool> y(true);

  // Store operations (In other words - set value)
  x.store(false);
  x.store(y);

  // Load operations
  std::cout << "Value of the atomic bool is y: " << y.load() << '\n';

  // Exchange operation
  // Receive previous value of x and set to z
  // also, change current x value
  bool z = x.exchange(false);

  std::cout << "Current value of the atomic bool is x: " << x.load() << '\n';
  std::cout << "Previous value of the atomic bool is x: " << z << '\n';
  
}


// Ch 6 - [62] Explanation of compare_exchange functions
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_62_Compare_exchange_functions_UT)
{
  std::atomic<int> x(20);

  // Experiment 1
  int expected_value = 20;

  // Experiment 2
  // int expected_value = 10;

  std::cout << "Previous expected value - " << expected_value << '\n';
  bool return_val = x.compare_exchange_weak(expected_value, 6);

  std::cout << "Operation successful - " << (return_val ? "yes" : "No") << '\n';

  std::cout << "Current expected_value - " << expected_value << '\n';
  std::cout << "Current x - " << x.load() << '\n';
}


// Ch 6 - [63] Atomic Pointers
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_63_Atomic_Pointers_UT)
{
  int values[20];

  for (int i = 0; i < 20; i++)
  {
    values[i] = i + 1;
  }

  std::atomic<int*> x_pointer = values;
  std::cout << "Atomic integer pointer lock free ? "
      << (x_pointer.is_lock_free() ? "yes" : "no") << '\n';

  int* y_pointer = values + 3;

  x_pointer.store(y_pointer);
  std::cout << "Value referening to by pointer : " << *(x_pointer.load()) << '\n';
  bool ret_val = x_pointer.compare_exchange_weak(y_pointer, values + 10);

  std::cout << "Store operation sucessfull: " << (ret_val ? "yes" : "no") << '\n';
  std::cout << "New value pointer by atomic pointer : " << *x_pointer << '\n';

  /*
  // Experiment 2
  int values[20];

  for (int i = 0; i < 20; i++)
  {
    values[i] = i + 1;
  }
    std::atomic<int*> x_pointer = values;

    std::cout << "1. After initialization value pointer by atomic pointer - "
      << *x_pointer << '\n';

    // Fetch add
    int* prev_pointer_val_1 = x_pointer.fetch_add(12);

    std::cout << "2. After fetch add previous value pointed by atomic pointer - "
      << *prev_pointer_val_1 << '\n';
    std::cout << "2. After fetch add previous value pointed by atomic pointer - "
      << *x_pointer << '\n';

    // Fetch sub
    int* prev_pointer_val_2 = x_pointer.fetch_sub(3);

    std::cout << "3. After fetch sub previous value pointed by atomic pointer - "
      << *prev_pointer_val_2 << '\n';
    std::cout << "3. After fetch sub previous value pointed by atomic pointer - "
      << *x_pointer << '\n';

    // ++ operator
    x_pointer++;
    std::cout << "4. After post increment value pointed by atomic pointer - "
      << *x_pointer << '\n';

    // -- operator
    x_pointer--;
    std::cout << "5. After post decrement value pointed by atomic pointer - "
      << *x_pointer << '\n';
    */
}


// Ch 6 - [64] General discussion on atomic types

// Ch 6 - [65] Important relationships related to atomic operations between threads
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_65_Relationship_between_atomic_threads_UT)
{
  std::thread reader_thread(reader_func);
  std::thread writer_thread(writer_func);

  reader_thread.join();
  writer_thread.join();
}


// Ch 6 - [66] Introduction to Memory ordering options
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_66_Intro_to_memory_ordering_options_UT)
{
  std::atomic<int> x;
  x.store(5); // by default: std::memory_order_seq_cst

  x.store(10, std::memory_order_release);
  x.load(std::memory_order_acquire);

  int value = 11;
  bool ret_val = x.compare_exchange_weak(value, 13, std::memory_order_release, std::memory_order_relaxed);
}


// Ch 6 - [67] Discussion on memory_order_seq_cst
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_67_memory_order_seq_cst_UT)
{
  x = false;
  y = false;
  z = 0;

  /*
  We want this execution order. But in reality, order can be different
  because we did not specify order of execution. For example, read_y_then_x
  can be executed first what leads to undefined behavior.
  */
  std::thread thread_a(write_x);
  std::thread thread_b(write_y);
  std::thread thread_c(read_x_then_y);
  std::thread thread_d(read_y_then_x);

  thread_a.join();
  thread_b.join();
  thread_c.join();
  thread_d.join();

  // assert(z != 0);
  std::cout << "z = " << z << '\n';
}


// Ch 6 - [68] Introduction to instruction reordering
// Ch 6 - [69] Discussion on memory_order_relaxed
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_69_memory_order_relaxed_UT)
{
  /*
  x = false;
  y = false;
  z = 0;

  std::thread writer_thread(write_x_then_y);
  std::thread reader_thread(read_y_then_x_0);
  */
}


// Ch 6 - [70] Discussion on memory_order_acquire and memory_order_release
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_70_memory_order_aquire_release_UT)
{}


// Ch 6 - [71] Important aspects of memory_order_acquire and memory_order_release
// Ch 6 - [72] Concept of Transitive synchronization
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_72_Transitive_sync_UT)
{
  std::thread thread_1(thread_1_func);
  std::thread thread_2(thread_2_func);
  std::thread thread_3(thread_3_func);

  thread_1.join();
  thread_2.join();
  thread_3.join();

}


// Ch 6 - [73] Discussion on memory_order_consume
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_73_memory_order_consume_UT)
{
  //std::thread thread_1(thread_1_func);
  //std::thread thread_2(thread_2_func);
  //std::thread thread_3(thread_3_func);

  //thread_1.join();
  //thread_2.join();
  //thread_3.join();

}


// Ch 6 - [74] Concept of release sequence
TEST_F(MultithreadingUnitTest, DISABLED_Ch_6_74_release_sequence_UT)
{
  std::thread writer_threa(writer_queue);
  std::thread reader_threa(reader_queue);

}


// Ch 6 - [75] Implementation of spin lock mutex
TEST_F(MultithreadingUnitTest, Ch_6_75_spinlock_mutex_UT)
{

  //std::thread thread_1(thread_1_func);
  //std::thread thread_2(thread_2_func);
  //std::thread thread_3(thread_3_func);

  //thread_1.join();
  //thread_2.join();
  //thread_3.join();

  // Ch 6 - Item 74
  // std::thread writer_threa(writer_queue);
  // std::thread reader_threa(reader_queue);

  // Ch 6 - Item 75
  std::thread thread_1(func);
  std::thread thread_2(func);

  thread_1.join();
  thread_2.join();

  // assert(z != 0);

}