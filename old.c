int handle_packet(int fd, int epoll_fd) {

    //packet_queue[i]->state
    //0x00 - finished
    //0x01 - unfinished
    //0x02 - len unknown

    if (!packet_queue) {
        packet_queue = malloc(sizeof(Packet*));
        packet_queue[0] = NULL;
        packets = 0;
    }

    Packet* packet;
    while (1) {
        packet = NULL;
        for (int i = 0; i < packets; i++) {
            if (packet_queue[i] && packet_queue[i]->state && packet_queue[i]->from == fd) {
                packet = packet_queue[i];
                break;
            }
        }

        if (!packet) {
            packet = malloc(sizeof(Packet));
            packet->buf = init_buffer();
            packet->from = fd;
            packet->state = LEN_UNKNOWN;

            packet_queue = realloc(packet_queue, sizeof(Packet*) * (packets + 1));
            if (!packet_queue) { perror("realloc"); exit(1); }
            packet_queue[packets++] = packet;
        }

        char buf[4096];
        // int amout = (packet->state==UNFINISHED && packet->len > sizeof(buf))? sizeof(buf) : (packet->state==LEN_UNKNOWN)? 1 : packet->len - packet->buf->length;
        int amout;
        if (packet->state==UNFINISHED) {
            if (packet->len > sizeof(buf)) amout = sizeof(buf);
            else amout = packet->len - packet->buf->length;
        } else if (packet->state==LEN_UNKNOWN) amout = 1;

        ssize_t n = recv(fd, buf, amout, 0);
        if (n == 0) {
            close_connection(fd);
            for (int i = 0; i < packets; i++) {
                if (packet_queue && packet_queue[i]->from==fd) {
                    free_buffer(packet_queue[i]->buf);
                    free(packet_queue[i]);
                    packet_queue[i] = NULL;
                }
            }
            return 1;
        } else if (n < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recv");
                close_connection(fd);
                return 2;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return 0;
            }
        }

        append_to_buffer(packet->buf, buf, n);
        // Example: handle your packet data here
        // printf("[fd=%d] Received %zd bytes\n", fd, n);

        if (packet->state == LEN_UNKNOWN) {
            uint32_t packet_len = 0;
            char err = 0;
            int i = 0;
            int shift = 0;
            // while (1) {
            for (; i < packet->buf->length; i++) {
                uint8_t b = packet->buf->buffer[i];
                packet_len |= (b & 0x7F) << shift;
                if (!(b & 0x80)) break;
                shift += 7;
                if (i==packet->buf->length) {err = 1;break;}
                if (shift >= 32) {err = 1; break;}
            } if (!err) {
                if (packet_len > 0) {
                    packet->len = packet_len;
                    packet->state = UNFINISHED;
                    cut_buffer(packet->buf, -(i+1));
                }
            } else {
                return 3;
            }
        }
        if (packet->buf->length == packet->len && packet->state == UNFINISHED) {
            packet->state = FINISHED;
            // printf("[fd=%i] Got a minecraft packet:\n",fd);
            // print_hex(packet->buf);
        }
        // if (packet->state == 0x00) break;
    }
    return 0;
}