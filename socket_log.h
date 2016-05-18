#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

#define SC_LOG__PATH "/var/log/sc_log.txt"

int soc_init();
int soc_write(char * string);
int soc_end();