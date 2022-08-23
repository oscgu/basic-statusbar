#include "modules.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LENGTH(X) (sizeof X / sizeof X[0])
#define DEBUG     TRUE

/* structs */
typedef struct {
        char *(*func)(Args *, int);
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
        unsigned int i;
        char status[256];

        for (i = 0; i < LENGTH(modules); i++) {
                char *text =
                    modules[i].func(&modules[i].args, modules[i].flag);
                if (i == 0) {
                        strcpy(status, text);
                        free(text);
                        continue;
                }
                strcat(status, &delimitter);
                strcat(status, text);
                free(text);
        }

#if DEBUG != TRUE
        Display *d = XOpenDisplay(NULL);
        if (d) dpy = d;

        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);
        XStoreName(dpy, root, status);
        XCloseDisplay(dpy);
#else
        printf("%s\n", status);
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
