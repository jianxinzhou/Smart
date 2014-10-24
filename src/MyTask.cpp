/*************************************************************************
  > File Name: MyTask.h
  > Author: Comst
  > Mail:750145240@qq.com 
  > Created Time: Sun 04 May 2014 08:56:41 PM CST
 ************************************************************************/

#include "MyTask.h"
#include <functional>
#include <algorithm>
#include "MyCache.h"
#include <string.h> 



namespace
{

int getLenOfUTF8(unsigned char c)
{
    int cnt = 0;
    while(c & (1 << (7-cnt)))
        ++cnt;
    return cnt; 
}


void parseUTF8String(const std::string &s, std::vector<uint32_t> &vec)
{
    vec.clear();
    for(std::string::size_type ix = 0; ix < s.size(); ++ix)
    {
        int len = getLenOfUTF8(s[ix]);
        uint32_t t = (unsigned char)s[ix]; /*e5*/
        if(len > 1)
        {
            --len; /*2*/
            /*拼接剩余的字节*/
            while(len--)
            {
                t = (t << 8) + (unsigned char)s[++ix];
            }
        }
        vec.push_back(t);
    }
}

inline int MIN(int a, int b, int c) 
{
    int ret = (a < b) ? a : b;
    ret = (ret < c) ? ret : c;
    return ret;
}

int edit_distance_uint_32(const std::vector<uint32_t> &w1, const std::vector<uint32_t> &w2) 
{
    int len_a = w1.size();
    int len_b = w2.size();
    int memo[100][100];
    memset(memo, 0x00, 100 * 100 * sizeof(int));
    for (int i = 1; i <= len_a; ++i) 
    {
        memo[i][0] = i;
    }
    for (int j = 1; j <= len_b; ++j) 
    {
        memo[0][j] = j;
    }
    for (int i = 1; i <= len_a; ++i) 
    {
        for (int j = 1; j <= len_b; ++j) 
        {
            if (w1[i - 1] == w2[j - 1]) 
            {
                memo[i][j] = memo[i - 1][j - 1];
            } 
            else 
            {
                memo[i][j] = MIN(memo[i - 1][j - 1], memo[i][j - 1],memo[i - 1][j]) + 1;
            }
        }
    }
    return memo[len_a][len_b];
}

}
// end namespace

MyTask::MyTask( MyConf& conf)
    : queryWord_(""),
    strDictPtr_(&(conf.strDict_)),
    vecDictPtr_(&(conf.vecDict_)),
    mapIndexPtr_(&conf.mapIndex_)
{
    memset(&addr_, 0, sizeof(addr_));
}

MyTask::MyTask(const std::string &queryWord, 
        const struct sockaddr_in &addr ,  
        MyConf& conf)
    : queryWord_(queryWord),
      addr_(addr),
      strDictPtr_(&(conf.strDict_)),
      vecDictPtr_(&conf.vecDict_), 
      mapIndexPtr_(&conf.mapIndex_)
{
    parseUTF8String(queryWord_, vecQueryWord_);
}

// 执行任务，并将结果发回客户端
void MyTask::excute(MyCache& cache) // cache_通过工作线程传入
{
    peerfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    std::cout << "Task excute" << std::endl ;
    std::unordered_map<std::string, std::string>::iterator iter;
    iter =  cache.isMapped(queryWord_);
    if(iter != cache.hashmap_.end())
    {
        std::cout << " cached "  << std::endl;
        int iret = sendto(peerfd_, (iter -> second).c_str(), 
                (iter -> second).size(), 0, 
                (struct sockaddr*)&addr_, sizeof(addr_));
        std::cout <<"send: " << iret << std::endl ;
    }
    else 
    {
        std::cout << " no cached " << std::endl ;
        get_result();
        //std::cout << inet_ntoa(m_addr.sin_addr) << std::endl ;
        if(result_.empty())
        {
            std::string res = "no anwser !" ;
            int iret = sendto(peerfd_, res.c_str(), 
                    res.size(), 0, 
                    (struct sockaddr*)&addr_, sizeof(addr_));
            std::cout <<"send: " << iret << std::endl;
        }
        else 
        {
            MyResult res = result_.top();
            int iret = sendto(peerfd_, res.word_.c_str(), 
                       res.word_.size(), 0, 
                       (struct sockaddr*)&addr_, sizeof(addr_));
            std::cout <<"send:" << iret << std::endl ;
            cache.map_to_cache(queryWord_, res.word_);
        }
    }
}

// 计算编辑距离
int MyTask::editdistance(const std::vector<uint32_t> &right) 
{
    return edit_distance_uint_32(vecQueryWord_, right);
}


void MyTask::satistic(std::set<int> & iset)
{
    std::set<int>::iterator iter ;
    for( iter = iset.begin() ;  iter != iset.end() ;  iter ++)
    {
        int dist = editdistance(  ((*vecDictPtr_)[ *iter ]).first  );
        if(dist < 3)
        {
            MyResult res ;
            res.word_ = ((*strDictPtr_)[ *iter ]).first ;
            res.distance_ = dist ;
            res.frequence_ = ((*vecDictPtr_)[ *iter ]).second ; 
            result_.push( res );
        }
    }
    /*
       std::cout << __LINE__ <<std::endl ;
       std::cout << "value::first :" << value.first << std::endl ;
       int dist = editdistance( value.first);
       if(dist < 3)
       {
       MyResult res ;
       res.m_word = value.first ;
       res.m_frequence = value.second ;
       res.m_dist = dist ;
       m_result.push( res );
       }
       */
}
void MyTask::get_result()
{
    uint32_t ch ;
    int index ;
    for(index = 0 ; index != vecQueryWord_.size(); index ++ )
    {
        ch = vecQueryWord_[index];
        if( ( *mapIndexPtr_ ).count(ch) )
        {
            std::cout << "map_ cout return true " << std::endl ;
            satistic( (*mapIndexPtr_)[ch] ) ;
        }
    } 
}
