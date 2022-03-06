#include "Tunnel.h"
#include "Header.h"

Tunnel::Tunnel()
{
	bzero(m_pipeFd, 2);
}

Tunnel::~Tunnel()
{
	::close(m_pipeFd[0]);
    ::close(m_pipeFd[1]);
}

bool Tunnel::isOpen() const
{
	return m_pipeFd[0] != 0;
}

void Tunnel::open()
{
	if(isOpen())
		return;
	
	try 
	{
    	int r = ::pipe(m_pipeFd);
        if(r != 0)
        	throw("create pipe error\n") ;
    }
    catch (const char* err)
    {
     	std::cout << err<< std::endl;
    }
}

void Tunnel::closeReadDescriptor()
{
	::close(m_pipeFd[0]);
}

void Tunnel::closeWriteDescriptor()
{
	::close(m_pipeFd[1]);
}

ssize_t Tunnel::read(char * buf, size_t count)
{
	return ::read(m_pipeFd[0], buf, count);
}

ssize_t Tunnel::write(char * buf, size_t count)
{
	return ::write(m_pipeFd[1], buf, count);
}