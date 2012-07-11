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
 * @title slackbot
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

#include <modules.h>
#include <slackbot.h> 

int DEBUG = 0; 
int NO_VERIFY = 0; 

config_t config;

const char *argp_program_version = "slackbot v0.1.0";
const char *argp_program_bug_address = "<slackwill@csh.rit.edu>"; 
const char doc[] = 
"slackbot is a chatbot designed for the Computer Sciense house.\n"
"It's uses extend outside of our private channels for both personal\n"
"and public IRC management.\n";

static struct argp_option options[] = { 
    {"debug", 'd', 0, 0, "Enable debug output"}, 
    {"port", 'p', "PORT", 0, "Port of the IRC server"},
    {"host", 'h', "HOST", 0, "Host address of IRC server, add a # to the beginning of the address for ssl connections"},
    {"nick", 'n', "NICK", 0, "Nickname of the chat bot in channel"}, 
    {"user", 'u', "USER", 0, "User who started the bot"}, 
    {"name", 'N', "NAME", 0, "Real name, preferably starter of bot"},
    {"channel", 'c', "CHANNEL", 0, "Initial channel to join"}, 
    {"ssl_no_verify", 'X', 0, 0, "No verification for SSL"}, 
    {"ldap_host", 'H', "LDAP_HOST", 0, "Host of ldap server"}, 
    {"ldap_port", 'P', "LDAP_PORT", 0, "Specify ldap port"}, 
    { 0 } //Terminating Option
}; 

static char args_doc[] = 
"debug: takes no arguments, enables additional debug output.\n"
"port: port slackbot connects to on IRC server, default is 6667.\n"
"host: addresss of the server to connect to, default is localhost.\n"
"nick: nickname of the slackbot in channel.\n"
"user: username of the starter of the bot, default is None.\n" 
"channel: channel to initially join, default is slackbot.\n"
"ssl_no_verify: takes no arguments, disables ssl verification.\n"
"ldap_host: address of the ldap server to connect to.\n"
"ldap_port: port of the ldap server to connet to.\n";

static error_t
parse_opt (int key, char *arg, struct argp_state *state) { 
    arguments *arguments = state->input; 

    switch(key) { 
        /* Handle any character options here seperately. 
         ex. case 'q': set arguments x to y
         Also handle any ARGP_KEY_ARG's as well. */

        case 'd': 
            DEBUG = 1; 
            break;
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
        case 'X': 
            NO_VERIFY = 1;
            break;
        case 'H': 
            arguments->ldap_host = arg; 
        case 'P': 
            arguments->ldap_port = atoi(arg); 

        case ARGP_KEY_ARG: 
            if((int)(state->arg_num) >= 0)  //increment/decrement based on args
                /* Too many arguments */
                argp_usage(state); 
            arguments->args[state->arg_num] = arg; 
            break; 

        case ARGP_KEY_END: 
            if ((int)(state->arg_num) < 0) //increment/decrement based on required args
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
    arguments.ldap_host = "localhost"; 
    arguments.ldap_port = 389;
    arguments.config = "~/.config/slackbot.cfg";

    irc_callbacks_t callbacks;
    irc_session_t *session; 
    irc_ctx_t ctx;  

    
    /** 
     * slackbot will display all of its logging 
     * in the syslog file, //TODO: implement a verbose
     * flag so that is can display to the terminal, 
     * syslog, or both.
     */
    openlog("slackbot", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); 
    syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_NOTICE), "Slackbot started "
        "by User %d", getuid()); 


    /* Load Configuration File, takes preference over the 
     * default configurations given 
     */
    syslog(LOG_INFO, "Parsing configuration"); 
    if(arguments.config) { 
        syslog(LOG_INFO, "Reading configuration from %s\n",
            arguments.config); 
        config_init(&config); 
        if(config_read_file(&config, "slackbot.cfg") != CONFIG_TRUE)
            printf("failed to parse configuration\n");
    }
   
    if(config_lookup_string(&config, "host", (const char **)&arguments.host))
        syslog(LOG_INFO, "Configuration parsed IRC host %s", arguments.host); 
    if(config_lookup_int(&config, "port", &arguments.port))
        syslog(LOG_INFO, "Configuration parsed IRC port %d", arguments.port); 
    if(config_lookup_string(&config, "channel", (const char **)&arguments.channel))
        syslog(LOG_INFO, "Configuration parsed IRC channel %s", arguments.channel);
    if(config_lookup_string(&config, "name", (const char **)&arguments.name))
        syslog(LOG_INFO, "Configuration parsed IRC name %s", arguments.name); 
    if(config_lookup_string(&config, "nick", (const char **)&arguments.nick)) 
        syslog(LOG_INFO, "Configuration parsed IRC nick %s", arguments.nick); 
    if(config_lookup_string(&config, "user", (const char **)&arguments.user))
        syslog(LOG_INFO, "Configuration parsed IRC user %s", arguments.user); 
    if(config_lookup_bool(&config, "ssl_no_verify", &NO_VERIFY))
        syslog(LOG_INFO, "Configuration parsed SSL options %s", NO_VERIFY ? 
             "No SSL verication" : "Verify SSL"); 

    /* Argurment parse constants defined in the 
       slackbot.h header. 
       They take precedence over all the others. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments); 

    /* Load all the modules of the program */ 
    load_all_modules(&arguments);

    //clear the callbacks just in case 
    memset(&callbacks, 0, sizeof(callbacks)); 
   
    /* Insert all the callback events here */
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

    /* Set IRC options that could not be set outright. */
    if(DEBUG) { 
        irc_option_set(session, LIBIRC_OPTION_DEBUG); 
        syslog(LOG_INFO, "Enabled Debug Output"); 
    }
    if(NO_VERIFY) { 
        irc_option_set(session, LIBIRC_OPTION_SSL_NO_VERIFY); 
        syslog(LOG_INFO, "Disabled SSL verification"); 
    }

    syslog(LOG_INFO, "Connecting with: "); 
    syslog(LOG_INFO, "Host: %s", arguments.host); 
    syslog(LOG_INFO, "Port: %d", arguments.port);
    syslog(LOG_INFO, "Nick: %s", arguments.nick); 
    syslog(LOG_INFO, "User: %s", arguments.user); 
    syslog(LOG_INFO, "Name: %s", arguments.name);
    syslog(LOG_INFO, "Channel: %s", arguments.channel);

    /* Uses the arguments given with argp, addend new ones 
     * to the options and arguments struct, and add handlers 
     * to the option parse function, and */
    int status = irc_connect(
            session, 
            arguments.host,
            arguments.port, 
            "", //TODO: Add password support
            arguments.nick, 
            arguments.user, 
            arguments.name); 
    printf("Slackbot status: %d\n", status); 

    if(irc_is_connected(session)) { 
        printf("Connected\n"); 
        int status = irc_run(session);
        if(status != 0) { 
            syslog(LOG_INFO, "IRC run failed uncleanly"); 
            printf("Session run error: %d\n", status); 
        }

    }

    irc_disconnect(session);

    return 0; 
}
