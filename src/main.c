#include <stdio.h>
#include <stdlib.h>
#include <wayland-client.h>

int
main(int argc, char *argv[])
{
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to the display\n");
        return 1;
    }
    printf("Connected to the display\n");

    wl_display_disconnect(display);
    printf("Disconnected from the display\n");
    
    return 0;
}
