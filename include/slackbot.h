#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

/* 
 * IRC Session contect struct 
 */
typedef struct { 
    char *channel; 
    char *nick; 
}irc_ctx_t;

void slack_handler_connect(irc_session_t*, const char*, const char*, const char **, unsigned int);
void slack_handler_join(irc_session_t*, const char*, const char*, const char **, unsigned int);

#endif 
