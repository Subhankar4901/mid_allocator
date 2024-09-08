#include<stdio.h>
#include<time.h>
#include<string.h>
#include<syslog.h>
#include "utils.h"
char * LogLevelToString(LogLevel level)
{
    switch(level)
    {
        case INFO:
        return "INFO";
        break;
        case WARNING:
        return "WARNING";
        break;
        case ERROR:
        return "ERROR";
        break;
        default:
        return "UNKNOWN";
        break;
    }
}
void Log_Msg(LogLevel level,const char *msg,const char * file,int line){
    time_t now=time(NULL);
    char * timestamp=ctime(&now);
    timestamp[strlen(timestamp)-1]='\0';
    if(level!=INFO)
    printf("[%s] [%s] [%s:%d]: %s\n",LogLevelToString(level),timestamp,file,line,msg);
    else
    printf("[%s] [%s]: %s\n",LogLevelToString(level),timestamp,msg);
    if(level==ERROR || level==WARNING)
    printf("%s",CHECK_SYS_LOG_MSG);
    Log_Sys(level,msg,file,line);
}
void Log_Sys(LogLevel level,const char * msg,const char * file,int line){
    int pri=-1;
    switch (level)
    {
    case ERROR:
        pri=LOG_ERR;   
        break;
    case WARNING:
        pri=LOG_WARNING;
    case INFO:
        pri=LOG_INFO;
    default:
        pri=LOG_DEBUG;
        break;
    }
    openlog("mid_allocator",LOG_PID | LOG_CONS, LOG_USER);
    if(level==INFO)
    syslog(pri,"[%s] : [%s]",LogLevelToString(level),msg);
    else
    syslog(pri,"[%s] [%s:%d] : %s",LogLevelToString(level),file,line,msg);
    closelog();
}