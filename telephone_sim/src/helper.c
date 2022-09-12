#include "helper.h"

void print_tu_state(TU *tu) {
	TU_STATE state = tu->state;
	if(state == TU_ON_HOOK)
		dprintf(tu->fd, "ON HOOK %d\n", tu->extension);
	else if(state == TU_RINGING)
		dprintf(tu->fd, "RINGING\n");
	else if(state == TU_DIAL_TONE)
		dprintf(tu->fd, "DIAL TONE\n");
	else if(state == TU_RING_BACK)
		dprintf(tu->fd, "RING BACK\n");
	else if(state == TU_BUSY_SIGNAL)
		dprintf(tu->fd, "BUSY SIGNAL\n");
	else if(state == TU_CONNECTED)
		dprintf(tu->fd, "CONNECTED %d\n", tu->peer_tu->extension);
	else if(state == TU_ERROR)
		dprintf(tu->fd, "ERROR\n");
}

int is_integer(char *str) {
	for(int i = 0; i < strlen(str); i++) {
		char c = *(str + i);
		if(c > '9' || c < '0')
			return 0;
	}
	return 1;
}

int same_string(char *str1, char *str2) {
	// str2 will always have null term
	int str2_len = strlen(str2);
	if(*(str1 + str2_len) != '\0' && *(str1 + str2_len) != ' ') {
		return 0;
	}

	for(int i = 0; i < strlen(str2); i++) {
		if(*str1 != *str2)
			return 0;
		str1++;
		str2++;
	}
	return 1;
}