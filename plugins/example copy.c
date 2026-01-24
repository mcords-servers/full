#include "kit.h"

static void init(void *ptr) {
    printf(" %s",File_Name);
}

static void on_Tick(void *ptr) {
    printf("Plugin ticking\n");
}

static void on_Packet(Packet *ptr) {
    printf("Plugin packet_recieved %d\n",ptr->from);
    // int error = 0;
    // parse_varint(ptr->buf, );
}

static __attribute__((section("plugins"), used))
void (*plugin_init)(void *ptr) = init;
static __attribute__((section("plugins"), used))
void (*plugin_on_Tick)(void *ptr) = on_Tick;
static __attribute__((section("plugins"), used))
void (*plugin_on_Packet)(Packet *ptr) = on_Packet;