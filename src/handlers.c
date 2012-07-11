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
 * @file handlers.c
 * @author William Dignazio <slackwill@csh.rit.edu>
 */

#include <stddef.h> 
#include <syslog.h>
#include <string.h>

#include <slackbot.h> 

void
slack_handler_connect(
        irc_session_t *session, 
        const char *event, 
        const char *origin, 
        const char **params, 
        unsigned int count) { 
    syslog(LOG_INFO, "Connecting To IRC Server");
    irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session); 
    irc_cmd_join(session, ctx->channel, NULL);
    //TODO: Add password handling from ctx object 
}


void 
slack_handler_join(
        irc_session_t *session, 
        const char *event, 
        const char *origin, 
        const char **params, 
        unsigned int count) { 
    irc_ctx_t *ctx = (irc_ctx_t *)irc_get_ctx(session); 
    syslog(LOG_INFO, "%s connected to channel %s", origin, ctx->channel);
    const char *msg = "Good news everyone! %s has joined the channel!"; 
    char buf[strlen(msg) + strlen(origin)];
    sprintf(buf, msg, origin); 
    irc_cmd_msg(session, ctx->channel, buf); 
}
