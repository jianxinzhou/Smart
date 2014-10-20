#ifndef __MYCONF_H__
#define __MYCONF_H__
#include <map>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <set>

class MyConf
{
    public:
        
        MyConf(const std::string& name)
            : infile_(name.c_str())
        {
            // 处理配置文件
            if(!infile_)
            {
                std::cout << __DATE__ << " " << __TIME__ <<" "
                          << __FILE__ << " " << __LINE__ <<": "
                          << "open failed!"  << std::endl;
                exit(-1);
            }

            std::string line ;
            while(getline(infile_, line))
            {
                std::istringstream instream(line) ;
                std::string key, value ;
                instream >> key >> value ;
                mapConf_.insert(make_pair(key, value)) ;
            }
            infile_.close();
            

            // 处理字典
            infile_.open(mapConf_["my_dict"].c_str());
            if(!infile_)
            {
                std::cout << __DATE__ << " " << __TIME__ <<" "
                          << __FILE__ << " " << __LINE__ <<": "
                          << "open failed!"  << std::endl;
                exit(-1);
            }
            while(getline(infile_, line))
            {
                std::istringstream instream(line);
                std::string word;
                int frequency;
                instream >> word >> frequency;
                vecDict_.push_back(make_pair(word, frequency));
            }
            infile_.close();
            
            // 依次设置每个单词的每个字母所对应的索引
            for(int vecDictIndex = 0 ; 
                vecDictIndex != vecDict_.size();
                ++vecDictIndex)
            {
                setIndex(vecDictIndex);
            }
        }
          
        
        void setIndex(int vecDictIndex)
        {
            std::string word, letter; // 单词，以及单词中的每一个字母（汉字）
            word = vecDict_[vecDictIndex].first ; // 根据词典的下标来找每一个单词（词典的下标正是我们传给setIndex的参数）
            // 遍历单词的每一个字母，制作索引
            for(int index = 0; 
                index != word.size(); 
                ++index)
            {
                // 中文
                if(word[index] & (1 << 7))
                {
                    letter = word.substr(index, 2);
                    index ++ ;
                }else 
                {
                    letter = word.substr(index, 1) ;
                }
                mapIndex_[letter].insert(vecDictIndex); //将包含这个字母的单词 所在vector的下标放入set中
            }
        }
        
        std::map<std::string, std::string>& getMapConf()
        {
            return mapConf_ ;
        }
    
    private:
        std::ifstream infile_;
        
        std::map<std::string, std::string> mapConf_ ;           // 配置文件
        std::vector< std::pair<std::string, int> > vecDict_;    // 词频字典
        std::map<std::string, std::set<int> > mapIndex_;        // 字母索引


        friend class MyTask ;
        friend class MySocket ;
        friend class ThreadPool ;

};


#endif
