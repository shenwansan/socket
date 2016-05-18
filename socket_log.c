#include "socket_log.h"
#include "packet.h" 


static int sc_handle;

int soc_init()
{
	char string[PACKET_128] ;
	memset(string, 0,  PACKET_128);
	
	if((sc_handle = open(SC_LOG__PATH, O_RDWR|O_TRUNC|O_CREAT))== -1)
	{
		sprintf(string,"%s:%s:%d,open error",__FILE__,__func__,__LINE__ );
		perror(string);
		return -1;
	}
	return 0;
}

int soc_write(char * string)
{
	char string_err[PACKET_128] ;
	memset(string_err, 0,  PACKET_128);
	if(sc_handle != 0)
		write(sc_handle,string,strlen(string) );
	else
	{
		sprintf(string_err,"%s:%s:%d,write error",__FILE__,__func__,__LINE__ );
		perror(string_err);
		return -1;
	}
	return 0;
}




int soc_end()
{
	char string[PACKET_128] ;
	memset(string, 0,  PACKET_128);
	if(sc_handle != 0)
	{
		close(sc_handle);
	}
	else
	{
		sprintf(string,"%s:%s:%d,close error",__FILE__,__func__,__LINE__);
		perror(string);
		return -1;
	}
	return 0;	
}