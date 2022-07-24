
#include "profiler.h"
#include <iostream>
#include <vector>
#include <windows.h>

#define PROFILING 1
#if PROFILING
#define TIMER_SCOPE(name) Timer timer##__LINE__(name)
#define TIMER_FUNCTION() TIMER_SCOPE(__FUNCTION__)   // (__FUNCSIG__)
#else
#define TIMER_SCOPE(name)
#define TIMER_FUNCTION() TIMER_SCOPE()
#endif


void a()
{
    TIMER_FUNCTION();
    Sleep(50);
}

void b()
{
    TIMER_FUNCTION();
    Sleep(100);
}

void f1() 
{
    TIMER_FUNCTION();
    Sleep(200);
    a();
}

void f2() 
{
    TIMER_FUNCTION();
    Sleep(200);
    b();
}

void f3() 
{
    TIMER_FUNCTION();
    Sleep(200);
    a();
    b();
}

int main()
{
    Profiler::Get().BeginSession("Test");

    std::vector<std::thread> workers;

    workers.push_back(std::thread(f1));
    workers.push_back(std::thread(f2));
    workers.push_back(std::thread(f3));

    for (auto& worker : workers)
    {
        worker.join();
    }

    Profiler::Get().EndSession();

    return 0;
}

