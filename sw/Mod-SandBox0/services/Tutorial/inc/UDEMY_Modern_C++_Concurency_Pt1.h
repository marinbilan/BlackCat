#include <thread>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <atomic>
// Ch 2
#include <mutex>
#include <list>
#include <stack>
// Ch 3
#include <condition_variable>
#include <queue>
#include <future>


// Ch 3 - [28] Intro Condition Variables
// Ch 3 - [29] Details Condition Variables
bool have_i_arrived = false;
int total_distance = 10;
int distance_covered = 0;

std::condition_variable cv;
// std::mutex m;  // Already defined in another h help file


bool keep_moving()
{
  // Keep driving all the time
  while (distance_covered < 13)
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


// Ch 3 - [30] Thread safe queue introdu
// Ch 3 - [31] Thread safe queue - Impl
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
    // Avoid exception case creating shared ptr
    queue.push(std::make_shared<T>(value));
    cv.notify_one();
  }

  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lg(m);

    // Avoid race condition
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


// Ch 3 - [32] Introduce to future and async tasks
int find_the_answer_how_old_universe()
{
  return 5000;
}


void do_other_calculations()
{
  std::cout << "Doing other stuff ..." << '\n';
}


// Ch 3 - [33] Async tasks detailed discussion
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


// Ch 3 - [34] Parallel accumulate with async task
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


// Ch 3 - [35] Intro_to_package_task
int add(int x, int y)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "Add function runs in: " << std::this_thread::get_id() << '\n';

  return x + y;
}


void task_thread()
{
  // Wrap add method in package_task task_1
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

  // Start task_1 manually
  task_1(7, 8);

  std::cout << "Task normal - " << future_1.get() << '\n';
}


// Ch 3 - [36] Communication between threads using std::promises
void print_int(std::future<int>& fut)
{
  std::cout << "Waiting for value from printing thread \n";
  std::cout << "Value: " << fut.get() << '\n';
  std::cout << "Next statement in print_int() " << '\n';
}


// Ch 3 - [37] Retrieving exception using std::futures
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


// Ch 3 - [38] std::shared_futures
void print_result2(std::shared_future<int>& fut)
{
  std::cout << fut.get() << " - valid future \n";
}


// CHAPTER 4 - Intro to lock based thread safe data structures and algorithms
// Thread safe Queue review

// Ch 4 - [39] Intro to locked based algos
// Ch 4 - [40] - Queue data structure impl using linked list data structure
template<typename T>
class sequential_queue
{
  
  struct node
  {
    T data;
    std::unique_ptr<node> next;

    node(T _data) : data(std::move(_data))
    {

    }
  };
  
  std::unique_ptr<node> head;
  node* tail;
  std::mutex head_mutex;
  std::mutex tail_mutex;

public:

  sequential_queue() : head(new node), tail(head.get())
  {}

/*
// old push

  void push(T value)
  {
    std::unique_ptr<node> new_node(new node(std::move(value)));
    node* const new_tail = new_node.get();

    std::lock_guard<std::mutex> tlg(tail_mutex);
    if(tail)
    {
      tail->next = std::move(new_node);
    }
    else
    {
      std::lock_guard<std::mutex> hlg(head_mutex);
      head = std::move(new_node);
    }
    tail = new_tail;
  }
*/
  // New push
  void push(T value)
  {
    // Change current dummy nodes data value
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
    tail->data = new_data;

    // Add new dummy node to tail
    std::unique_ptr<node> p(new node);
    node* const new_tail = p.get();
    tail->next = std::move(p);
    tail = new_tail;
  }


/*
// Old pop
  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> hlg(head_mutex);

    if(!head)
    {
      return std::shared_ptr<T>();
    }

    std::shared_ptr<T> const res(std::make_shared<T>(std::move(head->data)));
    std::unique_ptr<node> const old_head = std::move(head);

    head = std::move(old_head->next);

    return res;
  }
*/
  // New pop
  std::shared_ptr<T> pop()
  {
    
    if(head.get() == tail)
    {
      return std::shared_ptr<T>();
    }

    std::shared_ptr<T> const res(head->data);
    std::unique_ptr<node> const old_head = std::move(head);
    head = std::move(old_head->next);

    return res;
  }
};


// Ch 4 - [41] - Thread safe queue - Final
template<typename T>
class sequential_queue_final
{
  
  struct node
  {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;

    node()
    {}

    node(T _data) : data(std::move(_data))
    {}

  };
  
  std::unique_ptr<node> head;
  node* tail;

  std::mutex head_mutex;
  std::mutex tail_mutex;

  std::condition_variable cv;

  node* get_tail()
  {
    std::lock_guard<std::mutex> lg(tail_mutex);
    return tail;
  }

public:

  sequential_queue_final() : head(new node), tail(head.get())
  {}


