/**
 * This file is part of slackbot.
 *
 * slackbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * slackbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with slackbot.  If not, see <http://www.gnu.org/licenses/>.kh
 *
 */

/**
 * @ingroup slackbot
 * @file slackbot.c
 * @author William Dignazio <slackwill@csh.rit.edu>
 */

#include <stdio.h>
#include <stddef.h> 
#include <stdarg.h> 
#include <stdlib.h>
#include <string.h> 
#include <syslog.h> 
#include <unistd.h>
#include <argp.h>

#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

#include <slackbot.h> 

static struct argp argp = { 0, 0, 0, doc }; 

int 
main(int argc, char *argv[]) { 

    //Argurment parse constants defined in the 
    //slackbot.h header. 
    argp_parse(&argp, argc, argv, 0, 0, 0); 

    /** 
     * slackbot will display all of its logging 
     * in the syslog file, //TODO: implement a verbose
     * flag so that is can display to the terminal, 
     * syslog, or both.
     */
    openlog("slackbot", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); 
    syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_NOTICE), "Slackbot started "
        "by User %d", getuid()); 

    irc_callbacks_t callbacks;
    irc_session_t *session; 
    irc_ctx_t ctx;  
    
    //clear the callbacks just in case 
    memset(&callbacks, 0, sizeof(callbacks)); 
    
    callbacks.event_connect = slack_handler_connect; 
    callbacks.event_join = slack_handler_join; 

    session = irc_create_session(&callbacks); 
    if(!session) { 
        printf("Failed to Create Session"); 
        syslog(LOG_INFO, "Failed to create slackbot session");
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
    syslog(LOG_INFO, "Connecting To IRC Server");
    irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session); 
    irc_cmd_join(session, ctx->channel, NULL);
    //TODO: Add password handling from ctx object 
}


void 
slack_handler_join(
        irc_session_t *session, 
        const char *event, 
        const char *origin, 
        const char **params, 
        unsigned int count) { 
    irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session); 
    syslog(LOG_INFO, "Connected to channel %s", ctx->channel);
}
