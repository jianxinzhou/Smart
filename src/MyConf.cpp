#include "MyConf.h"


namespace
{

    // 计算UTF8编码所占的字节
    int getLenOfUTF8(unsigned char c)
    {
        int cnt = 0;
        while(c & (1 << (7-cnt)))
            ++cnt;
        return cnt; 
    }

    // 每个vector代表一个word
    // e5 ae bf
    // 把字符串解析成uint32_t数组
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

}

// end namespace


MyConf::MyConf(const std::string& name)
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

        std::vector<uint32_t> vecWord;
        parseUTF8String(word, vecWord);

        strDict_.push_back(make_pair(word, frequency));     // vector<string int>
        vecDict_.push_back(make_pair(vecWord, frequency));
    }
    infile_.close();

    // 依次设置每个单词(vector<uint32_t>)的每个字母所对应的索引
    for(int vecDictIndex = 0 ; 
            vecDictIndex != vecDict_.size();
            ++vecDictIndex)
    {
        setIndex(vecDictIndex);
    }
}


void MyConf::setIndex(int vecDictIndex)
{
    std::vector<uint32_t> vecWord; uint32_t letter; // 单词，以及单词中的每一个字母（汉字）
    vecWord = vecDict_[vecDictIndex].first ; // 根据词典的下标来找每一个单词（词典的下标正是我们传给setIndex的参数）
    // 遍历单词的每一个字母，制作索引
    for(int index = 0; 
            index != vecWord.size(); 
            ++index)
    {
        letter = vecWord[index];
        mapIndex_[letter].insert(vecDictIndex); //将包含这个字母的单词 所在vector的下标放入set中
    }
}

std::map<std::string, std::string>& MyConf::getMapConf()
{
    return mapConf_ ;
}

