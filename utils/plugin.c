#include "plugin.h"
#include "kit.h"

extern void (*__start_plugins)(void *ptr);
extern void (*__stop_plugins)(void *ptr);

// static struct Plugin {
//     char *name;

//     struct event {
//         char *name;
//         void (*function)(void *ptr);
//     };
//     struct event **events;
// };

// struct Plugin **plugins;

void run_plugins(int shift, void *arg) {
    int plugin_fields = 3;

    void (**begin)(void *ptr) = &__start_plugins;
    void (**end)(void *ptr)   = &__stop_plugins;

    if (shift >= (end - begin))
        return;

    for (void (**p)(void *ptr) = begin + shift; p < end; p+=plugin_fields) {
        (*p)(arg);
    }
}

// int add_plugin(char *name, void *plugin) {
//     int index = 0;
//     if (!plugins) {
//         plugins = malloc(sizeof(struct Plugin)*6);
//     }

//     return index;
// }

// int players_add(int fd) {
//     int base_size = 5;
//     if (!plugins) {
//         plugins = calloc(base_size+1, sizeof(&plugins));
//         if (!plugins) return -1;
//         plugins[base_size] = NULL;
//     }
//     int i = 0;
//     while (plugins[i]&&plugins[i]!=NULL) i += 1;
//     if (plugins[i]) {
//         int *temp = realloc(plugins, ((i*2)+1)*sizeof(&plugins));
//         if (!temp) return -2;
//         for (int j = i+2; j < i*2; j++) temp[j] = 0;
//         temp[i*2] = NULL;
//         temp[i] = fd;
//         plugins = temp;
//     } else {
//         plugins[i] = fd;
//     }
//     return 0;
// }