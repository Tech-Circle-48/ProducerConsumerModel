/*
 * @Author: your name
 * @Date: 2022-03-03 12:15:21
 * @LastEditTime: 2022-03-06 14:53:14
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/ConfigManager.h
 */
#ifndef _CONFIGMANAGER_H_
#define _CONFIGMANAGER_H_

#include <string>
#include "Singleton.h"
using namespace std;

class ConfigManager:public Singleton<ConfigManager>
{
public:
	friend class Singleton<ConfigManager>;

	int Init(int argc, char* argv[]);

	inline int GetProduceFd()
	{
		return m_produceFd;
	}
	inline int GetConsumeFd()
	{
		return m_consumeFd;
	}
	inline int GetMaxNumberOfProducer()
	{
		return m_maxProducer;
	}
	inline int GetMaxNumberOfConsumer()
	{
		return m_maxConsumer;
	}
	inline size_t GetMaxOffset()
	{
		return m_maxOffset;
	}
		
private:
	ConfigManager();
	~ConfigManager();

	void InitResult(int error);
	int CheckFile();

	string m_produceFile;
	string m_consumeFile;
	int m_produceFd;
	int m_consumeFd;
	size_t m_maxOffset;
	int m_maxProducer;
	int m_maxConsumer;
};

#endif
