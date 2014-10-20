#ifndef __MYLOCK_H__
#define __MYLOCK_H__
#include <pthread.h>
#include <iostream>
class MyLock
{
    public:
        MyLock()
        {
            // 初始化成功返回0
            if( pthread_mutex_init(&m_mutex, NULL) )
            {
                std::cout << __DATE__  << " " << __TIME__ << " " 
                          << __FILE__  << " " << __LINE__ << ":"
                          << "pthread_mutex_init failed!" << std::endl;
                exit (-1); 
            }
        }
        
        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }
        
        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }
        
        ~MyLock()
        {
            pthread_mutex_destroy(&m_mutex);
        }
	
    private:
        pthread_mutex_t m_mutex ;
        
        friend class MyCondition;

};


#endif
