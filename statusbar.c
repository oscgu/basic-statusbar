#include <X11/X.h>
#include <X11/Xlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LEN(X) (sizeof X / sizeof X[0])

/* In theory max 8 modules allowed */
#define MAX_STATUS_LEN      256
#define MAX_MODULE_TEXT_LEN 32

/* config file */
#include "config.h"

static volatile sig_atomic_t keep_running = 1;
static Display *dpy;

static void
sig_handler(int sig)
{
        puts("stopping statusbar\n");
        (void) sig;
        keep_running = 0;
        XCloseDisplay(dpy);
}

/* function implementations */
static void
setroot(Display *dpy, Window root)
{
        char status[MAX_STATUS_LEN] = {0};
        char text[MAX_MODULE_TEXT_LEN];

        unsigned int i;
        for (i = 0; i < LEN(modules); i++) {
                Module m = modules[i];
                m.func(&m.args, text, LEN(text) -1);

                if (i > 0) {
                        strncat(status, delimiter,
                                MAX_STATUS_LEN - strlen(status) - 1);
                }

                strncat(status, text, MAX_STATUS_LEN - strlen(status) - 1);

                /* clear text buffer */
                text[0] = '\0';
        }
        XStoreName(dpy, root, status);
        XFlush(dpy);
}

int
main(void)
{
        signal(SIGINT | SIGKILL, sig_handler);

        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
                fprintf(stderr, "could not open display\n");
                exit(-1);
        }

        int screen = DefaultScreen(dpy);
        Window root = RootWindow(dpy, screen);

        while (keep_running) {
                setroot(dpy, root);
                sleep(refresh_interval_s);
        }

        return -1;
}
