#include <thread>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <atomic>
// Ch 2
#include <mutex>
#include <list>
#include <stack>


// Ch 1 - [1] Setting Up the Environment
// Ch 1 - [2] Introduction to Parallel Computing
// Ch 1 - [3] Parallel Programming in General (Heterogeneous Computing)

// Ch 1 - [4] How to Launch Thread

// [1] Run Thread as Regular Function
void foo() 
{
	std::cout << "Thread from foo() regular function" << '\n';
}


// [2] Run Thread via Funtion Operator ()
class callable_class
{
public:
  void operator()()
  {
    std::cout << "callable_class operator thead ID: " << std::this_thread::get_id() << '\n';
  }
};

// [3] Run Thread as Lambda Expression

// [4] Run Thread as Class Member Function (Instantiated class)
class CFoo {
  public:
    int m_i = 0;

    void bar() 
    {
      std::cout << "Increase m_i" << '\n';
      ++m_i;
    }
};


// [5] Return Lambda Member Function (1st way)
class Wrapper_0 {
public:
    void member1() 
    {
        std::cout << "I am member1 (Wrapper_0)" << std::endl;
    }

    void member2(const char *arg1, unsigned arg2) 
    {
        std::cout << "I am member2 (Wrapper_0) and my first arg is (" << arg1 << ") and second arg is (" << arg2 << ")" << std::endl;
    }

    std::thread member1Thread() 
    {
        return std::thread([=] { member1(); });
    }

    std::thread member2Thread(const char *arg1, unsigned arg2) 
    {
        return std::thread([=] { member2(arg1, arg2); });
    }
};


// [6] Return Lambda Member Function (2nd way)
class Wrapper_1 {
  public:
      void member1() {
          std::cout << "I am member1 (Wrapper_1)" << std::endl;
      }
      void member2(const char *arg1, unsigned arg2) {
          std::cout << "I am member2 (Wrapper_1) and my first arg is (" << arg1 << ") and second arg is (" << arg2 << ")" << std::endl;
      }
      std::thread member1Thread() {
          return std::thread(&Wrapper_1::member1, this);
      }
      std::thread member2Thread(const char *arg1, unsigned arg2) {
          return std::thread(&Wrapper_1::member2, this, arg1, arg2);
      }
};


// [7] Spawns n Threads
void doSomething(int id) 
{
    std::cout << "Spawns n Threads. Id: " << id << "\n";
}

void spawnThreads(int n)
{
    std::vector<std::thread> threads(n);
    // spawn n threads:
    for (int i = 0; i < n; i++) 
    {
        threads[i] = std::thread(doSomething, i + 1);
    }

    for (auto& th : threads) 
    {
        th.join();
    }
}

// Ch 1 - [5] Excersise
// Ch 1 - [6] Joinability of threads


// Ch 1 - [7] Join and detach function
void fooo() 
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "Hello from fooo" << '\n';
}

void bar()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "Hello from bar" << '\n';
}


// Ch 1 - [8] How to handle join, in exception scenarios
class thread_guard
{
public:
  explicit thread_guard(std::thread& _t) : t(_t)
  {

  }

  ~thread_guard()
  {
    // If thread is not already join or detach
    if (t.joinable())
    {
      t.join();
    }
  }

  thread_guard(thread_guard&) = delete;
  thread_guard& operator=(thread_guard&) = delete;

private:
  std::thread& t;
};


// Ch 1 - [9] Programming exercise: Trivial sale a ship model
// Ch 1 - [10] How to pass parameters to a thread
void func_1(int x, int y)
{
	std::cout << "Sum of x + y = " << x + y << '\n';
}

void func_2(int& x)
{
  /*
  while (true)
  {
    std::cout << "Thread 1 value of x : " << x << '\n';
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }
  */
}


