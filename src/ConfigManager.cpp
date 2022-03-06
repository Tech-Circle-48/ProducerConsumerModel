#include "ConfigManager.h"
#include "Header.h"
#include <unistd.h>

#define MAXOFF 2032

ConfigManager::ConfigManager():
	m_maxOffset(MAXOFF),
	m_maxProducer(1),
	m_maxConsumer(1)
{
}

ConfigManager::~ConfigManager()
{
}

int ConfigManager::Init(int argc, char* argv[])
{
	int errcode = 0;
	if(argc != 5)
		errcode = 1;
	else
	{
		char* pwdstr = getcwd(NULL,0);
		if(pwdstr != NULL)
		{
			m_produceFile = string(pwdstr) + string("/") + string(argv[1]);
			m_consumeFile = string(pwdstr) + string("/") + string(argv[2]);
		}
		else
			errcode = 2;
		CheckFile();
		m_maxProducer = atoi(argv[3]);
		m_maxConsumer = atoi(argv[4]);
	}

	InitResult(errcode);
	return errcode;
}

void ConfigManager::InitResult(int error)
{

	switch (error)
	{
		case 1:
			cout << "Usage:./copyfile [sourcefile] [dstfile] [producer_number] [consumer_number]" << endl;
			break;
		case 2:
		case 3:
			cout << "file check error!" << endl;
			break;
		default:
			cout<<"init ok!"<<endl;
			break;
	}
}

int ConfigManager::CheckFile()
{
	m_produceFd = open(m_produceFile.c_str(), O_RDONLY);
	if(m_produceFd < 0)
	{
		perror("produce file open error!");
		return 2;
	}

	m_consumeFd = open(m_consumeFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR |  S_IWUSR);
	if(m_consumeFd < 0)
	{
		perror("consume file open error!");
		return 3;
	}

	return 0;
}
