#include "MyThread.h"
#include "ThreadPool.h"
#include <iostream>

void MyThread::run()
{
	std::cout << "run" << std::endl ;
	cache_.read_from_file( (threadPoolPtr_ -> conf_).getMapConf()["my_cache"].c_str());
	while(true)
	{
        MyTask task(threadPoolPtr_-> conf_) ;
        if(!(threadPoolPtr_ -> get_task(task)) )
        {
            break ;
        }
        task.excute(cache_) ;
    }
}