// Ch 1 - [11] Problematic situations may arise when passing params to thread
void func_22(int& x)
{
  while (true)
  {
    try {
      std::cout << x << '\n';
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    catch (...)
    {
      throw std::runtime_error("This is a runtime error");
    }
  }
}

void func_11()
{
  int x = 5;
  std::thread thread_2(func_22, std::ref(x));
  thread_2.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  std::cout << "thread_11 finished execution \n";
}


// Ch 1 - [12] Transfering ownership of thread
void foo12() {}
void bar12() {}


// Ch 1 - [13] Some useful operations on thread
// Ch 1 - [14] Programming excersise
// Ch 1 - [15] Parallel accumulate algorithm explanation
void sequential_accumulate_test()
{
  std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  // Accumulation is general operation (depends on predicate)
  // sum (default), product ...
  int sum = std::accumulate(v.begin(), v.end(), 0);
  // Start multiply from 1 ...
  int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

  std::cout << "Sum: " << sum << '\n';
  std::cout << "Product: " << product << '\n';

  // Let's show all elems separated by dash (use lambda)
  // int to string
  /*
  We need two args for this lambda - first for already accumulated string and second for integer from the vector,
  then we can pass this lambda expression as the predicate for accumulate algorithm
  */
  auto dash_fold = [](std::string a, int b)
  {
    return std::move(a) + "-" + std::to_string(b);
  };

  /*
  Accumulate (same as reduce, fold, agregate ...) :
  ----
 

  First lambda argument
  (string a) - Start accumulation from (first elem to string)- std::to_string(v[0])
  First arg in lambda is value accumulator

  Second lambda argument:
  (int b)
  From second element - std::next(v.begin())
  Up to last element  - v.end()
  */
  std::string s = std::accumulate(std::next(v.begin()), v.end(), std::to_string(v[0]), dash_fold);

  /*
  In first iteration return value: string(1)-2
  Second iteration return value: string(1)-string(2)-3
  ...
  */

  std::cout << "Dash fold: " << s << '\n';
}


// Ch 1 - [16] Parallel accumulate algorithm implementation
/*
Divide dataset in different data chunks - Block size
Important - Block size needs to be big enough because we can slow down exec time

Algorithm takes iterator and data type as parameters
Return summation
*/

#define MIN_BLOCK_SIZE 1000

// Helper method
template<typename iterator, typename T>
void accumulate(iterator start, iterator end, T &ref)
{
  ref = std::accumulate(start, end, 0);
}


template<typename iterator, typename T>
T parallel_accumulate(iterator start, iterator end, T &ref)
{
  // First calculate number of threads depending on input dataset
  // Find nubmer of elems in input dataset
  int input_size = std::distance(start, end);

  int allowed_threads_by_elements = (input_size) / MIN_BLOCK_SIZE;

  // Also, we need hardware max num of threads, because we can have 1000 parallel threads according above formula
  int allowed_threads_by_hardware = std::thread::hardware_concurrency();

  // Number of threads is min of those two vals
  int num_threads = std::min(allowed_threads_by_elements, allowed_threads_by_hardware);

  // Calculate block size
  int block_size = (input_size + 1) / num_threads;

  // Vector of result for each block/thread
  std::vector<T> results(num_threads);
  // Vector of threads - (-1 - because last accumulation is in calling thread)
  std::vector<std::thread> threads(num_threads - 1);

  // Last element of each data block
  iterator last;

  // Launch threads with correct data blocks
  for (int i = 0; i < num_threads - 1; i++)
  {
    last = start;
    std::advance(last, block_size);

    threads[i] = std::thread(accumulate<iterator, T>, start, last, std::ref(results[i]));

    start = last;
  }

  results[num_threads - 1] = std::accumulate(start, end, 0);

  // Iterate via thread's vectora and call join function
  // At this point all treads (block) accumulation results are stored in result vector
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  // Now we can call accumulate on last vector to get result
  return std::accumulate(results.begin(), results.end(), ref);
}


// Ch 1 - [17] Thread local storage
//std::atomic<int> i = 0;
// Each trhead will have local i
//thread_local std::atomic<int> i = 0;

void foo17()
{
  //++i;
  //td::cout << i;
}



// TODO: Add rest items from Ch 1

// CHAPTER 2

// Ch 2 - [19] Introduction to locking mechanisms
// Ch 2 - [20] Concept of invariants

// Ch 2 - [21] Mutexes
std::list<int> my_list;
std::mutex m;

/*
Important to understand: In both methods
add_to_list and size - we use the same mutex
It two or more threads try to access my_list,
one thread will can access because of one mutex m.
*/

void add_to_list(int const& x)
{
  // [Ex 1] Explicit lock/unlock
  /*
  m.lock();
  my_list.push_front(x);
  m.unlock();  // Important to release because all threads will be blocked
  */

  // [Ex 2] lock/unlock using lock_guard
  std::lock_guard<std::mutex> lg(m);
  my_list.push_back(x);
}

void size()
{
  /*
  m.lock();
  int size = my_list.size();
  m.unlock();  // Important to release
  */

  // [Ex 2]
  std::lock_guard<std::mutex> lg(m);
  int size = my_list.size();

  std::cout << "Size of the list is : " << size << '\n';
}


// Ch 2 - [22] Things to remember when using Mutexes
class list_wrapper
{
public:
  void add_to_list(int const& x)
  {
    // lock/unlock using lock_guard
    std::lock_guard<std::mutex> lg(m);
    my_list.push_back(x);
  }

  void size()
  {
    std::lock_guard<std::mutex> lg(m);
    int size = my_list.size();
    std::cout << "Size of the list is : " << size << '\n';
  }

  // List not thread safe because caller can do anything with list
  std::list<int>* get_data()
  {
    return &my_list;  // return ptr to protected list
  }

private:
std::list<int> my_list;
std::mutex m;
};


// Ch 2 - [23] Thread safe stack intro  
// Ch 2 - [24] Thread safe stack implementation: Implementation
template<typename T>
class trivial_thread_safe_stack
{
public:
  void push(T element)
  {
    std::lock_guard<std::mutex> lg(m);
    stk.push(element);
  }

  void pop(T element)
  {
    std::lock_guard<std::mutex> lg(m);
    stk.pop(element);
  }

  T& top()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.top();
  }

  bool empty()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.empty();
  }

  size_t size()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.size();
  }

