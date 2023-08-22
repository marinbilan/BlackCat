#pragma once
#include <atomic>


// Ch 6 - [60] Functionality of std::atomic_flag
/*
Output:
1. previous flag value: 0
2. previous flag value: 1
3. previous flag value: 0

First return value and set inverse value.
In first case 0 > set 1
Second 1 > set 0
...
clear will (re)set value to 0
*/


// Ch 6 - [61] Functionality of std::atomic_bool
// Ch 6 - [62] Explanation of compare_exchange functions
// Ch 6 - [63] Atomic Pointers

// Ch 6 - [64] General discussion on atomic types
// Ch 6 - [65] Important relationships related to atomic operations between threads
std::atomic<bool> data_ready = false;
std::vector<int> data_vector;

void reader_func()
{
  /*
  In reading thread this while loop needs to be executed before
  first elem of vector is printed
  This is Happen_before relationship
  We know that data_ready flag is init false and it is set only in writer_func (thread)
  This is syncronization between reading and writing thread - Inter-thread-happen-before relationship
  Very important relationship
  */
  while (!data_ready)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  std::cout << data_vector[0] << '\n';
}

void writer_func()
{
  /*
  We know for sure that pushing_back happened before setting flag to true
  This kind of situation is Happen_before relationship
  */
  data_vector.push_back(3);
  data_ready.store(true);
}

/*
void run_code_6()
{
  std::atomic<int> x;
  x.store(5); // by default: std::memory_order_seq_cst

  x.store(10, std::memory_order_release);
  x.load(std::memory_order_acquire);

  int value = 11;
  bool ret_val = x.compare_exchange_weak(value, 13, std::memory_order_release, std::memory_order_relaxed);
}
*/


// Ch 6 - [67] Discussion on memory_order_seq_cst
std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
  x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
  y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
  // Loop until x is true
  while (!x.load(std::memory_order_seq_cst));

  // Check wether y is true
  if (y.load(std::memory_order_seq_cst))
  {
    z++;
  }
}

void read_y_then_x()
{
  // Loop until y is true
  while (!y.load(std::memory_order_seq_cst));

  // Check wether x is true
  if (x.load(std::memory_order_seq_cst))
  {
    z++;
  }
}


// Ch 6 - [68] Introduction to instruction reordering
// Ch 6 - [69] Discussion on memory_order_relaxed
// Already defined above
//std::atomic<bool> x, y;  // Init to false
//std::atomic<int> z;  // Init to 0

void write_x_then_y()
{
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}


void read_y_then_x_0()
{
  while (!y.load(std::memory_order_relaxed));
  if (x.load(std::memory_order_relaxed))
  {
    z++;
  }
}


// Ch 6 - [70] Discussion on memory_order_acquire and memory_order_release
// Ch 6 - [71] Important aspects of memory_order_acquire and memory_order_release
// Ch 6 - [72] Concept of Transitive synchronization
std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);


void thread_1_func()
{
  data[0].store(42, std::memory_order_relaxed);
  data[1].store(45, std::memory_order_relaxed);
  data[2].store(47, std::memory_order_relaxed);
  data[3].store(49, std::memory_order_relaxed);
  data[4].store(56, std::memory_order_relaxed);

  sync1.store(true, std::memory_order_release);
}


void thread_2_func()
{
  while (!sync1.load(std::memory_order_acquire));
  sync2.store(true, std::memory_order_release);
}


void thread_3_func()
{
  while (!sync2.load(std::memory_order_acquire));

  assert(data[0].load(std::memory_order_relaxed) == 42);
  assert(data[1].load(std::memory_order_relaxed) == 45);
  assert(data[2].load(std::memory_order_relaxed) == 47);
  assert(data[3].load(std::memory_order_relaxed) == 49);
  assert(data[4].load(std::memory_order_relaxed) == 56);
}


// Ch 6 - [73] Discussion on memory_order_consume
struct X
{
  int i;
  std::string s;
};

std::atomic<X*> p;
std::atomic<int> a;


void create_x()
{
  X* x = new X;
  x->i = 42;
  x->s = "hello";

  a.store(20, std::memory_order_relaxed);
  p.store(x, std::memory_order_release);
}


void use_x()
{
  X* x;
  while (!(x = p.load(std::memory_order_consume)));
  assert(x->i == 42);
  assert(x->s == "hello");

  // This can fail because, there is no guarantie that use thread sees this value
  /*
  In case where only need sync is only depend statements, you can use memory_order_consume
  instead of memory_order_release
  */
  assert(a.load(std::memory_order_relaxed) == 20);
}


// Ch 6 - [74] Concept of release sequence
std::atomic<int> count;
std::queue<int> data_queue;
int max_count = 20;

void writer_queue()
{
  for (int i = 0; i < 20; i++)
  {
    data_queue.push(i);
  }

  count.store(20, std::memory_order_release);
}


