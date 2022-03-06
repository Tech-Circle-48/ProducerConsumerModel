#include "Process.h"
#include "Header.h"

Process::Process()
{	
}

Process::~Process()
{

}

int Process::Run(void *pContext)
{
    m_ProcessID = fork();       
	if(m_ProcessID == 0)
	{
        m_ProcessID = getpid();

		cout<<"startprocessfunc!!!!!!!"<<endl;
		StartFunctionOfProcess(pContext);

		exit(0);
	}
	else if(m_ProcessID == -1)
	{
		cout << "Process::Run, fork error" << endl;
		return -1;
	}
    else
	   return 0;

}

int
Process::WaitForDeath()
{
	if(m_ProcessID == -1)
	{
		return -1;
	}

	if(waitpid(m_ProcessID, 0, 0) == -1)
	{
		return -1;
	}
	else
	   return 0;
}

