#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__
#include "Thread.h"
#include "MyCache.h"

class ThreadPool;

// 继承抽象类Thread
class MyThread : public Thread 
{
    public:
        void get_related(ThreadPool* p)
        {
            threadPoolPtr_ = p ;
        }
    
    private:
        void run(); // 需要自己实现虚函数
        ThreadPool * threadPoolPtr_ ;
        MyCache cache_ ;
	    
        friend class  MyCacheThread ;

};
#endif
