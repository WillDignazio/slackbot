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
#include <string.h> 
#include <syslog.h> 

#include <ldap.h>

#include <module.h>
#include <slackbot.h> 

LDAP *ldap;

/** 
 * Initializes ldap module, not explicitly 
 * required, just used to standardize future modules. 
 */
int
module_init( arguments *args ) { 
    syslog(LOG_INFO, "LDAP Module initalizing"); 
    char buf[100];//oh god I'm lazy I know, TODO: fix
    sprintf(buf, "%s:%d", args->ldap_host, args->ldap_port); 
    ldap_initialize(&ldap, buf);
    return MODULE_OK; 
}

/**
 * Same deal, not explicitly required, just used to 
 * make standared modules for easy cleanup.
 */
int 
module_cleanup() { 
    return MODULE_OK; 
}

/** 
 * While in this case it is redundant, if later on there 
 * are modules that have constantly running routines, they 
 * will need a better way to be started. 
 */
int 
load_ldap_module( arguments *args ) { 
    module_init(args); 
    return MODULE_OK;
}
