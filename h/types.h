#ifndef TYPES_H
#define TYPES_H

typedef struct Player Player;
struct Player {
    struct {
       double x;
       double y;
       double z;
    } position;
    struct {
        float yaw;
        float pitch;
    } rotation;
    struct {
        int x;
        int z;
        char is_set;
    } chunk_center;

    char chunks_sent;
};

#endif