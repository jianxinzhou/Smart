#ifndef __MYCACHE_H__
#define __MYCACHE_H__
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <utility>
#include <fstream>
#include "MyLock.h"

// 数据成员为
// unorderer_map
// 控制互斥访问unordered_map的锁：hashmapLock_
class MyCache
{
	public:
        std::unordered_map<std::string, std::string> hashmap_;
		
        void map_to_cache(std::string& key, std::string& value )
		{
			hashmapLock_.lock();
			hashmap_[key] = value;
			hashmapLock_.unlock();
		}
		
        std::unordered_map<std::string, std::string>::iterator 
        isMapped(const std::string& word)
		{
			return hashmap_.find(word);
		} 
		
        // 将内存cache写入硬盘
        void write_to_file(std::ofstream& outfile)
		{
			hashmapLock_.lock();
			for(std::unordered_map<std::string, std::string>::iterator iter = hashmap_.begin(); 
                iter != hashmap_.end();
                ++iter)
			{
				outfile << iter -> first << "\t" << iter -> second << std::endl ;
			}
			hashmapLock_.unlock();
		}
		
        // 从硬盘cache读入内存
        void read_from_file(const std::string &fileName)
		{
			std::ifstream infile(fileName.c_str());
			if(!infile)
			{
				std::cout << "cache file: " << file_name << std::endl ; 
				throw std::runtime_error("open cache file fail !");
			}
			std::string query , result;
			while(infile >> query >> result)
			{
				hashmap_.insert(std::make_pair<std::string, std::string>(query, result));
			}
			infile.close();
		}
	
    private:
		MyLock hashmapLock_;
};


#endif
