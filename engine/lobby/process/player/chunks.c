#include "kit.h"

int engine_lobby_process_player_chunks(int fd) {
    Player *player = fds_get(fd, "player");
    if (!player) return 1;
    if (player->chunks_sent) return 0;

    Buffer* buf = init_buffer();
    build_varint(buf, 0x77);

    // build_varint(buf, 0x10); //Tag_Compound
    char message[32];
    snprintf(message, sizeof(message), "Entered chunk: %d, %d", (int)floor(player->position.x/16.0), (int)floor(player->position.z/16.0));
    TAG_Compound(buf,None);
    TAG_String(buf,"color","red");
    TAG_String(buf,"text","some text");
    TAG_List(buf,"extra",10,1);
    // remove: TAG_Compound(buf,"");
    TAG_String(buf,"color","red");
    TAG_String(buf,"text"," some other text");
    // remove: TAG_End(buf);
    TAG_End(buf);
    TAG_End(buf);
    build_varint(buf,false);

    // // printf("[0] w....color..red...extra........color..red...text.. some other text....text..some text..\n[1] ");
    // printf("[0] 77 0A 08 00 05 63 6F 6C 6F 72 00 03 72 65 64 09 00 05 65 78 74 72 61 0A 00 00 00 01 08 00 05 63 6F 6C 6F 72 00 03 72 65 64 08 00 04 74 65 78 74 00 10 20 73 6F 6D 65 20 6F 74 68 65 72 20 74 65 78 74 00 08 00 04 74 65 78 74 00 09 73 6F 6D 65 20 74 65 78 74 00 00\n[1] ");
    // print_hex(buf);
    // printf("[2] ");
    // print_readable(buf);
    packet_send(buf, fd);
    free_buffer(buf);

    player->chunks_sent = true;
    return 0;
}