#ifndef PROCESS_H_
#define PROCESS_H_

#include <stddef.h>
#include <sys/types.h>

class Process
{
public:
    Process();
	virtual ~Process();
    
    virtual int Run(void *pContext=NULL);
	virtual int WaitForDeath();

protected:
    virtual int StartFunctionOfProcess(void *pContext=NULL) = 0;

private:
   Process(const Process&);
   Process& operator=(const Process&);

protected:
   pid_t m_ProcessID;
};

#endif
