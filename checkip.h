char* getCurrentIP();
int checkIPUpdate();
typedef enum{
    WTFIP,           //wtfismyip.com/text + https!
    DYNDNS,         //checkip.dyndns.com
    WIMIPDHOST       //whatismyip.host
} IPSources;
char* LAST_IP_CHECK;