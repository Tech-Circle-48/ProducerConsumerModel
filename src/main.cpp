/*
 * @Author: your name
 * @Date: 2021-09-24 23:15:09
 * @LastEditTime: 2022-03-06 14:53:38
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/main.cpp
 */
#include "ProduceProcess.h"
#include "ConsumerProcess.h"
#include "ConfigManager.h"
#include "Tunnel.h"
#include "Header.h"

Tunnel *g_pPipe = NULL;

int main(int argc,char *argv[])
{
	//Program init
	int rt = ConfigManager::getInstance()->Init(argc, argv);
	if(rt != 0)
		exit(-1);

	//prepare pipes for Producers and Consumers
	g_pPipe = new Tunnel();
	g_pPipe->open();

	cout << "producing......" << endl;
	
	//initialize Producers

	std::vector<ProducePocess*> produceList;

	for(int i = 0;i < ConfigManager::getInstance()->GetMaxNumberOfProducer(); i++)
	{
		 ProducePocess *p = new ProducePocess(
		 	i,
		 	ConfigManager::getInstance()->GetProduceFd(),
		 	g_pPipe->getReadDescriptor(),
			g_pPipe->getWriteDescriptor());
		 produceList.emplace_back(p);
	}

	//create Producers
	for(auto produce : produceList)
	{
	     produce->Run();
	}

	
	cout << "consuming......" << endl;

	std::vector<ConsumerProcess*> consumerList;
	for(int j = 0 ; j < ConfigManager::getInstance()->GetMaxNumberOfConsumer(); j++)
    {
	    ConsumerProcess *pConsume = new ConsumerProcess(
			ConfigManager::getInstance()->GetConsumeFd(),
			g_pPipe->getReadDescriptor(),
			g_pPipe->getWriteDescriptor());
	    consumerList.push_back(pConsume);
    }
	//initialize and create Consumers

	for(auto consume : consumerList)
	{
	     consume->Run();
	}
	
	for(auto p : produceList)
	{
		p->WaitForDeath();
	}

	g_pPipe->closeWriteDescriptor();
	
    for(auto c : consumerList)
    {
        c->WaitForDeath();
    }

	g_pPipe->closeReadDescriptor();
	
    for(auto k : consumerList)
    {
        delete k;
        k = NULL;
    }

    for(auto j : produceList)
    {
        delete j;
        j = NULL;
    }
	
	return 0;
}

