#include <stdlib.h>
#include <unistd.h>

#include "pbx.h"
#include "server.h"
#include "debug.h"

#include "signal.h"
#include <sys/socket.h>
#include "pthread.h"
#include "csapp.h"
#include "helper.h"

void SIGHUP_handler();
static void terminate(int status);

/*
 * "PBX" telephone exchange simulation.
 *
 * Usage: pbx <port>
 */
int main(int argc, char* argv[]){
    // Option processing should be performed here.
    // Option '-p <port>' is required in order to specify the port number
    // on which the server should listen.

    // Parse port number
    char *opt_string = "p:";
    char *port_num;
    if(getopt(argc, argv, opt_string) < 0) {
        fprintf(stderr, "Usage: bin/pbx -p <port>\n");
        exit(EXIT_FAILURE);
    }
    else if(optarg) {
        if(is_integer(optarg)) {
            port_num = optarg;
            port_num = port_num;
        } else {
            fprintf(stderr, "Usage: bin/pbx -p <port>\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Usage: bin/pbx -p <port>\n");
        exit(EXIT_FAILURE);
    }

    // Perform required initialization of the PBX module.
    debug("Initializing PBX...");
    pbx = pbx_init();



    // TODO: Set up the server socket and enter a loop to accept connections
    // on this socket.  For each connection, a thread should be started to
    // run function pbx_client_service().   In addition, you should install
    // a SIGHUP handler, so that receipt of SIGHUP will perform a clean
    // shutdown of the server.



    // Install SIGHUP handler
    struct sigaction act;
    act.sa_handler = &terminate;
    sigaction(SIGHUP, &act, NULL);

    // Open server socket
    int listen_fd, *conn_fdp;
    socklen_t client_len;
    struct sockaddr_storage client_addr;
    pthread_t tid;

    listen_fd = Open_listenfd(port_num);
    // Listen for connections and create new thread when one occurs
    while(1) {
        client_len = sizeof(struct sockaddr_storage);
        conn_fdp = Malloc(sizeof(int));
        *conn_fdp = Accept(listen_fd, (SA *)&client_addr, &client_len);
        Pthread_create(&tid, NULL, pbx_client_service, conn_fdp);
    }

    // Error occurred
    fprintf(stderr, "You have to finish implementing main() "
        "before the PBX server will function.\n");

    terminate(EXIT_FAILURE);
}

void SIGHUP_handler() {
    terminate(EXIT_SUCCESS);
}

/*
 * Function called to cleanly shut down the server.
 */
static void terminate(int status) {
    debug("Shutting down PBX...");
    pbx_shutdown(pbx);
    debug("PBX server terminating");
    exit(status);
}