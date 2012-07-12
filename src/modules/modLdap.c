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
    char *basednstr; 
    char *filterstr; 
    char *eventstr; 
    struct ldap_query_t *next; 
};

/**
 * Parses the configuration file, specifically the 
 * ldap section. This function will produce a tree, 
 * or really a linked list, tree sounds cooler, of 
 * ldap_query_t structures. This will allow a series 
 * of events to happen sequentially just by adding a
 * few lines to the configuration file. The returned 
 * node is the head of the tree/list.
 */
struct ldap_query_t
build_query_tree() { 
    syslog(LOG_INFO, "Building query event tree..."); 
    static struct ldap_query_t head; 
    struct ldap_query_t *tail = &head; 
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

            /* Each of the string must have a +1 for dot operator */
            char basednstr[strlen(query_param) + 8]; //length of 'basedn'
            sprintf(basednstr, "%s.%s", query_param, "basedn"); 
            syslog(LOG_INFO, "Parsing %s", basednstr); 
            char filterstr[strlen(query_param) + 6]; //length of 'filter'
            sprintf(filterstr, "%s.%s", query_param, "filter"); 
            syslog(LOG_INFO, "Parsing %s", filterstr); 
            char eventstr[strlen(query_param) + 5]; //length of 'event'
            sprintf(eventstr, "%s.%s", query_param, "event"); 
            syslog(LOG_INFO, "Parsing %s", eventstr); 

            syslog(LOG_INFO, "Building query node..."); 
            config_lookup_string(&config, basednstr, &(tail->basednstr)); 
            config_lookup_string(&config, filterstr, &(tail->filterstr)); 
            config_lookup_string(&config, eventstr, &(tail->eventstr)); 
            tail->next = malloc(sizeof(struct ldap_query_t));

            /* By this point the new node, TODO: write a failure method 
             * if we don't make it to this point. I beleive the only 
             * alternative to correct configuration is segfault :(
             */
            syslog(LOG_INFO, "Success, New Node:"); 
            syslog(LOG_INFO, "--->basedn = %s", tail->basednstr); 
            syslog(LOG_INFO, "--->filter = %s", tail->filterstr); 
            syslog(LOG_INFO, "--->event = %s", tail->eventstr); 
            /* Move onto the next node */ 
            tail = tail->next; 

            i++;
        } else { 
            syslog(LOG_INFO, "Parsed all ldap queries.");
            i = -1; 
            tail->next = NULL;
        } 
    } while(i != -1);

    return head;
}

/**
 * Debug function meant to display the query tree 
 * onto a log file or syslog. 
 */
void
log_query_tree(struct ldap_query_t *node) { 
    if(node->next != NULL) { 
        syslog(LOG_INFO, "/--NODE: %s, %s, %s", 
            node->basednstr, node->filterstr, node->eventstr); 
        return log_query_tree(node->next); 
    }
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
    struct ldap_query_t query = build_query_tree(); 
    log_query_tree(&query);  

    return 0;
}
