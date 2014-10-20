/*************************************************************************
	> File Name: MyThread.cpp
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Sun 04 May 2014 08:30:51 PM CST
 ************************************************************************/
#include "MyThread.h"
#include "ThreadPool.h"
#include <iostream>
void MyThread::run()
{
	std::cout << "run" << std::endl ;
	cache_.read_from_file(threadPoolPtr_ ->m_conf.get_map()["my_cache"]) ;
	while(true)
	{
		MyTask task(threadPoolPtr_->m_conf) ;
		if(  !(threadPoolPtr_ -> get_task(task)) )
		{
			break ;
		}
		task.excute(cache_) ;
		
	}
}
