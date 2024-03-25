#include <X11/X.h>
#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LEN(X)         (sizeof X / sizeof X[0])
#define MAX_STATUS_LEN 256
#define TEXT_LEN       24

/* config file */
#include "config.h"

static volatile sig_atomic_t keep_running = 1;

static void
sig_handler(int _)
{
        puts("stopped");
        (void) _;
        keep_running = 0;
}

/* function implementations */
void
setroot(Display *dpy, Window root)
{
        char status[MAX_STATUS_LEN];
        char text[TEXT_LEN];

        Module mfirst = modules[0];
        mfirst.func(&mfirst.args, status, LEN(status));

        unsigned int i;
        for (i = 1; i < LEN(modules); i++) {
                Module m = modules[i];
                m.func(&m.args, text, LEN(text));

                strncat(
                    status, &delimitter,
                    (MAX_STATUS_LEN - strlen(status) - strlen(&delimitter)));
                strncat(status, text,
                        (MAX_STATUS_LEN - strlen(status) - strlen(text)));
                text[0] = '\0';
        }
        XStoreName(dpy, root, status);
        XFlush(dpy);
}

int
main(void)
{
        signal(SIGINT, sig_handler);

        Display *dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
                fprintf(stderr, "could not open display");
                exit(-1);
        }

        int screen = DefaultScreen(dpy);
        Window root = RootWindow(dpy, screen);

        while (keep_running) {
                setroot(dpy, root);
                sleep(refresh_interval_s);
        }
        XCloseDisplay(dpy);

        return EXIT_SUCCESS;
}
