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
 * @file module.c
 * @author William Dignazio <slackwill@csh.rit.edu>
 */
#include <stddef.h> 
#include <syslog.h>

#include <libircclient.h>
#include <libirc_errors.h>
#include <libirc_events.h>
#include <libirc_options.h>
#include <libirc_rfcnumeric.h>

#include <slackbot.h>

/**
 * Loads all the modules specified. If a new module is
 * added, it needs to be either put in via a function 
 * call, or loaded in automatically with this function.
 */
int 
load_all_modules( arguments *args ) { 

    /*Load LDAP module*/
    load_ldap_module(args);  

    return 0; 
}
