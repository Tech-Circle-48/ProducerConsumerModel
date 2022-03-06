#ifndef _TUNNEL_H_
#define _TUNNEL_H_

#include <sys/types.h>

class Tunnel
{
public:
	Tunnel();
	~Tunnel();

	bool isOpen() const;
	void open();
	void closeWriteDescriptor();
	void closeReadDescriptor();
	ssize_t read(char *buf, size_t count);
	ssize_t write(char *buf, size_t count);

	inline int getReadDescriptor()
 	{
     	return m_pipeFd[0];
 	}

 	inline int getWriteDescriptor()
 	{
     	return m_pipeFd[1];
 	}

private:
    int m_pipeFd[2];
};

#endif