#include "h/globals.h"
#include "h/mem.h"
#include "h/fds.h"
#include "h/packet.h"
#include "h/requests.h"
#include "h/clock.h"
#include "engine/tick.h"
#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <malloc.h>

#define DEFAULT_EPOLL_FLAGS (EPOLLIN | EPOLLET | EPOLLRDHUP)

int epoll_fd;
int exitbool = 0;
int max_fds = 0;

// printf("\n");

Data** fds;
Packet** packet_queue;
int packets;
typedef struct config config;
config* c;

config* load(char* file, int len, char* fallback);
char* get_config(config* config, char* key);
int free_config(config* config);

void close_connection(int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);

    mem_free(fd);
    Data* block = fds[fd];
    while (block) {
        Data* next = block->next;
        free(block->key);
        free(block);
        block = next;
    }
    fds[fd] = NULL;

    packet_queue_free(fd);
    printf("Closed connection (fd=%d)\n", fd);

    //TODO: Remove this from final version (maybe)
    if (get_config(c, "debug")) {
        exitbool = 1;
    }
}

void accept_connection(int server_fd, int epoll_fd) {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break; // all pending connections accepted
            perror("accept");
            break;
        }

        fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

        struct epoll_event ev = { .events = DEFAULT_EPOLL_FLAGS };
        ev.data.fd = client_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
            perror("epoll_ctl (client)");

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        printf("New client connected: %s:%d (fd=%d)\n", ip, ntohs(client_addr.sin_port), client_fd);
        queue[client_fd] = NULL;
        fds[client_fd] = NULL;
    }
}

// [ ]:

int create_connection(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        close(sock);
        return -1;
    }

    int r = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        if (r == 0) {
        printf("Connected immediately (fd=%d)\n", sock);

        struct epoll_event ev = {
            .events = DEFAULT_EPOLL_FLAGS,
            .data.fd = sock
        };
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);

        return sock;
    }

    if (r < 0 && errno != EINPROGRESS) {
        close(sock);
        return -1;
    }

    struct epoll_event ev = {
        .events = DEFAULT_EPOLL_FLAGS,
        .data.fd = sock
    };

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);


    queue[sock] = NULL;
    fds[sock] = NULL;

    printf("Created connection (fd=%d)\n",sock);
    fds_set(sock, "", NULL);

    return sock;

}

int handle_packet(int fd, int epoll_fd) {

    Buffer* stream = init_buffer();
    char buf[4096];

    if (!packet_queue) {
        packet_queue = malloc(sizeof(Packet*));
        packet_queue[0] = NULL;
        packets = 0;
    }

    /* ========== 1. FULLY DRAIN SOCKET INTO STREAM ========== */
    while (1) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);

        if (n > 0) {
            append_to_buffer(stream, buf, n);
            continue;
        }

        if (n == 0) { 
            close_connection(fd);
            return 1;
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK)
            break;

        perror("recv");
        close_connection(fd);
        return 2;
    }

    /* If no data read, nothing to do */
    if (stream->length == 0)
        return 0;

    /* ========== 2. BEGIN PACKET EXTRACTION LOOP ========== */
    while (1) {

        /* Find unfinished packet belonging to this fd */
        Packet *packet = NULL;
        for (int i = 0; i < packets; i++) {
            if (packet_queue[i] &&
                packet_queue[i]->from == fd &&
                packet_queue[i]->state != FINISHED) {

                packet = packet_queue[i];
                break;
            }
        }

        /* No existing packet → allocate new state */
        if (!packet) {
            packet = malloc(sizeof(Packet));
            packet->buf  = init_buffer();
            packet->from = fd;
            packet->state = LEN_UNKNOWN;
            packet->len = 0;

            packet_queue = realloc(packet_queue, sizeof(Packet*) * (packets + 1));
            if (!packet_queue) { perror("realloc"); exit(1); }

            packet_queue[packets++] = packet;
        }

        /* ========== 3. READ PACKET LENGTH IF UNKNOWN ========== */
        if (packet->state == LEN_UNKNOWN) {
            uint32_t packet_len = 0;
            uint32_t shift = 0;
            int varint_bytes = 0;

            /* We need at least 1 byte */
            if (stream->length == 0)
                break;

            while (varint_bytes < stream->length) {
                uint8_t b = stream->buffer[varint_bytes];
                packet_len |= (b & 0x7F) << shift;

                if (!(b & 0x80)) {
                    /* full varint decoded */
                    cut_buffer(stream, -(varint_bytes + 1));
                    packet->len = packet_len;
                    packet->state = UNFINISHED;
                    goto LENGTH_DONE;
                }

                shift += 7;
                varint_bytes++;

                if (shift >= 32)
                    return 3;
            }

            /* incomplete varint */
            break;

LENGTH_DONE:;
        }

        /* ========== 4. MOVE DATA FROM STREAM INTO PACKET ========== */
        size_t need = packet->len - packet->buf->length;
        size_t available = stream->length;

        if (available == 0)
            break;

        size_t take = (need < available) ? need : available;

        append_to_buffer(packet->buf, stream->buffer, take);
        cut_buffer(stream, -((int)take));

        /* ========== 5. PACKET COMPLETED ========== */
        if (packet->buf->length == packet->len) {
            packet->state = FINISHED;

            /* At this point your packet is complete */
            /* call your handler here */

            /* After processing it, free slot for next packet */
            // Example: hold finished packets in queue or dispatch immediately
        }

        /* If finished → loop again and try next packet inside stream */
    }

    free_buffer(stream);
    return 0;
}

