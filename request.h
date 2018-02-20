//Common Headers
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
//Other Headers
#include "netdb.h"
#include <arpa/inet.h>
#include <netinet/in.h>
//System Headers
#include "sys/socket.h"
#include "sys/types.h"
//configuration
#include "config.h"
#ifdef deploy
    #define memChunk256k 262144
#else
    #define memChunk256k 40960 //DEBUG smaller value
#endif

char* makeHTTPRequest(char* hostname ,char* GET_, char* POST_);
char* makeHTTPSRequest(char* hostname, char* GET_, char* POST_);