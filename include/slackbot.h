#ifndef SLACKBOT_GAURD_H 
#define SLACKBOT_GAURD_H 

#include <stddef.h> 
#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

irc_session_t session; 
const char port; 
const char *passwd; 
const char *nick; 
const char *username; 
const char *realname; 

#endif 