  // Final push
  void push(T value)
  {
    // Change current dummy nodes data value
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
    std::unique_ptr<node> p(new node);
    node* const new_tail = p.get();

    {
      std::lock_guard<std::mutex> lgt(tail_mutex);
      tail->data = new_data;
      tail->next = std::move(p);
      tail = new_tail;
    }
    
    cv.notify_one();
  }


  // Final pop
  std::shared_ptr<T> pop()
  {
    
    if(head.get() == tail)
    {
      return std::shared_ptr<T>();
    }

    std::shared_ptr<T> const res(head->data);
    std::unique_ptr<node> const old_head = std::move(head);
    head = std::move(old_head->next);

    return res;
  }
};


// Ch 4 - [42] - Parallel STL introduction
// Ch 4 - [43] - Parallel Quick Sort
// Set values for par execution STL algorithms
const size_t testSize = 1'000'000;
const int iterationCount = 5;


// Quicksort - Sequential
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
  // Recursive return condition
  if (input.size() < 2)
  {
    return input;
  }

  // Select the pivot value
  std::list<T> result;
  // Transfer elements from one list to another
  // In this case - store in new list first element from input list
  // That is pivot - it's first element not middle one (that is choice)
  // Note that pivot element is removed from original list
  result.splice(result.begin(), input, input.begin());
  T pivot = *result.begin();


  // Arrange the input array
  // Now we arange array that all elemets lower then pivot put
  // on the left side of list and all elements greater than pivot put
  // on the right side of list
  auto divide_point = std::partition(input.begin(), input.end(),
    [&](T const& t)
  {
    // This will return pointer of the middle elem (divide point)
    return t < pivot;
  });


  // Call the sequential_quick_sort recursively
  std::list<T> lower_list;
  /*
  After this step lower part of origin input list is in new list and upper
  part is remain in input array
  */
  lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

  // Now we can call recureslivy for each part
  auto new_lower(sequential_quick_sort(std::move(lower_list)));
  auto new_upper(sequential_quick_sort(std::move(input)));  // Upper part is remain in input array


  // Arraging the result list
  result.splice(result.begin(), new_lower);
  result.splice(result.end(), new_upper);

  return result;
}


// Quicksort - Parallel
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
  // Recursive return condition
  if (input.size() < 2)
  {
    return input;
  }

  // Select the pivot value
  std::list<T> result;
  // Transfer elements from one list to another
  // In this case - store in new list first element from input list
  // That is pivot - it's first element not middle one (that is choice)
  // Note that pivot element is removed from original list
  result.splice(result.begin(), input, input.begin());
  T pivot = *result.begin();

  // Arrange the input array
  // Now we arange array that all elemets lower then pivot put
  // on the left side of list and all elements greater than pivot put
  // on the right side of list
  auto divide_point = std::partition(input.begin(), input.end(),
    [&](T const& t)
  {
    // This will return pointer of the middle elem (divide point)
    return t < pivot;
  });

  // Call the sequential_quick_sort recursively
  std::list<T> lower_list;
  /*
  After this step lower part of origin input list is in new list and upper
  part is remain in input array
  */
  lower_list.splice(lower_list.end(), input, input.begin(), divide_point);


  // **** CHANGES FOR PARRALEL EXECUTION ****
  // One recursive call make as async task
  // Now we can call recureslivy for each part
  auto new_lower(parallel_quick_sort(std::move(lower_list)));
 
  // auto new_upper(sequential_quick_sort(std::move(input)));  // Upper part is remain in input array
  std::future<std::list<T>> new_upper_future(std::async(&parallel_quick_sort<T>, std::move(input)));

  // Arraging the result list
  result.splice(result.begin(), new_lower);
  result.splice(result.end(), new_upper_future.get());

  return result;
}


// Ch 4 - [44] - Parallel for_each implementation
class join_threads
{
  std::vector<std::thread>& threads;

public:
  explicit join_threads(std::vector<std::thread>& threads_) :
    threads(threads_)
  {}
  ~join_threads()
  {
    for (unsigned long i = 0; i < threads.size(); ++i)
    {
      if (threads[i].joinable())
        threads[i].join();
    }
  }
};


//
template<typename Iterator, typename Func>
void parallel_for_each_pt(Iterator first, Iterator last, Func f)
{
  unsigned long const length = std::distance(first, last);

  if (!length)
    return;

  // Calculate the optimum number of threads to run algorithm
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;

  //
  std::vector<std::future<void>> futures(num_threads - 1);
  std::vector<std::thread> threads(num_threads - 1);

  join_threads joiner(threads);

  //
  Iterator block_start = first;
  for (unsigned long i = 0; i <= (num_threads - 2); i++)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);

    std::packaged_task<void(void)> task(
      [=]()
    {
      std::for_each(block_start, block_end, f);
    });

    futures[i] = task.get_future();
    futures[i] = std::thread(std::move(task));

    block_size = block_end;
  }

  // Call the function for last block from this thread
  std::for_each(block_start, last, f);

  // Wait until futures are ready
  for (unsigned long i = 0; i < (num_threads - 1); i++)
  {
    futures[i].get();
  }

} // end



