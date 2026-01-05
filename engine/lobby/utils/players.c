#include "kit.h"

extern int *lobby_players;

int players_add(int fd) {
    if (!lobby_players) {
        lobby_players = calloc(21, sizeof(int));
        if (!lobby_players) return -1;
        lobby_players[20] = 1;
    }
    int i = 0;
    while (lobby_players[i]&&lobby_players[i]!=1) i += 1;
    if (lobby_players[i]) {
        int *temp = realloc(lobby_players, ((i*2)+1)*sizeof(int));
        if (!temp) return -2;
        for (int j = i+2; j < i*2; j++) temp[j] = 0;
        temp[i*2] = 1;
        temp[i] = fd;
        lobby_players = temp;
    } else {
        lobby_players[i] = fd;
    }

    Player *player = malloc(sizeof(Player));
    player->chunk_center.is_set = false;
    player->chunks_sent = false;
    player->position.x = 0.0;
    player->position.y = 0.0;
    player->position.z = 0.0;
    mem_add(fd, player);
    fds_set(fd, "player", player);
    return i;
}

int players_remove(int fd) {
    if (!lobby_players) return -1;
    for (int i = 0; lobby_players[i]!=1; i++) {
        if (lobby_players[i]==fd) {
            lobby_players[i] = 0;
            return i;
        }
    }
    return -1;
}

int players_get(int fd) {
    if (!lobby_players) return -1;
    for (int i = 0; lobby_players[i]!=1; i++) {
        if (lobby_players[i]==fd) {
            return i;
        }
    }
    return -1;
}

// #include <stdlib.h>
// int main() {
//     int fd = (rand()%1000)+2;
//     for (int i = 0; i < 300; i++) {
//         players_add(fd);
//         player_remove(fd);
//         fd = (rand()%1000)+2;
//     }
//     for (int i = 0; lobby_players[i]!=1; i++) {
//         printf("%d, ",lobby_players[i]);
//     }
//     putc('\n', stdout);
// }