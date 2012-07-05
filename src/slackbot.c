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

const char *argp_program_version = "slackbot v0.1.0";
const char *argp_program_bug_address = "<slackwill@csh.rit.edu>"; 
const char doc[] = 
"slackbot is a chatbot designed for the Computer Sciense house.\n"
"It's uses extend outside of our private channels for both personal\n"
"and public IRC management.\n";

static struct argp_option options[] = { 
    {"port", 'p', "PORT", 0, "Port of the IRC server"},
    {"host", 'h', "HOST", 0, "Host address of IRC server"},
    {"nick", 'n', "NICK", 0, "Nickname of the chat bot in channel"}, 
    {"user", 'u', "USER", 0, "User who started the bot"}, 
    {"name", 'N', "NAME", 0, "Real name, preferably starter of bot"},
    {"channel", 'c', "CHANNEL", 0, "Initial channel to join"}, 
    { 0 } //Terminating Option
}; 

static char args_doc[] = ""; // TODO: Add argument documentation for every entry

struct arguments { 
    char *args[2]; // increase with every added argument
    char *host; 
    char *nick; 
    char *user; 
    char *name; 
    char *channel;
    int port; 
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state) { 
    struct arguments *arguments = state->input; 

    switch(key) { 
        /* Handle any character options here seperately. 
         ex. case 'q': set arguments x to y
         Also handle any ARGP_KEY_ARG's as well. */

        case 'h': 
            arguments->host = arg; 
            break; 
        case 'p': 
            arguments->port = atoi(arg); 
            break;
        case 'n': 
            arguments->nick = arg; 
            break;
        case 'u': 
            arguments->user = arg; 
            break; 
        case 'N': 
            arguments->name = arg; 
            break;
        case 'c': 
            arguments->channel = arg;
            break;

        case ARGP_KEY_ARG: 
            if(state->arg_num >= 0)  //increment/decrement based on args
                /* Too many arguments */
                argp_usage(state); 
            arguments->args[state->arg_num] = arg; 
            break; 

        case ARGP_KEY_END: 
            if (state->arg_num < 0) //increment/decrement based on required args
                /* Not enough arguments */
                argp_usage(state); 
            break; 

        default: 
            return ARGP_ERR_UNKNOWN; 
    }
    return 0; 
}

static struct argp argp = {options, parse_opt, args_doc, doc };

int 
main(int argc, char *argv[]) { 

    struct arguments arguments; 
    arguments.host = "localhost"; 
    arguments.port = 6667;
    arguments.nick = "slackbot";
    arguments.user = "None";
    arguments.name = "slackbot";
    arguments.channel = "#slackbot";

    /* Argurment parse constants defined in the 
       slackbot.h header. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments); 

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

    ctx.channel = arguments.channel;
    ctx.nick = arguments.nick; 
    irc_set_ctx(session, &ctx); 

    /* Uses the arguments given with argp, addend new ones 
     * to the options and arguments struct, and add handlers 
     * to the option parse function, and */
    irc_connect(
            session, 
            arguments.host,
            arguments.port, 
            "", //TODO: Add password support
            arguments.nick, 
            arguments.user, 
            arguments.name); 

    if(irc_is_connected(session)) { 
        printf("Connected\n"); 
        irc_run(session);
    }

    irc_disconnect(session);

    return 0; 
}
