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

struct ldap_query_t { 
    char *basedn; 
    char *filter; 
    char *event; 
    struct ldap_query_t *next; 
};

struct ldap_query_t
build_query_tree() { 
    syslog(LOG_INFO, "Building query event tree..."); 
    static struct ldap_query_t head; 
    struct ldap_query_t *next; 
    config_setting_t *setting = config_lookup(&config, "ldap.queries"); 
    int i = 0; 
    do { /* The caveat, and requirement, is that there be at least
            one ldap query parameter, otherwise you just shouldn't
            include the ldap module...*/
        char *query = config_setting_get_string_elem(setting, i);
        if(query != NULL) { 
            char query_param[strlen("ldap.query") //GCC/clang will optimize
                + strlen(query)+1]; // +1 for dot op
            sprintf(query_param, "ldap.query.%s", query);
            syslog(LOG_INFO, "Parsed LDAP query %s", query_param); 
            syslog(LOG_INFO, "Building LDAP query node..."); 
            //TODO: Build it. 
            i++;
        } else { 
            syslog(LOG_INFO, "Parsed all ldap queries.");
            i = -1; 
        } 
    } while(i != -1);

    return head;
}


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

    syslog(LOG_INFO, "Building LDAP query tree..."); 
    build_query_tree(); 

    return 0;
}
