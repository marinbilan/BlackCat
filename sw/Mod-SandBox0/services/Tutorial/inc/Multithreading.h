#pragma once
#include <thread>
#include "TutorialIf.h"


namespace Services
{

class Multithreading : public TutorialIf
{
public:
	Multithreading(const std::string& dbPath, const std::string& name);

	~Multithreading();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services



// --- Bo YouTube ----
/*
----
2/10
----
std::thread t1(function_1)
- Any callable object (Functor operator () // another pair of zagrada, lambda function ...)

- join
- detach

try
catch(...)
t1.join();

Pass param to thread?
Additional argument of thread object!
Pass by reference - ako ne navedemo explicit uvijek se prenosi preko VALUE!
- Ako zelimo prenijeti preko reference - std::ref(...) - reference wrapper for s
- Second way - pass by pointer (sharing memory - data race problem) - std::move - from main to child thread!

Many things that can be moved and not coppied!
std::thread t1 = t2; // Will not work! - Use std::move(...)
Each thread ID number std_thread::get_id()

----
3/10 - Data race (Mutex)
----
std::mutex mu;

Ex: Both main and child thread call void shared_print(string msg, int id) { ... }
mu.lock();
// ... // PROBLEM: What if this code throw exception? USE std::lock_guard<std::mutex> guard(mu);
// ANOTHER PROBLEM: Ostali threadovi mogu koristiti isto cout i da ne idu kroz nas mutex
mu.unlock();

Problem gore: Kada guard objekt ode iz scopea, unistava se mu!

DA BI MUTEX ZASTITIO RESOURCE - ON MORA BITI BINDAN S RESOURCEOM KOJEG STITI!
Wraper object - koji sadrzi mutex kao i ofstream f (npr) kojeg stiti!
Sada otvaramo file u konstruktoru f.open() f LOKANA VARIJABLA
Sada cijeli objekt prenosimo preko std::ref a objekt je wrapper koji se brine
NEVER RETURN f outside of object - koji je wrapper!

----
4/10 - Deadlock
----
I want to protect SA dva mutexa!
1. Cout mutex
2. File security reason
Da zastitim - trebam dva mutexa i dva shared printa!
- KORISTIMO lock_guard u dvije metode ALI u obrnutom poretku!
[ KLASICNA DEADLOCK situacija ]
Prva metoda je lockala mutex m1 u prvoj metodi a drugi thread je lockao mutex m2 u drugoj metodi
Prvi ceka da se releasea m1 a drugi da se releasea m2

EVERYBODY SHOULD LOCK in THE SAME ORDER!
Better soultion in std lib
std::lock(m1, m2);
std::lock_guard<mutex> locker(m1, std::adopt_lock); // Tells that mutex is already locked
std::lock_guard<mutex> locker(m2, std::adopt_lock);

Kako izbjec - svaki lock_guard u svojim zagradama

----
5/10 - Unique Lock and Lazy Init
----
[ 3th WAY TO LOCK MUTEX ]:
std::unique_lock<mutex> locker (mu);
... do something ...
locker.unlock(); // Samo dio je u zastiti fleksibilnijeg unique_lock-a
// Ovaj dio ne spada pod mutex

// lock / unlock koliko god  puta zelimo
// Unique lock can only be moved - transfer ownership

LAZY INIT: open inside lambda function

----
6/10 - Condition Variable
----

Problem globalne variable npr. std::deque<int> q;
Thread 1 - producer of data Tread 2 - Consumer data
Unique lock is ok - But problem - Thread 1 is in busy wait state ... - Need sincronisation - More efficient
Take a nap and go in another thread (Time duration?)
Thread one - notify thread two (CONDITIONAL VARIABLE)

std::condition_variable cond;
PRODUCER: cond.notify_one(); // Notify one waiting thread, if there is one!
CONSUMER: cond.wait(locker); // Thread two in sleep unitl tread one notify thread two
Thread two - samo kada se odradi thread one
SPURIOUS WAKE - cond.wait(LAMBDA METHOD)
Broadcast to all thread cond.notify_all() - Wake up all threads

----
7/10 - Future, Promise and  async()
----
Return result from child thread to parrent thread std::ref(...) - not enought x shared between parrent and child thread
First make sure that x is set in parent thread and then passed to child thread.
Code structure becomes measy.

Just launch thread instead thread object:
std::async(factorial, 4); - async is class - returns FUTURE!
stf::future<int> fu = std::async(factorial, 4); // Future is channel how to return variable
x = fu.get();    // Method need to return variable - Wait until child thread finish and return value from child thread
// Get something from future ONLY ONCE!

Async may or may not create new thread (std::launch::deferred, factorial, 4) - Odgodi kreiranje threada!
std::promise<int> p;
std::future<int> f = p.get_future(); // I will get value but I do not have it now.. I will send it in the future!


----
8/10 - Callable objects
----
*/