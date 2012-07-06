#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

/* Struct for handling modules, this 
 * defines their object from the aspect of
 * the main program. It provides a semi-
 * anonymous interface so that a module 
 * does not have to be directly integrated
 * into the main program. 
 */
typedef struct module_t { 
    void (*routine) (irc_session_t *session); 
} module_t;

/* List of modules types, insert module handlers here.
 * The initial size of this module at creation is determined
 * by the number of modules currently developed. If more 
 * modules are to be added later, or at runtime, this 
 * array must be rebuilt. 
 */
module_t *modules[1]; 

/* Argument struct for handling input from cli */
struct arguments { 
    char *args[2]; // increase with every added argument
    char *host; 
    char *nick; 
    char *user; 
    char *name; 
    char *channel;
    int port; 
    char *ldap_host; 
    int ldap_port;
};


/* IRC Session connect struct */
typedef struct { 
    char *channel; 
    char *nick; 
}irc_ctx_t;

void slack_handler_connect(irc_session_t*, const char*, const char*, const char **, unsigned int);
void slack_handler_join(irc_session_t*, const char*, const char*, const char **, unsigned int);

int load_modules(); 

#if _LDAP
/* If the LDAP module is compiled into the bot,
 * then we want to create the appropriate external 
 * variables for it. 
 */
#include <ldap.h>
extern LDAP* ldap_conn; 

#endif 



#endif 
