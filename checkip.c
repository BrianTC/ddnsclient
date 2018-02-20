#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "checkip.h"
#include "request.h"

int checkIPUpdate(){
    char* ipFile="/tmp/dnschecker.ip";
    //try to open file, if null assume file doesnt exist
    //000.000.000.000 16 chars (15+null)
    size_t ipLen=16;
    char oldIP[ipLen];
    FILE* ipStore = fopen(ipFile,"r+"); 
    char* newIP;//="000.000.000.000";
    newIP=getCurrentIP(WTFIP);//use WTFIP default since https
    if(ipStore==NULL){
        ipStore=fopen(ipFile,"w");
        fputs(newIP,ipStore);
        return  1;
    }
    fgets(oldIP,ipLen,ipStore);
    if(strcmp(newIP,oldIP)==0){
        return 0;
    }
    else{
        ipStore = fopen(ipFile,"w");
        fputs(newIP,ipStore);
        return  1;
    }
}

char* getCurrentIP(IPSources checktype){
    char* requestResult,* suba;//sub(x) are temporary values to aid in retrevial of the IP address
    if(checktype==WTFIP){
        //DOES soupport HTTPS
        requestResult=makeHTTPSRequest("wtfismyip.com","/text","");
        if(requestResult==(char*)-1){
            printf("DNS FAILURE TRY ANOTHER SOURCE\n");
            checktype++; //GO TO NEXT SOURCE
        }
        else{
            suba=strstr(requestResult,"\r\n\r\n")+4;//find location of dual CR/NL + 4 chars
            if(suba==NULL) exit(23);
            strtok(suba,"\r\n");
            LAST_IP_CHECK=suba;
            return suba;
        }
    }
    if(checktype==DYNDNS){
        //Doesnt soupport HTTPS
        requestResult=makeHTTPRequest("checkip.dyndns.com","","");
        if(requestResult==(char*)-1){
            printf("DNS FAILURE TRY ANOTHER SOURCE\n");
            exit(0);
        }
        else{
            suba=strstr(requestResult,"Address:")+9;//find location of Address:_ 9 characterslong
            if(suba==NULL) exit(23);
            strtok(suba," <");//the space is probably NOT needed but im inclined to keep it
            LAST_IP_CHECK=suba;
            return suba;
        }
    }
    if(checktype==WIMIPDHOST){
        //Doesnt soupport HTTPS
        requestResult=makeHTTPRequest("whatismyip.host","","");
        if(requestResult==(char*)-1){
            printf("DNS FAILURE TRY ANOTHER SOURCE\n");
            exit(0);
        }
        else{
            suba=strstr(requestResult,"<p class=\"ipaddress\">")+21;
            if(suba==NULL) exit(23);
            strtok(suba,"<");//the space is probably NOT needed but im inclined to keep it 128.235.85.192</p>
            LAST_IP_CHECK=suba;
            return suba;
        }
    }
    else{
        printf("Lookup Failure.\n");
    }
    // if(){
    //
    // }
    LAST_IP_CHECK=NULL;
    return NULL;
}