void reader_queue()
{
  while (true)
  {
    int item_index = 0;
    if (!(item_index = count.fetch_sub(1, std::memory_order_acquire) <= 0))
    {
      // Wait for items
      std::this_thread::sleep_for(std::chrono::microseconds(500));
      continue;
    }
    // Process the item
  }
}


// Ch 6 - [75] Implementation of spin lock mutex
class spinlock_mutex
{
  std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
  spinlock_mutex() {}

  void lock()
  {
    while (flag.test_and_set(std::memory_order_acquire));
  }

  void unlock()
  {
    flag.clear(std::memory_order_release);
  }
};


spinlock_mutex mutex;

void func()
{
  std::lock_guard<spinlock_mutex> lg(mutex);
  std::cout << std::this_thread::get_id() << " hello " << '\n';
  std::this_thread::sleep_for(std::chrono::microseconds(5000));
}


// Ch 7 - [76] Introduction and some terminology
// Ch 7 - [77] Stack recap
// Ch 7 - [78] Simple lock free thread safe stack (pop function)

// Free lock stack implementation (Ch 76, Ch 77, Ch 78)
/*
template<typename T>
class lock_free_stack
{
private:
  struct node
  {
    // T data;
    std::shared_ptr<T> data;
    node* next;

    node(T const& _data) : data(std::make_shared<T>(_data)) {}
  };

  // [1 STEP] change change head to atomic head
  // node* head;
  std::atomic<node*> head;


public:
  void push(T const& _data)
  {
    node* const new_node = new node(_data);
   
    // new_node->next = head;
    new_node->next = head.load(); // load in order to emphasise

    // This is issue, because many threads can add new node (data)
    // This needs to be done in atomic manner
    // [1 STEP]
    // head = new_node;

    // This is not enough - we need while loop update (unitl all threads update)
    // head.compare_exchange_weak(new_node->next, new_node);
    while(!head.compare_exchange_weak(new_node->next, new_node));
  }


  // void pop(T& result)
  //std::shared_ptr<T> pop(T& result)
  std::shared_ptr<T> pop()
  {
    
    // node* old_head = head;
    // head = old_head->next;
    // result = old_head->data;
    // delete old_head;
    
    node* old_head = head.load();
    // If old_head is the same as old_head->next OK
    // If not the same (other thread changed it) - do again (TODO: Check)
    // while (!head.compare_exchange_weak(old_head, old_head->next));

    // In order not to have memory leak
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next));

    // result = old_head->data;
    return old_head ? old_head->data : std::shared_ptr<T>();
  }
};
*/


// Ch 7 - [79] Stack memory reclaim mechanism using thread counting
template<typename T>
class lock_free_stack
{
private:
  struct node
  {
    // T data;
    std::shared_ptr<T> data;
    node* next;

    node(T const& _data) : data(std::make_shared<T>(_data)) {}
  };

  std::atomic<node*> head;
  std::atomic<int> threads_in_pop;
  std::atomic<node*> to_be_deleted;

  void try_reclaim(node* old_head)
  {
    if(threads_in_pop == 1)
    {
      // Delete node pointed by old head
      delete old_head;

      node* claimed_list = to_be_deleted.exchange(nullptr);

      if(!--threads_in_pop)
      {
        delete_nodes(claimed_list);

      }
      else if(claimed_list)
      {
        node* last = claimed_list;
        while(node* const next = last->next)
        {
          last = next;
        }

        last->next = to_be_deleted;
        while(!to_be_deleted.compare_exchange_weak(last->next, claimed_list));
      }
    }
    else
    {
      // Add node pointed by old_head to the to_be_deleted list
      old_head->next = to_be_deleted;
      while(!to_be_deleted.compare_exchange_weak(old_head->next, old_head));
      --threads_in_pop;
    }

  }

  void delete_nodes(node* nodes)
  {
    while(nodes)
    {
      node* next = nodes->next;
      delete nodes;
      nodes = next;
    }
  }


public:
  void push(T const& _data)
  {
    node* const new_node = new node(_data);
   
    // new_node->next = head;
    new_node->next = head.load(); // load in order to emphasise

    // This is issue, because many threads can add new node (data)
    // This needs to be done in atomic manner
    // [1 STEP]
    // head = new_node;

    // This is not enough - we need while loop update (unitl all threads update)
    // head.compare_exchange_weak(new_node->next, new_node);
    while(!head.compare_exchange_weak(new_node->next, new_node));
  }


  // void pop(T& result)
  //std::shared_ptr<T> pop(T& result)
  std::shared_ptr<T> pop()
  {
    ++threads_in_pop;
   
    node* old_head = head.load();

    // In order not to have memory leak
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next));

    std::shared_ptr<T> res;
    if(old_head)
    {
      res.swap(old_head->data);
    }

    try_reclaim(old_head);

    return res;
  }
};









