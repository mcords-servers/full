#include "engine/packets.h"
#include "engine/tick.h"
#include "kit.h"

// int ticks = 0;

void tick() {
    run_plugins(1, NULL);
    // if (++ticks%200==0) exitbool = 1;
    Packet** packet_ql = malloc(sizeof(Packet*));
    int packet_l = 0;
    int r;
    for (int i = 0; i < packets; i++) {
        if (!packet_queue[i]) continue;
        if (packet_queue[i]->state) {
            packet_ql = realloc(packet_ql, sizeof(Packet*) * (packet_l + 1));
            packet_ql[packet_l++] = packet_queue[i];
        } else {
            // run_plugins(2, packet_queue[i]);
            run_plugins(2, packet_queue[i]);
            r = process_packet(packet_queue[i]);
            if (r) {
                LOG("Disconnected (fd=%d) due to error code %d from process_packet", packet_queue[i]->from, r);
                close_connection(packet_queue[i]->from);
            }
            free_buffer(packet_queue[i]->buf);
            free(packet_queue[i]);
        }
    }

    free(packet_queue);
    packet_queue = packet_l?packet_ql:NULL;
    packets = packet_l?packet_l:0;
    if (!packet_l) free(packet_ql);

    int engine_lobby(int fd, Packet *packet);
    engine_lobby(-1, NULL);

}