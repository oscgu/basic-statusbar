#include "modules.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LEN(X)         (sizeof X / sizeof X[0])
#define MAX_STATUS_LEN 256
#define TEXT_LEN       24

//#define DEBUG

/* structs */
typedef struct {
        void (*func)(Args *, int, char *, int);
        Args args;
        int flag;
} Module;

/* config file */
#include "config.h"

/* function implementations */
void
setroot(Display *dpy, Window root)
{
        char status[MAX_STATUS_LEN];
        char text[TEXT_LEN];

        Module mfirst = modules[0];
        mfirst.func(&mfirst.args, mfirst.flag, status, LEN(status));

        unsigned int i;
        for (i = 1; i < LEN(modules); i++) {
                Module m = modules[i];
                m.func(&m.args, m.flag, text, LEN(text));

                strcat(status, &delimitter);
                strcat(status, text);
                text[0] = '\0';
        }

        XStoreName(dpy, root, status);
}

int
main()
{
        for (;;) {
                Display *dpy;

                Display *d = XOpenDisplay(NULL);
                if (d) dpy = d;

                int screen = DefaultScreen(dpy);
                Window root = RootWindow(dpy, screen);

                setroot(dpy, root);
                sleep(1);
                XCloseDisplay(dpy);
        }
}

