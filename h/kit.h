#ifndef KIT_H
#define KIT_H

#include "fds.h"
#include "types.h"
#include "mem.h"
#include "build/packet.h"
#include "build/nbt.h"
#include "parse/packet.h"
#include "requests.h"
#include "clock.h"
#include "buffer.h"
#include "globals.h"
#include "packet.h"
#include "common.h"

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define false 0
#define true 1
#define max(x,y) (x>y?x:y)
#define min(x,y) (x<y?x:y)

typedef struct config config;
char* get_config(config* config, char* key);
extern config* c;
#endif