int main() {
    packets = 0;
    c = load("server.properties", 0,
        "# Default Minecraft server properties\n"
        "motd=My Minecraft Server\n"
        "server-port=25568\n"
        "max-players=20\n"
        "online-mode=true\n"
        "view-distance=10\n"
        "max-fds=65536\n"
        "max-events=1024\n");

    int port;
    char* ports = get_config(c, "server-port");
    sscanf(ports ? ports : "25568", "%i", &port);

    char* temp_fds = get_config(c, "max-fds");
    sscanf(temp_fds ? temp_fds : "65536", "%i", &max_fds);

    queue = malloc(sizeof(PacketQueue*)*max_fds);
    fds = calloc(max_fds, sizeof(Data*));

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        return 1;
    }

    fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        close(server_fd);
        return 1;
    }

    http_init(); // keep http system

    int max_events;
    char* events_str = get_config(c, "max-events");
    sscanf(events_str ? events_str : "1024", "%d", &max_events);

    struct epoll_event ev = { .events = DEFAULT_EPOLL_FLAGS };
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl (server_fd)");
        close(server_fd);
        close(epoll_fd);
        return 1;
    }

    struct epoll_event* events = malloc(sizeof(struct epoll_event) * max_events);
    if (!events) {
        perror("malloc (events)");
        close(server_fd);
        close(epoll_fd);
        return 1;
    }

    printf("Node listening on port %d (EPOLLET, non-blocking, timeout=0)\n", port);
    int ticks = 0;
    while (!exitbool) {
        int nfds = epoll_wait(epoll_fd, events, max_events, 0); // non-blocking!
        if (nfds < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            uint32_t ev_flags = events[i].events;

            if (fd == server_fd) {
                accept_connection(server_fd, epoll_fd);
            } else if (ev_flags & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                close_connection(fd);
            } else if (ev_flags & EPOLLIN) {
                handle_packet(fd, epoll_fd);
            } else if (ev_flags & EPOLLOUT) {
                packet_queue_send(fd);
            }
        }

        if(delay_repeat(0.05, &ticks)) tick();
    }

    for (int i = 0; i < max_fds; i++) {
        if (fds[i]) {
            close_connection(i);
        }
    }

    fflush(stdout);

    free(events);
    free_config(c);
    http_cleanup();
    close(server_fd);
    close(epoll_fd);
    return 0;
}
