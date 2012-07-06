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
 * @ingroup slackbot_modules
 * @author: Will Dignazio 
 */

#include <stdio.h>
#include <stddef.h> 
#include <stdarg.h> 
#include <stdlib.h>
#include <string.h> 
#include <syslog.h> 
#include <unistd.h>
#include <argp.h>

#include <module.h>
#include <slackbot.h> 

LDAP *ldap;

/**
 * Main routine for the ldap function, should when 
 * queried handle all ldap searches and return 
 * patterns. 
 */
void
module_routine() { 

}

int
module_init( irc_session_t *session ) { 
    syslog(LOG_INFO, "LDAP Module initalizing"); 
    //TODO: add ldap module handling logic 
    return 0; 
}

int module_exit() { 
    return 0; 
}
