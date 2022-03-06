/*
 * @Author: your name
 * @Date: 2022-03-03 12:15:17
 * @LastEditTime: 2022-03-06 14:58:47
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /cpp/ProduceProcess.h
 */
#ifndef PRODUCEPROCESS_H_
#define PRODUCEPROCESS_H_

#include "Process.h"

typedef  struct Producer
{
	// length of readbuf
    size_t length;
	// beiginning position for reading  source file
    size_t readoffset;
	// the content from source file
    char *readbuf;
} __attribute__((packed)) Producer;

class ProducePocess : public  Process
{
public:
	ProducePocess(int index, int pfd, int piperfd, int pipewfd);
	virtual  int StartFunctionOfProcess(void *pContext=NULL);
private:
	Producer m_producer;
	int m_produceFile;
	int m_pipeRfd;
	int m_pipeWfd;
	size_t m_maxLength;
};

#endif
