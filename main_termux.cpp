#include <termuxgui/termuxgui.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "render.h"

int main() {
    // create connetion
    tgui_connection c;
    if (tgui_connection_create(&c) != 0) {
        puts("error connection create\n");
        exit(1);
    }

    // create activity
    tgui_activity a;
    if (tgui_activity_create(c, &a, TGUI_ACTIVITY_NORMAL, NULL, false) != 0) {
        puts("error activity create\n");
        exit(1);
    }

    tgui_view iv = -1;
    // Create a buffer description
    tgui_buffer b = {
        .width = 320,
        .height = 240,
        .format = TGUI_BUFFER_FORMAT_ARGB8888
    };

    // Create the buffer
    if (tgui_add_buffer(c, &b) != 0) {
        puts("error addbuffer\n");
        exit(1);
    }

    // set a dark gray background
    memset(b.data, 32, b.width*b.height*4);

    // animation progress
    int prog = 0;

    // pause when the Activity isn't visible
    bool paused = true;
    time_t start = time(NULL);
    while (true) {
        tgui_event e;
        bool available;
        if (tgui_poll_event(c, &e, &available) != 0) {
            puts("error poll\n");
            exit(1);
        }

        if (available) {
            if (e.type == TGUI_EVENT_CREATE) {
                // Activity created, we can create the ImageView and set the buffer now
                if (iv == -1) {
                    if (tgui_create_image_view(c, a, &iv, NULL, TGUI_VIS_VISIBLE, false) != 0) {
                        puts("error create imageview\n");
                        exit(1);
                    }
                    if (tgui_set_buffer(c, a, iv, &b) != 0) {
                        puts("error setbuffer\n");
                        exit(1);
                    }
                }
                paused = false;
            }
            if (e.type == TGUI_EVENT_PAUSE) {
                paused = true;
            }

            if (e.type == TGUI_EVENT_DESTROY) {
                exit(0);
            }

            tgui_event_destroy(&e);
        }

        // Exit after 10 seconds
        if (time(NULL) - start >= 10) break;

        // if the ImageVIew is created and we aren't paused, render the animation
        if (iv != -1 && ! paused) {
            render(b.width, b.height, prog, static_cast<unsigned char*>(b.data));
            //memset(b.data+120*(320*4)+(160+prog)*4+2, 255, 1);
		    prog++;

            // show the buffer in the ImageView
            if (tgui_blit_buffer(c, &b) != 0 || tgui_refresh_image_view(c, a, iv) != 0) {
                puts("error buffer blit\n");
                exit(1);
            }
        }
        usleep(10000);
    }

    tgui_connection_destroy(c);

    return 0;
}



