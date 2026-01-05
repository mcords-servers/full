#include "kit.h"

int engine_lobby_login(int fd, Packet *packet) {
    int error = 0;

    int pid = parse_varint(packet->buf, &error);
    if (error) return 1;

    Buffer *buf;

    switch (pid) {
    case 0:
        char *username = parse_string(packet->buf, 16, &error);
        if (error) return 2;
        mem_add(fd, username);
        fds_set(fd, "username", username);

        buf = init_buffer();
        build_varint(buf, 2);

        //UUID
        build_integer(buf, 0, 8, 0);
        build_integer(buf, 0, 8, 0);

        build_string(buf, username);

        //Profile
        build_varint(buf, 0);

        packet_send(buf, fd);
        free_buffer(buf);

        break;

    case 3:
        if (!fds_get(fd, "username")) return 3;
        fds_set(fd, "state", (void *)2);

        // buf = init_buffer();
        // build_varint(buf, 2);

        // build_varint(buf, 8);
        // build_integer(buf, 4, 2, 0);
        // append_to_buffer(buf, "text", 4);

        // packet_send(buf, fd);
        // free_buffer(buf);
        break;

    default:
        break;
    }

    return 0;
}