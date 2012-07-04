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
    irc_ctx_t ctx;  
    
    //clear the callbacks just in case 
    memset(&callbacks, 0, sizeof(callbacks)); 
    
    callbacks.event_connect = slack_handler_connect; 
    callbacks.event_join = slack_handler_join; 

    session = irc_create_session(&callbacks); 
    if(!session) { 
        printf("Could Not Create Session"); 
        return 1; 
    }

    ctx.channel = "#slackbot"; // TODO: Change to argument
    ctx.nick = "slackbot"; //TODO: Change to argument
    irc_set_ctx(session, &ctx); 

    irc_connect(
            session, 
            "localhost",
            6667, 
            "", 
            "slackbot", 
            "slackbot", 
            "Slackwill"); 

    if(irc_is_connected(session)) { 
        printf("Connected\n"); 
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
    //printf("Connected To IRC Server"); 
    irc_cmd_join(session, "#slackbot", NULL);
}


void 
slack_handler_join(
        irc_session_t *session, 
        const char *event, 
        const char *origin, 
        const char **params, 
        unsigned int count) { 
    printf("Joined IRC Channel"); 
}
