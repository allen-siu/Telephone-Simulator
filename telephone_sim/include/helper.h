#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semaphore.h"

#include "pbx.h"

struct tu {
    int extension;
    int fd;
    int references;
    TU_STATE state;
    TU *peer_tu;
    sem_t sem;
};

struct pbx {
    TU *tu_list[PBX_MAX_EXTENSIONS];
    sem_t sem;
};

void print_tu_state(TU *tu);

int is_integer(char *str);
int same_string(char *str1, char *str2);

#endif