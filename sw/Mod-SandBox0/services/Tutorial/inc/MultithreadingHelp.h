#include <thread> 

// Ch 1 - [4] How to launch thread
void foo() {}


// Run thread via operator ()
class callable_class
{
public:
  void operator()()
  {
    std::cout << "callable_class operator thead ID: " << std::this_thread::get_id() << '\n';
  }
};