#include <stdio.h>
#include <stddef.h> 
#include <stdarg.h> 
#include <stdlib.h>
#include <string.h> 

#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

#include <slackbot.h> 


int 
main(int argc, char *argv[]) { 
    
    irc_callbacks_t callbacks;
    irc_session_t *session; 
 
    //clear the callbacks just in case 
    memset(&callbacks, 0, sizeof(callbacks)); 
    session = irc_create_session(&callbacks); 

    callbacks.event_connect = slack_handler_connect; 

    irc_connect(
            session, 
            "irc.freenode.net",
            6667, 
            "", 
            "slackbot", 
            "slackbot", 
            "Slackwill"); 

    if(irc_is_connected(session)) { 
        printf("Connected, Destroying\n"); 
        irc_run(session);
    }

    irc_disconnect(session);

    return 0; 
}

void
slack_handler_connect(
        irc_session_t *session, 
        const char *event, 
        const char *origin, 
        const char **params, 
        unsigned int count) { 
    printf("Connected To IRC Server"); 
    printf("Can you hear me?");
}

