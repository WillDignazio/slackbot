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
#include <slackbot.h>

#define MODULE_OK 0
#define MODULE_INIT_FAIL 1 
#define MODULE_EXIT_FAIL 2

static int module_init(arguments *arg); 
static int module_cleanup(); 

#endif

