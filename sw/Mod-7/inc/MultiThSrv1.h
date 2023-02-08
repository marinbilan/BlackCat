#pragma once
#include "ServiceIf.h"

// Multithread stuff
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <future>
#include <numeric>
#include <functional>
#include <stdexcept>
#include <execution>
// #include <tbb/blocked_range.h>

// MODERN C++ CONCURENCY
/*
Section 3: Communication between thread using condition vars and futures
[28] Introduction to condition variables
[29] Details about condition variables
*/

bool have_i_arrived = false;
int total_distance = 10;
int distance_covered = 0;

std::condition_variable cv;
std::mutex m;


bool keep_moving()
{
  // Keep driving all the time
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    distance_covered++;

    if (distance_covered == 3)
    {
      std::cout << "[SEND MASTER NOTIFY] distance_covered is 3." << "\n";
      cv.notify_all();
    }

    if (distance_covered == 7)
    {
      std::cout << "[SEND MASTER NOTIFY] distance_covered is 7." << "\n";
      cv.notify_all();
    }

    std::cout << "Keep driving. Distance covered: " << distance_covered << " Total distance: " << total_distance << "\n";

    /*
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    distance_covered++;

    if (distance_covered == total_distance)
    {
      std::cout << "[SEND MASTER NOTIFY] distance_covered is equal to total_distance." << "\n";
      // cv.notify_one();
      cv.notify_all();
    }

    std::cout << "Keep driving. Distance covered: " << distance_covered << " Total distance: " << total_distance << "\n";
    */
  }

  return false;
}


void ask_driver_to_wake_u_up_at_right_time_first()
{
  std::cout << "Call from ask_driver_to_wake_u_up_at_right_time" << '\n';

  std::unique_lock<std::mutex> ul(m);
  // Attach to thread and wait for wake up from main thread
  cv.wait(ul, [] {
    std::cout << "Call from FIRST lambda" << '\n';
    return distance_covered == 3;
  });

  std::cout << "Finally (first) I am there (keep_awake_all_night), distance_covered = " << distance_covered << std::endl;
}


void ask_driver_to_wake_u_up_at_right_time_second()
{
  std::cout << "Call from ask_driver_to_wake_u_up_at_right_time" << '\n';

  std::unique_lock<std::mutex> ul(m);
  // Attach to thread and wait for wake up from main thread
  cv.wait(ul, [] {
    std::cout << "Call from SECOND lambda" << '\n';
    return distance_covered == 7;
  });

  std::cout << "Finally (second) I am there (keep_awake_all_night), distance_covered = " << distance_covered << std::endl;
}


// [30]
// [31] THREAD SAFE QUEUE
template<typename T>
class thread_safe_queue
{
public:

  thread_safe_queue()
  {
  };

  //thread_safe_queue(thread_safe_queue const& other_queue);

  void test()
  {
    std::cout << "Thread safe queue ..." << '\n';
  }

  void push(T value)
  {
    std::lock_guard<std::mutex> lg(m);

    // Queue is std queue - already existing in standard lib
    queue.push(std::make_shared<T>(value));
    cv.notify_one();
  }

  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lg(m);

    if(queue.empty())
    {
      return std::shared_ptr<T>();
    } 
    else 
    {
      std::shared_ptr<T> ref(queue.front());
      queue.pop();

      return ref;
    }
  }
  
  bool pop(T& ref)
  {
    // TODO: Implement pop(). Same as stack
    return true;
  }

  //void wait_pop(T& ref);
  
  /*

  */
  std::shared_ptr<T> wait_pop()
  {
    // Note unique_lock in this case
    std::unique_lock<std::mutex> lg(m);

    // After element is added in queue, check that
    // queue is not empty. If not empty keep on next 
    // statement.
    cv.wait(lg, [this] {
      return !queue.empty();
    });

  }


  bool empty()
  {
    std::lock_guard<std::mutex> lg(m);

    return queue.empty();
  }


private:
std::mutex m;
std::condition_variable cv;
std::queue<std::shared_ptr<T>> queue; 
};


// [32] Introduction to futures and async tasks
// #include <future>
int find_the_answer_how_old_universe()
{
  return 5000;
}


void do_other_calculations()
{
  std::cout << "Doing other stuff ..." << '\n';
}


// [33] Async tasks detailed discussion
void printing()
{
  std::cout << "Printing runs on - " << std::this_thread::get_id() << '\n';
}


int addition(int x, int y)
{
  std::cout << "Addition runs on - " << std::this_thread::get_id() << '\n';

  return x + y;
}


int substraction(int x, int y)
{
  std::cout << "Substraction runs on - " << std::this_thread::get_id() << '\n';

  return x - y;
}


// [34] Parallel accumulate algorithm implementation with async task
int MIN_ELEM_COUNT = 1000;

template<typename iterator>
int parallel_accumulate(iterator begin, iterator end)
{
  long length = std::distance(begin, end);

  // At least return 1000 elements - this is last call
  if(length < MIN_ELEM_COUNT)
  {
    std::cout << std::this_thread::get_id() << '\n';
    return std::accumulate(begin, end, 0);
  }

  iterator mid = begin;
  std::advance(mid, (length + 1) / 2);

  // Recursive all to parallel_accumulate
  // First sum from mid to end
  std::future<int> f1 = std::async(std::launch::async | std::launch::deferred,
    parallel_accumulate<iterator>, mid, end);

  // Second sum from begin to mid
  auto sum = parallel_accumulate(begin, mid);

  // Sum up everything
  return sum + f1.get();

  return 0;
}


// [35] Introduction to package_task
int add(int x, int y)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "Add function runs in: " << std::this_thread::get_id() << '\n';

  return x + y;
}


void task_thread()
{
  std::packaged_task<int(int,int)> task_1(add);
  std::future<int> future_1 = task_1.get_future();

  std::thread thread_1(std::move(task_1), 5, 6);
  thread_1.detach();

  std::cout << "Task thread- " << future_1.get() << '\n';
}


void task_normal()
{
  std::packaged_task<int(int,int)> task_1(add);
  std::future<int> future_1 = task_1.get_future();
  task_1(7, 8);

  std::cout << "Task normal - " << future_1.get() << '\n';
}


// [36] Communication between threads using std::promises
void print_int(std::future<int>& fut)
{
  std::cout << "Waiting for value from printing thread \n";
  std::cout << "Value: " << fut.get() << '\n';
  std::cout << "Next statement in print_int() " << '\n';
}


// [37] Retrieving exception using std::futures
void throw_exception()
{
  throw std::invalid_argument("Input must be positive");
}


void calculate_square_root(std::promise<int>& prom)
{
  int x = 1;
  std::cout << "Enter int:";

  try
  {
    std::cin >> x;

    if(x < 0)
    {
      throw_exception();
    }
    prom.set_value(std::sqrt(x));
  }
  catch(std::exception&)
  {
    prom.set_exception(std::current_exception());
  }
}


void print_result(std::future<int>& fut)
{
  try
  {
    int x = fut.get();
    std::cout << "Value: " << x << '\n';
  }
  catch(std::exception& e)
  {
    std::cout << "Exception caught: " << e.what() << '\n';
  }
}


// [38] std::shared_futures
void print_result2(std::shared_future<int>& fut)
{
  std::cout << fut.get() << " - valid future \n";
}


// --== CH 4 ==--
// [42] Parallel STL introduction