// Paralel for_each async
//
template<typename Iterator, typename Func>
void parallel_for_each_async(Iterator first, Iterator last, Func f)
{
  // Using recursive call - we need first
  unsigned long const length = std::distance(first, last);

  if (!length)
    return;

  unsigned long const min_per_thread = 25;

  if (length < 2 * min_per_thread)
  {
    std::for_each(first, last, f);
  }

  else
  {
    Iterator const mid_point = first + length / 2;

    std::future<void> first_half =
      std::async(&parallel_for_each_async<Iterator, Func>, first, mid_point, f);

    parallel_for_each_async(mid_point, last, f);
    first_half.get();
  }

}


// Ch 4 - [45] - Parallel find algo implemented with package task
//
template<typename Iterator, typename MatchType>
void parallel_find_pt(Iterator first, Iterator last, MatchType match)
{
  struct find_element
  {
    void operator()(Iterator begin, Iterator end, MatchType match, std::promise<Iterator>* result, std::atomic<bool>* done_flag)
    {
      try
      {
        for (; (begin != end) && !std::atomic_load(done_flag); ++begin)
        {
          if (*begin == match)
          {
            result->set_value(begin);
            std::atomic_store(done_flag, true);
            return;
          }
        }
      }
      catch (...)
      {
        result->set_exception(std::current_exception());
        done_flag->store(true);
      }
    }
  };

  unsigned long const length = std::distance(first, last);

  if (!length)
    return last;

  // Calculate the optimum number of threads to run algorithm
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;

  //
  std::promise<Iterator> result;
  std::atomic<bool> done_flag(false);

  //
  std::vector<std::thread> threads(num_threads - 1);

  {
    join_threads joiner(threads);
   
    // Task dividing loop
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); i++)
    {
      Iterator block_end = block_start;
      std::advance(block_end, block_size);

      // Need to launch threads with tasks
      threads[i] = std::thread(find_element(), block_start, block_end, match, &result, &done_flag);

      block_start = block_end;
    }

    // Perform the find operation for final block in this thread
    find_element()(block_start, last, match, &result, &done_flag);
  }

  if(!done_flag.load())
  {
    return last;
  }

  return result.get_future().get();
} // end


// Ch 4 - [46] Parallel find algorithm implementation with async
// TODO

// Ch 4 - [47] Partial sum algorithm introduction - seq
// Just explanation


// Ch 4 - [48] Partial sum algorithm parallel impl
template<typename Iterator, typename OutIterator>
void parallel_partial_sum(Iterator first, Iterator last)
{
  typedef typename Iterator::value_type value_type;

  struct process_chunk
  {
    void operator()(Iterator begin, Iterator last, std::future<value_type>* previous_end_value, std::promise<value_type>* end_value)
    {
      try
      {
        Iterator end = last;
        ++end;
        std::partial_sum(begin, end, begin);

        if (previous_end_value)
        {
          // This is not the first thread
          auto addend = previous_end_value->get();
          *last += addend;

          if (end_value)
          {
            // Not the last block
            end_value->set_value(*last);
          }

          std::for_each(begin, last, [addend](value_type& item)
          {
            item += addend;
          });
        }
        else if (end_value)
        {
          // This is first thread
          end_value->set_value(*last);
        }
      }
      catch (...)
      {
        if (end_value)
        {
          end_value->set_exception(std::current_exception());
        }
        else
        {
          // Final block - Main thread is the one process the final block
          throw;
        }
      }
    }
  };

  //
  unsigned long const length = std::distance(first, last);

  if (!length)
    return;

  // Calculate the optimum number of threads and block size to run algorithm
  // This is usually the same
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;

  //
  std::vector<std::thread> threads(num_threads - 1);
  std::vector<std::promise<value_type>> end_values(num_threads - 1);
  std::vector<std::future<value_type>> previous_end_values;
  previous_end_values.reserve(num_threads - 1);

  join_threads joiner(threads);

  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); i++)
  {
    Iterator block_last = block_start;
    std::advance(block_last, block_size - 1);

    threads[i] = std::thread(process_chunk(), block_start, block_last, (i != 0) ? previous_end_values[i - 1] : 0, &end_values[i]);

    block_start = block_last;
    ++block_start;
    previous_end_values.push_back(end_values[i].get_future());
  }

}


// Ch 4 - [49] Introduction to Matrix
// Ch 4 - [50] Parallel Matrix multiplication
// Ch 4 - [51] Parallel Matrix transpose
// Ch 4 - [52] Factors Affecting the Performance of concurrent code