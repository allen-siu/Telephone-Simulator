/*
 * "PBX" server module.
 * Manages interaction with a client telephone unit (TU).
 */
#include <stdlib.h>

#include "debug.h"
#include "pbx.h"
#include "server.h"

#include "pthread.h"
#include "csapp.h"

#include "helper.h"



void parse_command(TU *user, char *cmd) {
    // Find first '\0' or ' '
    char first_arg = *cmd;
    if(first_arg == 'p') {
        if(same_string(cmd, "pickup") && strlen(cmd) == strlen("hangup")) 
            tu_pickup(user);
        // else
        //     debug("Command unrecognizable: %s%s", cmd, "END");
    }
    else if(first_arg == 'h') {
        if(same_string(cmd, "hangup") && strlen(cmd) == strlen("hangup")) 
            tu_hangup(user);
        // else
        //     debug("Command unrecognizable: %s%s", cmd, "END");
    }
    else if(first_arg == 'd') {
        // if cmd has "dial " to signal that it may have the required argument
        if(same_string(cmd, "dial") && *(cmd + strlen("dial")) == ' ') {
            int ext;
            char *ext_arg_addr = cmd + strlen("dial");
            if((ext = atoi(ext_arg_addr)))
                //tu_dial(user, get_tu_from_ext(ext));
                pbx_dial(pbx, user, ext);
        }
        // else
        //     debug("Command unrecognizable: %s%s", cmd, "END");
    }
    else if(first_arg == 'c') {
        if(same_string(cmd, "chat")) 
            tu_chat(user, (cmd + strlen("chat")));
        // else
        //     debug("Command unrecognizable: %s%s", cmd, "END");
    }
    else {
        // debug("Command unrecognizable: %s%s", cmd, "END");
    }

}


/*
 * Thread function for the thread that handles interaction with a client TU.
 * This is called after a network connection has been made via the main server
 * thread and a new thread has been created to handle the connection.
 */
#if 1
void *pbx_client_service(void *arg) {
    // TO BE IMPLEMENTED
    int fd = *(int *)arg;
    Pthread_detach(pthread_self());
    free(arg);

    // Add tu to server
    TU *user = tu_init(fd);
    if(pbx_register(pbx, user, fd)) {
        // error
    }
    print_tu_state(user);

    // Service loop
    while(1) {
        // Get input from user until "\r\n" occurs
        char *cmd;
        size_t cmd_len;
        FILE *mem_stream = open_memstream(&cmd, &cmd_len);

        int prev_char_cr = 0;
        int cmd_end = 0;
        while(!cmd_end) {
            char c;
            read(fd, &c, 1);

            // If a character was read, add it to the command string
            if(c > 0) {
                if(prev_char_cr) {
                    if(c == '\n') {
                        cmd_end = 1;
                        break;
                    }
                    else {
                        fprintf(mem_stream, "\r");
                        prev_char_cr = 0;
                    }
                }

                if(c == '\r')
                    prev_char_cr = 1;
                else
                    fprintf(mem_stream, "%c", c);
            }
        }
        fclose(mem_stream);

        parse_command(user, cmd);
    }
}
#endif
