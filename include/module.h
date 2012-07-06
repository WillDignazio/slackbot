/**
 * If there are to be extra modules for the 
 * bot, whether they are new or just want to 
 * be included in the compilation, they must
 * use this header. 
 *
 * This header defines the required functions 
 * that a slackbot module must have. It also 
 * provides an interface for the main slackbot 
 * program to hook into. 
 */

#ifndef MODULE_GAURD_H
#define MODULE_GAURD_H

#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

static int module_init( irc_session_t *session ); 
static int module_exit(); 
static void module_routine(); 

#endif

