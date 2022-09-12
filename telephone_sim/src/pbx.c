/*
 * PBX: simulates a Private Branch Exchange.
 */
#include <stdlib.h>

#include "pbx.h"
#include "debug.h"

#include <sys/socket.h>
#include "pthread.h"

#include "csapp.h"
#include "helper.h"


/*
 * Initialize a new PBX.
 *
 * @return the newly initialized PBX, or NULL if initialization fails.
 */
#if 1
PBX *pbx_init() {
    // TO BE IMPLEMENTED
    pbx = malloc(sizeof(PBX));
    for(int i = 0; i < PBX_MAX_EXTENSIONS; i++)
        pbx->tu_list[i] = NULL;

    Sem_init(&pbx->sem, 0, 1);
    return pbx;
}
#endif

/*
 * Shut down a pbx, shutting down all network connections, waiting for all server
 * threads to terminate, and freeing all associated resources.
 * If there are any registered extensions, the associated network connections are
 * shut down, which will cause the server threads to terminate.
 * Once all the server threads have terminated, any remaining resources associated
 * with the PBX are freed.  The PBX object itself is freed, and should not be used again.
 *
 * @param pbx  The PBX to be shut down.
 */
#if 1
void pbx_shutdown(PBX *pbx) {
    // TO BE IMPLEMENTED

    // Free all tus in tu_list
    for(int i = 0; i < PBX_MAX_EXTENSIONS; i++) {
        TU *tu = pbx->tu_list[i];
        if(tu) {
            // Disconnect tu fd from server
            shutdown(tu->fd, SHUT_RDWR);
            free(tu);
        }
    }
    // Free pbx and semaphore
    sem_destroy(&pbx->sem);
    free(pbx);
}
#endif

/*
 * Register a telephone unit with a PBX at a specified extension number.
 * This amounts to "plugging a telephone unit into the PBX".
 * The TU is initialized to the TU_ON_HOOK state.
 * The reference count of the TU is increased and the PBX retains this reference
 *for as long as the TU remains registered.
 * A notification of the assigned extension number is sent to the underlying network
 * client.
 *
 * @param pbx  The PBX registry.
 * @param tu  The TU to be registered.
 * @param ext  The extension number on which the TU is to be registered.
 * @return 0 if registration succeeds, otherwise -1.
 */
#if 1
int pbx_register(PBX *pbx, TU *tu, int ext) {
    // TO BE IMPLEMENTED
    P(&pbx->sem);
    // Find an open spot in tu list
    int open_tu_found = 0;
    for(int i = 0; i < PBX_MAX_EXTENSIONS; i++) {
        if(pbx->tu_list[i] == NULL) {
            tu->extension = ext;
            pbx->tu_list[i] = tu;
            open_tu_found++;
            break;
        }
    }

    // If max number of tu's already registered return -1
    if(!open_tu_found)
        return -1;

    V(&pbx->sem);

    return 0;
}
#endif

/*
 * Unregister a TU from a PBX.
 * This amounts to "unplugging a telephone unit from the PBX".
 * The TU is disassociated from its extension number.
 * Then a hangup operation is performed on the TU to cancel any
 * call that might be in progress.
 * Finally, the reference held by the PBX to the TU is released.
 *
 * @param pbx  The PBX.
 * @param tu  The TU to be unregistered.
 * @return 0 if unregistration succeeds, otherwise -1.
 */
#if 1
int pbx_unregister(PBX *pbx, TU *tu) {
    // TO BE IMPLEMENTED
    P(&pbx->sem);
    // Search for given tu in tu_list, free the given tu and set value to null in list
    for(int i = 0; i < PBX_MAX_EXTENSIONS; i++) {
        if(pbx->tu_list[i] == tu) {
            sem_destroy(&tu->sem);
            free(tu);
            pbx->tu_list[i] = NULL;
            V(&pbx->sem);
            return 0;
        }
    }
    V(&pbx->sem);

    return -1;
}
#endif

/*
 * Use the PBX to initiate a call from a specified TU to a specified extension.
 *
 * @param pbx  The PBX registry.
 * @param tu  The TU that is initiating the call.
 * @param ext  The extension number to be called.
 * @return 0 if dialing succeeds, otherwise -1.
 */
#if 1
int pbx_dial(PBX *pbx, TU *tu, int ext) {
    // TO BE IMPLEMENTED
    P(&pbx->sem);
    TU *tu_to_call = NULL;

    // Search for tu with matching ext number in tu_list
    for(int i = 0; i < PBX_MAX_EXTENSIONS; i++) {
        TU *curr = pbx->tu_list[i];
        if(curr) {
            if(curr->extension == ext)
                tu_to_call = curr;
        }
    }

    // dial fails
    if(tu_dial(tu, tu_to_call))
        return -1;

    V(&pbx->sem);

    return 0;
}
#endif
