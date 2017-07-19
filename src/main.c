#include <stdio.h>
#include <stdlib.h>
#include <wayland-client.h>

void
global_registory_handler(void *data,
                         struct wl_registry *wl_registry,
                         uint32_t name,
                         const char *interface,
                         uint32_t version)
{
    printf("global_registory_handler: %s, name %d, version %d\n",
           interface, name, version);
}

void
global_registory_remover(void *data,
                         struct wl_registry *wl_registry,
                         uint32_t name)
{
    printf("global_registory_remover: name %d\n", name);
}

void
print_server_info(struct wl_display *display)
{
    const struct wl_registry_listener registry_listener = {
        global_registory_handler,
        global_registory_remover
    };
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    int n_events = wl_display_dispatch(display);
    printf("wl_display_dispatch: n_events %d\n", n_events);
    n_events = wl_display_roundtrip(display);
    printf("wl_display_roundtrip: n_events %d\n", n_events);
}

int
main(int argc, char *argv[])
{
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to the display\n");
        return 1;
    }
    printf("Connected to the display\n");

    print_server_info(display);

    wl_display_disconnect(display);
    printf("Disconnected from the display\n");
    
    return 0;
}
