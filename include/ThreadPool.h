#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "MyThread.h"
#include "MyLock.h"
#include "MyCondition.h"
#include "MyTask.h"
#include <vector>
#include <string>
#include <queue>
#include "MyConf.h"
#include <map>
#include "MyCacheThread.h"


class ThreadPool
{
	public:
	    friend class MyCacheThread ;
		
        ThreadPool(MyConf &conf, int size = 12)
            :vecThreads_(size),
             queueTaskslock_(), 
             queueTasksCond_(queueTaskslock_),
             isStarted_(false),
             conf_(conf),
             cacheThread_(size)
	{
		std::vector<MyThread>::iterator iter ;
		for(iter = vecThreads_.begin(); 
                    iter != vecThreads_.end(); 
                    ++iter )
		{
		    iter -> get_related(this);       // 使线程池中的每一个工作线程持有线程池对象的指针
		}
		cacheThread_.get_related(this);      // 使线程池中的扫描线程持有线程池对象的指针
	}

		void on()
		{
			if(isStarted_)
			{
				return ;
			}
			isStarted_ = true ;
			std::vector<MyThread>::iterator iter ;
			for(iter = vecThreads_.begin(); iter != vecThreads_.end(); iter ++)
			{
				iter -> start();    // 开启工作线程
			}
			cacheThread_.start();   // 开启扫描线程
		}
		void off()
		{
			if(isStarted_)
			{
				isStarted_ = false ;
				queueTasksCond_.broadcast();
				while(!queueTasks_.empty())
				{
					queueTasks_.pop();
				} 
			}
		}
		void allocate_task( MyTask& task)
		{
			queueTaskslock_.lock();
			std::cout << "Add Task" << std::endl ;
			queueTasks_.push(task);
			queueTaskslock_.unlock();
			queueTasksCond_.broadcast();
		}
		bool get_task(MyTask &task)
		{
			queueTaskslock_.lock();
			while(isStarted_ && queueTasks_.empty())
			{
				queueTasksCond_.wait();
			}
			if(!isStarted_)
			{ 
				queueTaskslock_.unlock();
				queueTasksCond_.broadcast();
			 	return false ;
			}
			task = queueTasks_.front();
			queueTasks_.pop();
			queueTaskslock_.unlock();
			queueTasksCond_.broadcast();
			std::cout << "get task" << std::endl ;
			return true ;
		}
		
        MyConf &conf_; // 配置对象的引用                       
	
    private:
        // 禁止赋值和复制
        ThreadPool(const ThreadPool& obj) ;
	ThreadPool& operator = (const ThreadPool& obj) ;
		
        std::vector<MyThread> vecThreads_ ;   // 存放工作线程的容器
	std::queue<MyTask>    queueTasks_ ;   // 存放任务的队列
		
        MyLock queueTaskslock_ ;
	MyCondition queueTasksCond_ ;
		
        bool isStarted_ ;                     // 用于标识线程池是否开启的变量
		
        MyCacheThread cacheThread_ ;          // 定时扫描内存cache的线程
};



#endif
