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
 * @file event.c
 * @author William Dignazio <slackwill@csh.rit.edu>
 */

#include <stdlib.h>
#include <syslog.h> 
#include <pthread.h> 

#include <slackbot.h> 
/* Main event queue objects */ 
slack_event *EVENT_QUEUE_FRONT;
slack_event *EVENT_QUEUE_BACK; 

pthread_mutex_t lock; 

/** 
 * Asynchronous wrap around the push function. 
 * This can be used to create an indiviual thread 
 * that will not halt the current process, while 
 * still pushing to the event queue. 
 */ 
void 
push_async(slack_event *event) { 
   pthread_t ept; 
   if(pthread_create(&ept, NULL, &push, NULL))
        syslog(LOG_INFO, "Error pushing event"); 
}

/**
 * After ensuring that the event queue is 
 * not in use, the push procedure will set the tail 
 * of the event queue to the provided event. 
 */ 
void
push(slack_event *event) {
    /* Wait until we get a lock on the mutex */ 
    pthread_mutex_lock(&lock); 

    if(EVENT_QUEUE_BACK != EVENT_QUEUE_FRONT) { 
        EVENT_QUEUE_BACK->next = event; 
        EVENT_QUEUE_BACK = event; 
    } else { 
        if(EVENT_QUEUE_FRONT != NULL) { 
            EVENT_QUEUE_FRONT->next = event; 
            EVENT_QUEUE_BACK = event; 
        } else { 
            EVENT_QUEUE_FRONT = event; 
        }
    }

    pthread_mutex_unlock(&lock); 
}

/** 
 * Simple init function to setup the mutex and other
 * necessary data for the event queue system. 
 */ 
void
init_event_queue() { 
    if(pthread_mutex_init(&lock, NULL) != 0) { 
        syslog(LOG_INFO, "Mutex initialization failed"); 
        exit(1); 
    }
}

/**
 * Called when the bot needs to broadcast
 * an event onto the event queue. This essentially 
 * means that this event has been slated to be 
 * recieved by all the modules that have been 
 * loaded. 
 */ 
int 
broadcast(slack_event *event) { 
    return 0; 
 }
