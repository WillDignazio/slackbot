#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

const char *argp_program_version = "slackbot v0.1.0";
const char *argp_program_bug_address = "<slackwill@csh.rit.edu>"; 

/* 
 * IRC Session contect struct 
 */
typedef struct { 
    char *channel; 
    char *nick; 
}irc_ctx_t;

void slack_handler_connect(irc_session_t*, const char*, const char*, const char **, unsigned int);
void slack_handler_join(irc_session_t*, const char*, const char*, const char **, unsigned int);

const char doc[] = 
"slackbot is a chatbot designed for the Computer Sciense house.\n"
"It's uses extend outside of our private channels for both personal\n"
"and public IRC management.\n";

#endif 
