#include "kit.h"

int *lobby_players;

int engine_lobby_tick() {
    if (!lobby_players) goto w1;
    for (int i = 0; lobby_players[i]!=1; i++) {
        if (!lobby_players[i]) continue; //Empty
        
        int engine_lobby_process_player(int fd);
        engine_lobby_process_player(lobby_players[i]);

        // printf("[!] Ticking Player Found: %s\n",(char *)fds_get(lobby_players[i], "username"));
    }
w1:
    return 0;
}