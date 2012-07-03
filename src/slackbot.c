#include <slackbot.h> 

int main(int argc, char *argv[]) { 
    int status = irc_connect(
            &session, 
            "skynet.csh.rit.edu", 
            6697, 
            "", 
            "slackbot", 
            "slackbot", 
            "Slackwill");

    return 0; 
}

