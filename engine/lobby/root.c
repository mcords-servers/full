//Entry point for the managing players in lobby

#include "kit.h"
int engine_lobby_tick();
int engine_lobby_login(int fd, Packet *packet);
int engine_lobby_config(int fd, Packet *packet);
int engine_lobby_play(int fd, Packet *packet);

int engine_lobby(int fd, Packet *packet) {
    if (!packet) return engine_lobby_tick();

    //Runs only on a new packet
    uintptr_t state = (uintptr_t)fds_get(fd, "state");
    if (!state) {
        fds_set(fd, "state", (void *)1);
        state = 1;
    }
    int r = 0;

    if (state == 1) {
        r = engine_lobby_login(fd, packet);
    } else if (state == 2) {
        r = engine_lobby_config(fd, packet);
    } else if (state == 3) {
        //I'm doing like this because i might at some point want to stop packet handling by lobby
        r = engine_lobby_play(fd, packet);
    }

    return r;
}