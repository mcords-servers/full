#include "kit.h"

int engine_lobby_play(int fd, Packet *packet) {
    int error = 0;

    int pid = parse_varint(packet->buf, &error);
    if (error) return 1;

    Buffer *buf;
    Player *player;
    double pos[2];
    switch (pid) {
    case 0x1B:
        buf = init_buffer();
        build_varint(buf, 0x2B);
        build_integer(buf, 1, 8, 1);
        packet_send(buf, fd);
        free_buffer(buf);

        break;

    case 0x08:
        exitbool = 1;
        break;

    case 0x1D:
        player = fds_get(fd, "player");
        if (!player) return 2;
        pos[0] = player->position.x;
        pos[1] = player->position.z;
        player->position.x = parse_double(packet->buf, &error);
        player->position.y = parse_double(packet->buf, &error);
        player->position.z = parse_double(packet->buf, &error);
        if (error) return 3;

        #define chunk_moved(a,b) (int)floor(a/16.0)!=((int)floor(b/16.0))

        if (!player->chunks_sent) break;
        else if (chunk_moved(player->position.x, pos[0])) player->chunks_sent=false;
        else if (chunk_moved(player->position.z, pos[1])) player->chunks_sent=false; 

        break;

    case 0x1E:
        player = fds_get(fd, "player");
        if (!player) return 4;
        pos[0] = player->position.x;
        pos[1] = player->position.z;
        player->position.x = parse_double(packet->buf, &error);
        player->position.y = parse_double(packet->buf, &error);
        player->position.z = parse_double(packet->buf, &error);
        player->rotation.yaw = parse_float(packet->buf, &error);
        player->rotation.pitch = parse_float(packet->buf, &error);
        if (error) return 5;

        if (!player->chunks_sent) break;
        else if (chunk_moved(player->position.x, pos[0])) player->chunks_sent=false;
        else if (chunk_moved(player->position.z, pos[1])) player->chunks_sent=false;
        #undef chunk_moved

        break;

    case 0x1F:
        player = fds_get(fd, "player");
        if (!player) return 6;
        player->rotation.yaw = parse_float(packet->buf, &error);
        player->rotation.pitch = parse_float(packet->buf, &error);

        if (error) return 7;
        break;
    
    }

    return 0;
}