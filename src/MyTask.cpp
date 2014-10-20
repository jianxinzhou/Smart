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
int MyTask::editdistance(const std::string& right)
{
	std::cout << __FILE__ << std::endl ;
	std::cout << " ----------edit_dist right:------------" << right <<"  size:" << right.size() << std::endl ;
	int len_left = length(queryWord_);
	int len_right = length(right);
	int ** arr_dist = new int* [len_left + 1];
	int index , index_left, index_right;
	std::string subleft , subright ;
	for(index = 0 ; index <= len_left ; index ++)
	{
		arr_dist[index] = new int[len_right + 1];
	}
	int index_i , index_j ;
	for(index_i = 0 , index_j = 0 ; index_j <= len_right ; index_j ++)
	{
		arr_dist[index_i][index_j] = index_j ;
	}
	for(index_i = 1 , index_j = 0 ; index_i <= len_left; index_i ++)
	{
		arr_dist[index_i][index_j] = index_i ;
	}
	for(index_i = 1 , index_left = 0;  index_i <= len_left; index_i ++, index_left ++)
	{
		if(queryWord_[index_left] & (1 << 7))
		{
			subleft = queryWord_.substr(index_left, 2);
			index_left ++ ; 
		}else 
		{
			subleft = queryWord_[index_left];

		}
		for(index_j = 1 , index_right = 0; index_j <= len_right; index_j ++ ,index_right ++)
		{
			if(right[index_right] & (1 << 7)) 
			{
				subright = right.substr(index_right, 2);
				index_right ++ ; 
			}else 
			{
				subright = right[index_right];
			}
			int a = arr_dist[index_i - 1][index_j - 1] + (subleft == subright ? 0 : 1) ;
			arr_dist[index_i][index_j] =  triple_min(a, arr_dist[index_i - 1][index_j] + 1, arr_dist[index_i][index_j - 1] + 1);
		}
	}
	int result = arr_dist[len_left][len_right] ;
	for(index = 0 ; index <= len_left; index ++)
	{
		delete [] arr_dist[index];
	}
	delete [] arr_dist ;
	return result ;

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
				res.word_ = ((*vecDictPtr_)[ *iter ]).first ;
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
	std::string ch ;
	int index ;
	for(index = 0 ; index != queryWord_.size(); index ++ )
	{
		if(queryWord_[index] & (1 << 7))
		{
			ch = queryWord_.substr(index, 2) ;
			index ++ ;
		}else 
		{
			ch = queryWord_.substr(index, 1);
		}
		if( ( *mapIndexPtr_ ).count(ch) )
		{
			std::cout << "map_ cout return true " << std::endl ;
			satistic( (*mapIndexPtr_)[ch] ) ;
		}
	} 

	/*
	   std::cout <<__FILE__<<std::endl ;
	//	for_each((*m_dict).begin(), (*m_dict).end(), satistic );
	std::map<std::string , int>::iterator iter = (*m_dict).begin() ;
	//int line = 0 ;
	while(iter != (*m_dict).end())
	{
	//std::cout << "dict word: " << iter -> first << std::endl ;
	//std::cout <<"_________" << ++line << "_________________"<< std::endl ;
	satistic(*iter);
	iter ++ ;
	}
	*/
}
