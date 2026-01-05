//Connect To Server
#include "buffer.h"
#include "fds.h"
#include "mem.h"
#include "build/packet.h"
#include <malloc.h>
#include "packet.h"

int script_cts(int fd, Packet *packet) {
    Buffer *buf = NULL;
    int serv = create_connection("127.0.0.1", 25565);
    int *owner = malloc(sizeof(int));
    *owner = packet->from;
    mem_add(serv, owner);
    fds_set(serv, "owner", owner);

    int *server = malloc(sizeof(int));
    *server = serv;
    mem_add(fd, server);
    fds_set(fd, "server", server);

    buf = init_buffer();
    build_varint(buf, 0x00);
    build_varint(buf, 773);
    build_string(buf, "127.0.0.1");
    build_integer(buf, 25565, 2, 0);
    build_varint(buf, 2);
    packet_send(buf, serv);
    free_buffer(buf);

    packet_send(packet->buf, serv);
    return 0;
}