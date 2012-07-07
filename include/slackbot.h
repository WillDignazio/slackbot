#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

/* Argument struct for handling input from cli */
typedef struct arguments { 
    char *args[2]; // increase with every added argument
    char *host; 
    char *nick; 
    char *user; 
    char *name; 
    char *channel;
    int port; 
    char *ldap_host; 
    int ldap_port;
}arguments;

/* IRC Session connect struct */
typedef struct { 
    char *channel; 
    char *nick; 
}irc_ctx_t;

void slack_handler_connect(irc_session_t*, const char*, const char*, const char **, unsigned int);
void slack_handler_join(irc_session_t*, const char*, const char*, const char **, unsigned int);

int load_all_modules(); 

/* Individual module initializer functions 
 * Defined in their respective module files. */
int load_ldap_module(arguments*); 

#endif 
