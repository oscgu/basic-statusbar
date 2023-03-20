#include "modules.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LEN(X)         (sizeof X / sizeof X[0])
#define DEBUG          TRUE
#define MAX_STATUS_LEN 256
#define TEXT_LEN       24

/* structs */
typedef struct {
        void (*func)(Args *, int, char *, int);
        Args args;
        int flag;
} Module;

/* config file */
#include "config.h"

/* function declarations */
static void setroot();

/* variables */
static Display *dpy;
static int screen;
static Window root;

/* function implementations */
void
setroot()
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

#if DEBUG == TRUE
        printf("%s\n", status);
#else
        Display *d = XOpenDisplay(NULL);
        if (d) dpy = d;

        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);
        XStoreName(dpy, root, status);
        XCloseDisplay(dpy);
#endif
}

int
main()
{
        for (;;) {
                setroot();
                sleep(1);
        }
}
