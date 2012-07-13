#ifndef MODULES_GAURD_H
#define MODULES_GAURD_H 

#include <ldap.h> 

typedef struct slack_ldap_event_t { 
    LDAPMessage result; 

void slack_ldap_search(const char *); 

#endif
