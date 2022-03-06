/*
 * @Author: your name
 * @Date: 2022-03-03 12:37:43
 * @LastEditTime: 2022-03-06 15:27:31
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/ProduceProcess.cpp
 */
#include "ProduceProcess.h"
#include "ConfigManager.h"
#include "Header.h"

ProducePocess::ProducePocess(int index, int pfd, int piperfd, int pipewfd):
	m_produceFile(pfd),
	m_pipeRfd(piperfd),
	m_pipeWfd(pipewfd),
	m_maxLength(ConfigManager::getInstance()->GetMaxOffset())
{
	m_producer.readoffset = index*m_maxLength;
}

int ProducePocess::StartFunctionOfProcess(void * pContext)
{
	::close(m_pipeRfd);
	
	m_producer.readbuf = new char[m_maxLength];
	bzero(m_producer.readbuf, m_maxLength);

	int ret;
	size_t buffersize = sizeof(size_t)*2+m_maxLength;
    char *buffer = new char[buffersize];
    bzero(buffer, buffersize);

	cout << m_produceFile << "," << m_pipeRfd << "," << m_pipeWfd << endl;
    //Producer read from sourcefile
    while( (ret=pread(m_produceFile,m_producer.readbuf,m_maxLength,m_producer.readoffset)) == m_maxLength )
    {
		m_producer.length = m_maxLength;
        cout << "producer read offset: " << m_producer.readoffset << ", length: " << m_producer.length << endl;

        ::memcpy(buffer, &m_producer, sizeof(size_t)*2);
        ::memcpy(buffer+sizeof(size_t)*2, m_producer.readbuf, m_maxLength);

        //write to pipe
        int ret = ::write(m_pipeWfd, buffer, buffersize);
        if(ret < 0)
        {
            perror("write header error!");
            break;
        }
        else
        {
            cout << "producer write header to pipe " << ret << endl;
        }


        m_producer.readoffset += ConfigManager::getInstance()->GetMaxNumberOfProducer()*m_maxLength;
        bzero(m_producer.readbuf, m_maxLength);
        bzero(buffer, buffersize);
    }
    // maybe read to end of file, it's not enough for an amount of m_maxLength
    if(ret >=0 && ret < m_maxLength)
    {
        m_producer.length = ret;
		buffersize = sizeof(size_t)*2+m_maxLength;
        cout << " finish, read offset: " << m_producer.readoffset << ", length: " << m_producer.length << endl;
        if(ret > 0)
        {
           
            memcpy(buffer, &m_producer, sizeof(size_t)*2);
            memcpy(buffer+sizeof(size_t)*2, m_producer.readbuf, m_producer.length);
             //write to pipe
            int ret = write(m_pipeWfd, buffer, buffersize);
            if(ret < 0)
            {
                perror("write error!");
            }
        }
    }
    else
    {
        perror("Producer last pread error!");
		cout << m_produceFile << "," << m_producer.readoffset << endl;
    }

    //close(g_pipefd[1]);
    delete [] m_producer.readbuf;
	m_producer.readbuf = nullptr;
    delete [] buffer;
	buffer = nullptr;
    ::close(m_produceFile);
	::close(m_pipeWfd);

	return 0;
}
