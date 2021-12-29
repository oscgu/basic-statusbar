#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "modules.h"

/* macros */
#define LENGTH(X)       (sizeof X / sizeof X[0])

/* structs */
typedef struct {
        char *(*func)(void);
        char symbol[10];
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
        char *status = (char *) malloc(sizeof(char) * 256);

        for (i=0; i<LENGTH(modules); i++)
        {
                if (i==0) {
                        strcpy(status, modules[i].func());
                        continue;
                }
                strcat(status, modules[i].func());
                free(modules[i].func());
        }

        Display *d = XOpenDisplay(NULL);
        if (d) dpy = d;

        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);
        XStoreName(dpy, root, status);
        XCloseDisplay(dpy);

        free(status);
}

int
main()
{
        for (;;) {
                setroot();
                sleep(10);
        }
}
