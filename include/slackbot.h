#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

#include <semaphore.h> 

#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

#include <libconfig.h> 

extern config_t config;

/* All encompassing event for the event 
 * stack structure */ 
typedef struct slack_event { 
   irc_event_callback_t callback; 
   irc_event_dcc_chat_t dcc; 
   irc_event_dcc_send_t dccsend; 
   irc_eventcode_callback_t ecode; 
   struct slack_event **supplement; 
   struct slack_event *next; 
} slack_event; 

/* Argument struct for handling input from cli */
typedef struct arguments { 
    char *args[2]; // increase with every added argument
    char *config;
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

/*#################################
  #         Event Handlers        #
  #################################*/ 
void slack_handler_connect(irc_session_t*, const char*, const char*, const char **, unsigned int);
void slack_handler_join(irc_session_t*, const char*, const char*, const char **, unsigned int);

void push(slack_event*); 
int broadcast(slack_event*); 

#endif 
