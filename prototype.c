//Common Headers
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "request.h"
#include "checkip.h"

int main(){
    printf("Dynamic DNS Updater.\n");
    //char* hostname="freedns.afraid.org";
    //char* hostname="checkip.dyndns.com";
    //char* hostname="boards.4chan.org";
    //char* getRequest;///g/thread/61890565/dpt-daily-programming-thread";
    //char* httpResponse=makeHTTPRequest(hostname,"/g/thread/61890565/dpt-daily-programming-thread","");
    
    // char* httpsresp = makeHTTPSRequest("boards.4chan.org","/a/","");
    // printf("HTTPS: %lu\n\n%s",strlen(httpsresp),httpsresp);

    int isUpdated=checkIPUpdate();
    printf("%d\n",isUpdated);
    printf("%s\n",LAST_IP_CHECK);
    
    //printf("%s\n",getCurrentIP(WTFIP));

    //printf("HTTP: %d\n",strlen(makeHTTPRequest("boards.4chan.org","/g/thread/61898971","")));
    // char* stra=makeHTTPRequest("freedns.afraid.org","","");
    // char* strb=makeHTTPSRequest("freedns.afraid.org","","");
    // //stra[400]='\0';strb[400]='\0';
    // printf("HTTP: %d\n",strlen(stra));
    // printf("HTTPS: %d\n",strlen(strb));
    return 0;
}