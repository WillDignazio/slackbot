#ifndef MODULES_GAURD_H
#define MODULES_GAURD_H 

#include <semaphore.h> 
#include <ldap.h> 

extern sem_t esem; // Event semaphore declared in slacbot.c
void (*slack_event)(int,int,int,int); 

typedef struct slack_event_t { 
    struct slack_event_t *next; 
    void (*event_fp) ; 
}slack_event_t; 

typedef struct slack_ldap_query_t { 
    char *basednstr; 
    char *filterstr; 
    char *eventstr; // Not an actual event type
    struct slack_ldap_query_t *next; 
} slack_ldap_query_t;

void slack_ldap_search(const char *); 

#endif
