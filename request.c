#include "request.h"
#include "openssl/ssl.h"
//gdb#include "openssl/ssl23.h"

SSL_CTX *sslcontext;
void initssl(){
    SSL_load_error_strings();
    SSL_library_init();
    sslcontext = SSL_CTX_new(SSLv23_client_method());
}


char* makeHTTPSRequest(char* hostname ,char* GET_, char* POST){
    if(sslcontext==NULL){
        initssl();
    }
    char* service="https";
    struct addrinfo hints, *response,*res_itr;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    int status = getaddrinfo(hostname,service,&hints,&response);
    if(status!=0){
        printf("Lookup Error:\n");
        printf("%s: %i\n",gai_strerror(status),status);
        return (char*) -1;
        exit(1);
    }
    int sockfd = socket(response->ai_family,response->ai_socktype,response->ai_protocol);
    if(connect(sockfd,response->ai_addr,response->ai_addrlen)==-1){
        printf("Connection Error_Socket\n");
        printf("%s: %i\n",gai_strerror(status),status);
    }
    SSL* conn = SSL_new(sslcontext);
    SSL_set_fd(conn,sockfd);
    if(SSL_connect(conn)!=1){
        printf("Connection Error_SSL\n");
    }
    //Connection Made Send/receive
    char* testMsg=calloc(1,4096);
    strcat(testMsg,"GET ");
    if(strcmp(GET_,"")==0)
        GET_="/";
    else
        strcat(testMsg,GET_);
    
    strcat(testMsg," HTTP/1.1\r\nHost: ");
    strcat(testMsg,hostname);
    strcat(testMsg,":443");
    // strcat(testMsg,"");
    strcat(testMsg,"\r\nUser-Agent: DDNS Client Prototype\r\n\r\n");
    //printf("REQUEST:\n%s\n",testMsg);
    int msgLen = strlen(testMsg);
    int bytesSent=0;
    bytesSent+=SSL_write(conn,testMsg,msgLen);
    if(bytesSent<=0){
        printf("Send Error");
    }
    while(bytesSent<msgLen){
        //continueMsg??
        bytesSent=msgLen;
    }
    printf("Request Sent.\n");
    //receive data
    int tempBuffLen=Goodput;//Goodput is 1448
    char* fullBuff=malloc(memChunk256k);
    short int chunkCount=1;
    char* tempBuff=malloc(tempBuffLen+1);//+1 for nullterm
    tempBuff[tempBuffLen+1]='\0';
    int bytesRecv=0;
    int recvInt=0;
    while((recvInt=SSL_read(conn,tempBuff,tempBuffLen))){
        
        if(recvInt==-1){
            //printf("Receive Error\n");
            exit(3);
        }
        bytesRecv+=recvInt;
        //printf("Received: %i Bytes(Total: %i).\n",recvInt,bytesRecv);
        //printf("|%s|",tempBuff);
        if(bytesRecv>=memChunk256k*chunkCount){
            //allocate memory for a new chunk, copy old memory into it then free old memory.
            fullBuff=realloc(fullBuff,memChunk256k*(++chunkCount));        
        }
        memcpy(fullBuff+(bytesRecv-recvInt),tempBuff,recvInt);//dest, src,  len
        
        //chunked response termination
        
        char test[6];
        test[6]='\0';
        memcpy(test,fullBuff+(bytesRecv-5),5);
        // //compare numbers visually:
        // if(recvInt==5){
        //     printf("%s\n",test);
        //     for(int i=0;i<strlen(test);i++){
        //         printf("%d\n",test[i]);
        //     }
        //     printf("Check2\n");
        //     char* check2="0\r\n\r\n";
        //     for(int i=0;i<strlen(check2);i++){
        //         printf("%d\n",check2[i]);
        //     }
            
        // }
        // printf("STRCMP: %d\n",strcmp(test,"0\r\n\r\n"));
        if(strcmp(test,"0\r\n\r\n")==0){
            //Chunk sending EOS
            printf("Chunked Response, END\n");
            break;
        }
        
    }
    //printf("End Response\n");
    fullBuff[bytesRecv+1]='\0';
    //printf("Full Result:\n%s",fullBuff);
    //close
    SSL_shutdown(conn);
    shutdown(sockfd,SHUT_RDWR);
    return fullBuff;


}


char* makeHTTPRequest(char* hostname ,char* GET_, char* POST){
    char ipadr[INET6_ADDRSTRLEN];
    char* service="http";
    struct addrinfo hints, *response,*res_itr;
    memset(&hints,0,sizeof(hints));
    //hints.ai_family=AF_INET;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    int status =getaddrinfo(hostname,service,&hints,&response);
    if(status!=0){
        printf("Lookup Error:\n");
        printf("%s: %i\n",gai_strerror(status),status);
        exit(1);
    }
    int sockfd = socket(response->ai_family,response->ai_socktype,response->ai_protocol);
    if(connect(sockfd,response->ai_addr,response->ai_addrlen)==-1){
        printf("Connection Error\n");
    }
    //Connection Made Send/receive
    //char* testMsg="GET / HTTP/1.1\r\nHost: freedns.afraid.org:80\r\nUser-Agent: DDNS Client Prototype\r\n\r\n";
    char* testMsg = calloc(1,4096);
    strcat(testMsg,"GET ");
    if(strcmp(GET_,"")==0)
        GET_="/";
    strcat(testMsg,GET_);
    strcat(testMsg," HTTP/1.1\r\nHost: ");
    strcat(testMsg,hostname);
    strcat(testMsg,":80");
    // strcat(testMsg,"");
    strcat(testMsg,"\r\nUser-Agent: DDNS Client Prototype\r\n\r\n");
    int msgLen = strlen(testMsg);
    int bytesSent=0;
    bytesSent+=send(sockfd,testMsg,msgLen,0);
    if(bytesSent==-1){
        //printf("Send Error");
    }
    while(bytesSent<msgLen){
        //continueMsg??
        bytesSent=msgLen;
    }
    //receive data
    int tempBuffLen=Goodput;//Goodput is 1448
    char* fullBuff=malloc(memChunk256k);
    short int chunkCount=1;
    char* tempBuff=malloc(tempBuffLen+1);//+1 for nullterm
    tempBuff[tempBuffLen+1]='\0';
    int bytesRecv=0;
    int recvInt=0;
    while((recvInt=recv(sockfd,tempBuff,tempBuffLen,0))){
        
        if(recvInt==-1){
            //printf("Receive Error\n");
            exit(3);
        }
        bytesRecv+=recvInt;
        //printf("Received: %i Bytes(Total: %i).\n",recvInt,bytesRecv);
        //printf("|%s|",tempBuff);
        if(bytesRecv>=memChunk256k*chunkCount){
            //allocate memory for a new chunk, copy old memory into it then free old memory.
            fullBuff=realloc(fullBuff,memChunk256k*(++chunkCount));        
        }
        memcpy(fullBuff+(bytesRecv-recvInt),tempBuff,recvInt);//dest, src,  len
        //chunked response termination
        if(recvInt>=7){
            char test[8];
            memcpy(test,tempBuff+(recvInt-7),7);
            if(strcmp(test,"\r\n0\r\n\r\n")==0){
                //Chunk sending EOS
                //printf("Chunked Response, END\n");
                break;
            }
        }
    }
    //printf("End Response\n");
    fullBuff[bytesRecv+1]='\0';
    //printf("Full Result:\n%s",fullBuff);
    //close
    shutdown(sockfd,SHUT_RDWR);
    return fullBuff;
}