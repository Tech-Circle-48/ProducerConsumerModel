/*
 * @Author: your name
 * @Date: 2022-03-03 12:15:10
 * @LastEditTime: 2022-03-06 14:40:57
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/ConsumerProcess.h
 */
#ifndef CONSUMERPROCESS_H_
#define CONSUMERPROCESS_H_

#include "Process.h"

typedef struct Consumer
{
    // length of writebuf	
    size_t length;
	// the beginning  position for writing file
    size_t writeoffset;
	// the content from Tunnel 
    char *writebuf;
} __attribute__((packed)) Consumer;

class ConsumerProcess : public Process
{
public:
	ConsumerProcess(int cfd, int piperfd, int pipewfd);
	virtual int StartFunctionOfProcess(void *pContext=NULL);
private:
    Consumer m_consumer;
	int m_consumerFile;
	int m_pipeRfd;
	int m_pipeWfd;
	size_t m_maxLength;
};

#endif
