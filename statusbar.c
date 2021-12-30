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
        char *status = malloc(sizeof(char) * 256);

        for (i=0; i<LENGTH(modules); i++)
        {
                char *text = modules[i].func();
                printf("%s\n", text);
                if (i==0) {
                        strcpy(status, text);
                        free(text);
                        continue;
                }
                strcat(status, text);
                free(text);
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
