#include "modules.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* macros */
#define LEN(X)   (sizeof X / sizeof X[0])
#define DEBUG    TRUE
#define MAX_STATUS_LEN  256
#define TEXT_LEN 24

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
        unsigned int i;
        char status[MAX_STATUS_LEN];
        char text[TEXT_LEN];


        for (i = 0; i < LEN(modules); i++) {
                modules[i].func(&modules[i].args, modules[i].flag, text,
                                LEN(text));

                if (i == 0) {
                        strcpy(status, text);
                        continue;
                }
                strcat(status, &delimitter);
                strcat(status, text);
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
