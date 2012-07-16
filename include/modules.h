#ifndef MODULES_GAURD_H
#define MODULES_GAURD_H 

#include <ldap.h> 

typedef struct slack_ldap_query_t { 
    char *basednstr; 
    char *filterstr; 
    char *eventstr; // Not an actual event type
    struct slack_ldap_query_t *next; 
} slack_ldap_query_t;

void slack_ldap_search(const char *); 

#endif
