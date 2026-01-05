#include "kit.h"

int engine_lobby_config(int fd, Packet *packet) {
    Buffer *b = packet->buf;
    int error = 0;
    Buffer *buf;

    static char *minecraft_core;
    if (!minecraft_core) {
        minecraft_core = get_config(c, "minecraft-core");
        if (!minecraft_core) minecraft_core = "1.21.10";
    }

    int pid = parse_varint(b, &error);
    if (error) return 1;
    // printf("Packet %d from fd=%d\n",pid,fd);
    // print_readable(b);

    switch (pid) {
    case 2:
        char *plugin = parse_string(b, 32767, &error);
        if (error) return 2;
        if (strcmp(plugin, "minecraft:brand")) {
            free(plugin);
            break;
        }
        free(plugin);

        char *brand = parse_string(b, 32767, &error);
        fds_set(fd, "brand", brand);
        mem_add(fd, brand);

        buf = init_buffer(); //Server Brand
        build_varint(buf, 0x01);
        build_string(buf, "minecraft:brand");
        build_string(buf, "mcords");
        packet_send(buf, fd);
        free_buffer(buf);

        buf = init_buffer(); //Clientbound known packs
        build_varint(buf, 0x0e);
        build_varint(buf, 1); // Only one pack (core)
        build_string(buf, "minecraft");
        build_string(buf, "core");
        build_string(buf, minecraft_core);
        packet_send(buf, fd);
        free_buffer(buf);
        break;

    case 7:
        int lobby_registries(int fd);
        lobby_registries(fd);

        buf = init_buffer(); //Finish Configuration
        build_varint(buf, 0x03);
        packet_send(buf, fd);
        free_buffer(buf);
        break;

    case 3:
        if (!fds_get(fd,"brand")) return 3;
        fds_set(fd, "state", (void *)3);
        int players_add(int fd);
        int id = players_add(fd);
        if (id<0) return 4;

        buf = init_buffer();
        build_varint(buf, 0x30);
        build_integer(buf, id, 4, 1); //Entity ID
        build_varint(buf, 0); //Is hardcore
        build_varint(buf, 1); //Dimension Names
        build_string(buf, "minecraft:overworld"); //Dimension Names
        build_varint(buf, 20); //Max Players
        build_varint(buf, 10); //View Distance (2-32)
        build_varint(buf, 10); //Simulation Distance (2-32)
        build_varint(buf, 0); //Reduced Debug Info
        build_varint(buf, 1); //Enable Respawn Screen
        build_varint(buf, 0); //Do Limited Crafting
        build_varint(buf, 0); //Dimension ID
        build_string(buf, "minecraft:overworld"); //Dimension Name
        build_integer(buf, 0, 8, 1);
        build_varint(buf, 1); //Game Mode: (0: Survival, 1: Creative, 2: Adventure, 3: Spectator)
        build_varint(buf, 3); //Previous Game Mode
        build_varint(buf, 0); //Is Debug
        build_varint(buf, 0); //Is Flat
        build_varint(buf, 0); //Has Death Location
        build_varint(buf, 0); //Portal Cooldown
        build_varint(buf, 16); //Sea Level
        build_varint(buf, 0); //Enforces Secure Chat
        packet_send(buf, fd);
        free_buffer(buf);

        buf = init_buffer();
        build_varint(buf, 0x2B);
        build_integer(buf, 1, 8, 1);
        packet_send(buf, fd);
        free_buffer(buf);
        
        buf = init_buffer();
        build_varint(buf, 0x26);
        build_varint(buf, 13);
        build_float(buf, 0);
        packet_send(buf, fd);
        free_buffer(buf);

        printf("Player %s joined the lobby\n", (char *)fds_get(fd, "username"));
        break;

    default:
        break;
    }

    return 0;
}