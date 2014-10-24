#ifndef __MYTASK_H__
#define __MYTASK_H__
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <map>
#include <queue>
#include "MyConf.h"
#include "MyResult.h"
#include "MyCompare.h"
#include <unistd.h>
#include <stdint.h>

class MyCache ;

class MyTask
{
    public:
        MyTask( MyConf& conf);
        
        MyTask(const std::string &queryWord, 
               const struct sockaddr_in &addr ,  
               MyConf& conf);
        
        void excute(MyCache& cache) ;      // 执行函数。需要传递一个MyCache对象 。
        
        int length(const std::string& str) // 计算查询词的长度
        {
            int index ;
            int len = 0 ;
            for(index = 0 ; index != str.size(); index ++)
            {
                if(str[index] & (1 << 7))
                {
                    index ++ ;
                }
                len ++ ;
            }
            return len ;
        }
        
        ~MyTask()
        {
            close(peerfd_);
        }
        
        void satistic(std::set<int> & iset ); //计算vecDictPtr_指向的vector中下标在iset中的词与用户输入词的编辑距离 。
    
    
    private:
        std::string queryWord_;                  // 用户的查询词
        std::vector<uint32_t> vecQueryWord_;     // 经过转换后的用户的查询词
        struct sockaddr_in addr_;     // 用于保存用户端地址和端口号
        int peerfd_;                  // 与用户端通信的socket描述符
        
        std::vector<std::pair<std::vector<uint32_t>, int> > *vecDictPtr_;      // 指向保存数据词典的指针
        std::vector<std::pair<std::string, int>> *strDictPtr_;
        std::map<uint32_t, std::set<int> >* mapIndexPtr_;          // 指向词典索引的指针
        
        std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> result_; // 用于保存查询结果的优先级队列
        
        void get_result(); // 根据用户的查询词获取最终结果。最终结果将放在优先级队列里
        int editdistance(const std::vector<uint32_t> &right); // 计算right与用户输入查询词的编辑距离
        
        int triple_min(const int &a, const int &b, const int& c ) // 返回3个数中的最小值
        {
            return a < b ? (a < c ? a : c) : (b < c ? b : c) ;
        }

};
#endif