private:
  std::stack<T> stk;
  std::mutex m;
};


// Ch 2 - [25] Thread safe stack implementation: Race condition inherit from the interface
/*
T combine_pop_top()
{
  node* old_head = head;
  head = old_head->next;
  T value = old_head->data;
  delete old_head;
  return value;
}
*/

template<typename T>
class thread_safe_stack
{
public:
  void push(T element)
  {
    std::lock_guard<std::mutex> lg(m);
    // stk.push(element);
    // Push created shared pointer instead of value
    stk.push(std::shared_ptr<T>(element));
  }

  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lg(m);
    // stk.pop(element);

    // We want to avoid race condition between pop and empty functions
    if (stk.empty())
    {
      throw std::runtime_error("Stack is empty");
    }

    // Create new shared ptr using top function
    // Note that our stack stores shared pointers instead of elements
    std::shared_ptr<T> res(stk.top());
    stk.pop();

    // Return can not throw - shared pointer already created
    return res;
  }

  // Another pop function
  void pop(T& value)
  {
    std::lock_guard<std::mutex> lg(m);
    if (stk.empty()) throw std::runtime_error("Stack is empty!");
    value = *(stk.top().get());

    stk.pop();
  }

  T& top()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.top();
  }

  bool empty()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.empty();
  }

  size_t size()
  {
    std::lock_guard<std::mutex> lg(m);
    return stk.size();
  }

private:
  // std::stack<T> stk;
  // Instead of raw elements stack, stack of shared pointers of elements
  std::stack<std::shared_ptr<T>> stk;  
  std::mutex m;
};


// Ch 2 - [26] Deadlocks

class bank_account
{
public:
  bank_account() {};

  bank_account(double _balance, std::string _name) : balance(_balance), name(_name) {}

  // delete copy/assign
  void withdraw(double amount)
  {
    std::lock_guard<std::mutex> lg(m);
    balance += amount;  // -?
  }

  void deposite(double amount)
  {
    std::lock_guard<std::mutex> lg(m);
    balance += amount;
  }

  void transfer(bank_account& from, bank_account& to, double amount)
  {
    std::lock_guard<std::mutex> lg_1(from.m);
    std::cout << "lock for " << from.name << '\n';

    // Observe the outcome clearly
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "waiting to acquire lock for  " << from.name << " From thread: ..." << '\n';
    std::lock_guard<std::mutex> lg_2(to.m);

    from.balance -= amount;
    to.balance += amount;
    std::cout << "transfer to - " << to.name << " from - " << from.name << " end \n";
  }

private:
  double balance;
  std::string name;
  std::mutex m;
};