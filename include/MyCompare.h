#ifndef __MYCOMPARE_H__
#define __MYCOMPARE_H__
#include "MyResult.h"
class MyCompare
{
    public:

        // 重载函数调用操作符，主要用于初始化优先级队列。

        bool operator()(const MyResult& left, const MyResult& right)
        {
            if(left.distance_ > right.distance_)
            {
                return true ;
            }
            else if(left.distance_ == right.distance_ && 
                    left.frequence_ < right.frequence_)
            {
                return true ;
            }
            else if(left.distance_ == right.distance_ && 
                    left.frequence_ == right.frequence_ && 
                    left.word_ > right.word_)
            {
                return true ;
            }
            else 
            {
                return false ;
            }
        }
};
#endif
