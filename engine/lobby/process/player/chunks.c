#include "kit.h"

int engine_lobby_process_player_chunks(int fd) {
    Player *player = fds_get(fd, "player");
    if (!player) return 1;
    if (player->chunks_sent) return 0;

    Buffer* buf = init_buffer();
    build_varint(buf, 0x77);
    
    // build_varint(buf, 0x10); //Tag_Compound
    build_varint(buf, 0x08); //Tag_String
    build_varint(buf, 0x00);
    build_string(buf, "Chunks Sent");
    build_varint(buf, false);

    packet_send(buf, fd);
    free_buffer(buf);

    player->chunks_sent = true;
    return 0;
}