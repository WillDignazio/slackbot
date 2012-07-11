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
#include <libconfig.h>

#include <modules.h>
#include <slackbot.h> 

LDAP *ldap;
const char *uri;
const char *basedn, *binddn;
const char *password;

/**
 * Load the ldap modules, passing in any arguments 
 * given initially, which are to be used in the 
 * ldap module. 
 */
int 
load_ldap_module( arguments *args ) { 
    syslog(LOG_INFO, "LDAP Module initalizing"); 
    config_lookup_string(&config, "ldap.uri", &uri);
    syslog(LOG_INFO, "LDAP URI: %s\n", uri); 
    config_lookup_string(&config, "ldap.binddn", &binddn); 
    syslog(LOG_INFO, "LDAP BINDDN: %s\n", binddn); 
    config_lookup_string(&config, "ldap.password", &password);
    syslog(LOG_INFO, "LDAP PASSWORD: %s\n", password); 

    syslog(LOG_INFO, "Initializing LDAP Connection...%s\n", 
        ldap_err2string(ldap_initialize(&ldap, uri)));
    syslog(LOG_INFO, "Binding to URI...%s\n", 
        ldap_err2string(ldap_simple_bind_s(ldap, binddn, password)));

    return 0;
}
