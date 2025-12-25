#include "engine/packets.h"
#include "build/packet.h"
#include "parse/packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "h/fds.h"
#include "h/mem.h"
#include "h/packet.h"

#define MOTD "{\
    'version': {\
        'name': '1.21.8',\
        'protocol': 772\
    },\
    'players': {\
        'max': 20,\
        'online': 1,\
        'sample': [\
            {\
                'name': 'thinkofdeath',\
                'id': '4566e69f-c907-48ee-8d71-d7ba5aa00d20'\
			}\
        ]\
    },\
    'description': {\
        'text': 'Hello, world!'\
    },\
    'favicon': 'data:image/png;base64,<data>',\
    'enforcesSecureChat': false\
 }"

int process_packet(Packet* packet) {
    printf("From fd=%d\n",packet->from);
    print_readable(packet->buf);
    
    int fd = packet->from;

    char *_ = fds_get(fd, "intent");
    int intent = -1;
    if (_) intent = *_;
    Buffer* buf = NULL;

    int error = 0;

    char *ptr;

    switch (intent) {
    case 1:
        buf = init_buffer();
        if (!parse_varint(packet->buf, &error)) {
            build_varint(buf, 0x00);
            build_string(buf, MOTD);
        } else {
            build_varint(buf, 0x01);
            build_integer(buf, parse_integer(packet->buf, 8, 1, &error), 8, 1);
        }
        if (error) {
            free_buffer(buf);
            buf = NULL;
            close_connection(packet->from);
        }
        break;

    case 2:
        int *sfd = fds_get(fd, "server");
        if (sfd) {
            buf = init_buffer();
            append_to_buffer(buf, packet->buf->buffer, packet->buf->length);
            fd = *sfd;
            break;
        }
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
        buf = NULL;
        // LOG("Established connection with fd=%d", create_connection("127.0.0.1", 25565));
        break;

    case 3:    
        buf = init_buffer();
        build_varint(buf, 0x00);
        build_string(buf, "{'text':'Transfers are not supported!!!','color':'red'}");
        break;
    
    default:
        int *o = fds_get(fd, "owner");
        if (o) {
            buf = init_buffer();
            append_to_buffer(buf, packet->buf->buffer, packet->buf->length);
            fd = *o;
            break;
        }
        if (parse_varint(packet->buf, &error) || error) return 1;
        parse_varint(packet->buf, &error);
        free(parse_string(packet->buf, 255, &error));
        parse_integer(packet->buf, 2, 0, &error);
        intent = parse_varint(packet->buf, &error);
        if (error || intent<1 || intent>3 ) return 2;

        ptr = malloc(sizeof(char));
        *ptr = intent;

        mem_add(fd, ptr);
        fds_set(fd, "intent", ptr);
        break;
    }

    if (buf) {
        packet_send(buf, fd);
        free_buffer(buf);
    }

    return 0;
}
