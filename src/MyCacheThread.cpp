#include "MyCacheThread.h"
#include "ThreadPool.h"
#include "MyThread.h"
#include "MyCache.h"
#include <unistd.h>
#include <fstream>

void MyCacheThread::run()
{
	while(true)
	{
		sleep(60);
		scan_cache();	
		std::cout << "scan cache" << std::endl ;
	}
}

// 线程池对象中拥有一个扫描线程对象
// 线程池初始化时，会调用该函数，使该扫描线程对象持有指向线程池对象的指针
// 并使该扫描线程对象持有线程池中所有工作对象的指针
void MyCacheThread::get_related(ThreadPool* threadPoolPtr)
{
	threadPoolPtr_ = threadPoolPtr;
    
    std::vector<MyThread>::iterator  iter1  = (threadPoolPtr_ -> vecThreads_).begin();
	std::vector<MyThread*>::iterator iter2  = vecWorkThreadPtr_.begin() ;
    
    while(iter2 != vecWorkThreadPtr_.end() && 
          iter1 != (threadPoolPtr_ -> vecThreads_).end() )
	{
        *iter2 = &(*iter1);
        iter1++;
        iter2++;
    }
}

void MyCacheThread::scan_cache()
{
    std::vector<MyThread*>::iterator iter = vecWorkThreadPtr_.begin();
    for(; 
        iter != vecWorkThreadPtr_.end(); 
        ++iter)
    {
        ( (*iter) -> cache_ ).read_from_file( (threadPoolPtr_ -> conf_).getMapConf()["my_cache"].c_str()) ;
        
        std::ofstream outfile( (threadPoolPtr_ -> conf_).getMapConf()["my_cache"].c_str() ) ;
        if(!outfile)
        {
            throw std::runtime_error("scan cache : open cache failed");
        }		
        
        ( (*iter ) -> cache_ ).write_to_file(outfile) ;
        outfile.close();
    }
}

