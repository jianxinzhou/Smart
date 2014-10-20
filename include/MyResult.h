#ifndef __MYRESULT_H__
#define __MYRESULT_H__

// 用来存储程序查询结果的，该对象放在优先级对象中
class MyResult
{
	public:
		std::string word_ ; // 保存词库中的词
		int frequence_ ;    // 保存词库中词的词频
		int distance_ ;     // 保存词库中的词与用户输入词的编辑距离
};
#endif
