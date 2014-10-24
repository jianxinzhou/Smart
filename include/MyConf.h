#ifndef __MYCONF_H__
#define __MYCONF_H__
#include <map>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <set>
#include <stdint.h>
#include <vector>

class MyConf
{
    public:
        MyConf(const std::string& name);
        
        void setIndex(int vecDictIndex);
        std::map<std::string, std::string>& getMapConf();
    
    private:
        std::ifstream infile_;

        std::map<std::string, std::string> mapConf_ ;           // 配置文件
        std::vector<std::pair<std::string, int> > strDict_;
        std::vector< std::pair<std::vector<uint32_t>, int> > vecDict_;    // 词频字典
        std::map<uint32_t, std::set<int> > mapIndex_;        // 字母索引


        friend class MyTask ;
        friend class MySocket ;
        friend class ThreadPool ;

};


#endif
