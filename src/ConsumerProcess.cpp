/*
 * @Author: your name
 * @Date: 2022-03-03 12:30:15
 * @LastEditTime: 2022-03-06 14:35:03
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/ConsumerProcess.cpp
 */
#include "ConsumerProcess.h"
#include "ConfigManager.h"
#include "Header.h"

ConsumerProcess::ConsumerProcess(int cfd, int piperfd, int pipewfd):
	m_consumerFile(cfd),
	m_pipeRfd(piperfd),
	m_pipeWfd(pipewfd),
	m_maxLength(ConfigManager::getInstance()->GetMaxOffset())
{
}

int ConsumerProcess::StartFunctionOfProcess(void * pContext)
{
	::close(m_pipeWfd);
	
	size_t buffersize = sizeof(size_t)*2 + m_maxLength;
	char* pBuffer = new char[buffersize];
    bzero(pBuffer, buffersize);

    //consumer read from pipe
    while(true)
	{
    	ssize_t rd = read(m_pipeRfd, pBuffer, buffersize);
        if(rd < 0)
        {
			perror("read head from pipe error!");
            break;
        }
        cout << "consumer read from pipe:" << rd << endl;

        ::memcpy(&m_consumer, pBuffer, sizeof(size_t)*2);
		m_consumer.writebuf = new char[m_consumer.length];
        bzero(m_consumer.writebuf, m_consumer.length);
        ::memcpy(m_consumer.writebuf, pBuffer+sizeof(size_t)*2, m_consumer.length);

		cout << "consumer write from " << m_consumer.writeoffset << endl;

        //copy to copyfile
        int wn = pwrite(m_consumerFile, m_consumer.writebuf, m_consumer.length, m_consumer.writeoffset);
        if(wn < 0)
        {
            perror("pwrite to file error!");
            break;
        }

        delete [] m_consumer.writebuf;
		m_consumer.writebuf = NULL;
        bzero(pBuffer, buffersize);

        if(rd == 0 || rd < m_maxLength)
        {
            cout << "consumer finished!" <<endl;
            break;
        }
    }

    delete [] pBuffer;
    pBuffer = NULL;
    ::close(m_pipeRfd);

	return 0;
}
