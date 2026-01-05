#include "kit.h"

#define SEND(buf) packet_send(buf, fd); free_buffer(buf); buf = init_buffer()

int lobby_registries(int fd) {
    // I will pray to finish this
    Buffer *buf = init_buffer();
    build_varint(buf, 0x07);
    build_string(buf, "minecraft:damage_type");
    build_varint(buf, 25);
    build_string(buf,"minecraft:cactus");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:campfire");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:cramming");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:dragon_breath");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:drown");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:dry_out");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:ender_pearl");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:fall");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:fly_into_wall");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:freeze");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:generic");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:generic_kill");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:hot_floor");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:in_fire");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:in_wall");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:lava");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:lightning_bolt");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:magic");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:on_fire");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:out_of_world");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:outside_border");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:stalagmite");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:starve");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:sweet_berry_bush");
    build_varint(buf, 0); //No nbt data
    build_string(buf,"minecraft:wither");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:dimension_type");
    build_varint(buf, 1);
    build_string(buf, "minecraft:overworld");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:painting_variant");
    build_varint(buf, 1);
    build_string(buf, "wither");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:cat_variant");
    build_varint(buf, 1);
    build_string(buf, "white");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:chicken_variant");
    build_varint(buf, 1);
    build_string(buf, "warm");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:cow_variant");
    build_varint(buf, 1);
    build_string(buf, "warm");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:frog_variant");
    build_varint(buf, 1);
    build_string(buf, "warm");
    build_varint(buf, 0); //No nbt data
    SEND(buf);
    
    build_varint(buf, 0x07);
    build_string(buf, "minecraft:pig_variant");
    build_varint(buf, 1);
    build_string(buf, "warm");
    build_varint(buf, 0); //No nbt data
    SEND(buf);
    
    build_varint(buf, 0x07);
    build_string(buf, "minecraft:wolf_variant");
    build_varint(buf, 1);
    build_string(buf, "woods");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:wolf_sound_variant");
    build_varint(buf, 1);
    build_string(buf, "sad");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    build_varint(buf, 0x07);
    build_string(buf, "minecraft:worldgen/biome");
    build_varint(buf, 1);
    build_string(buf, "minecraft:plains");
    build_varint(buf, 0); //No nbt data
    SEND(buf);

    // LOG("Sent registries");

    free_buffer(buf);
    return 0;
}