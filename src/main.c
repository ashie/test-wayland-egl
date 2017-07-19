#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

struct wl_compositor *compositor = NULL;
struct wl_shell *shell = NULL;

void
global_registry_handler(void *data,
                        struct wl_registry *wl_registry,
                        uint32_t name,
                        const char *interface,
                        uint32_t version)
{
    printf("global_registry_handler: %s, name %d, version %d\n",
           interface, name, version);
    if (!strcmp(interface, "wl_compositor")) {
        compositor = wl_registry_bind(wl_registry, 
                                      name, 
                                      &wl_compositor_interface, 
                                      1);
    } else if (!strcmp(interface, "wl_shell")) {
        shell = wl_registry_bind(wl_registry, 
                                 name, 
                                 &wl_shell_interface, 
                                 1);
    }
}

void
global_registry_remover(void *data,
                        struct wl_registry *wl_registry,
                        uint32_t name)
{
    printf("global_registry_remover: name %d\n", name);
}

static void
add_registry_listener(struct wl_display *display)
{
    const struct wl_registry_listener registry_listener = {
        global_registry_handler,
        global_registry_remover
    };
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    int n_events = wl_display_dispatch(display);
    printf("wl_display_dispatch: n_events %d\n", n_events);
    n_events = wl_display_roundtrip(display);
    printf("wl_display_roundtrip: n_events %d\n", n_events);
}

static struct wl_surface *
create_surface(struct wl_compositor *compositor)
{
    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    if (surface)
        printf("A surface is created: %p\n", surface);
    else
        printf("Failed to create surface!\n");
    return surface;
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

    add_registry_listener(display);

    struct wl_surface *surface = create_surface(compositor);

    wl_surface_destroy(surface);
    wl_compositor_destroy(compositor);
    wl_shell_destroy(shell);

    wl_display_disconnect(display);
    printf("Disconnected from the display\n");

    return 0;
}
