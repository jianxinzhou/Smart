#ifndef __MYCACHETHREAD_H__
#define __MYCACHETHREAD_H__
#include "Thread.h"
#include <vector>
class ThreadPool;
class MyCache;
class MyThread;


class MyCacheThread	: public Thread
{

    public:
		MyCacheThread(const int& num = 12)
            : Thread(), vecWorkThreadPtr_(num)
		{ }
		
        void get_related(ThreadPool* threadPoolPtr);
	
    private:
		void run() ;
		void scan_cache() ;
		
        ThreadPool* threadPoolPtr_;                // 指向线程池的指针
		std::vector<MyThread*> vecWorkThreadPtr_ ; // 含有指向工作线程指针的vector 。
};


#endif
