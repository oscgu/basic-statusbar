#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "modules.h"

static void setroot();
/* variables */
static Display *dpy;
static int screen;
static Window root;

/* Caches */
char dateTime[35];
char memUsage[12];
char loadAvg[10];
char cpuCurrentLoad[20];
static long int cpuWorkCache = 0;
static long int cpuTotalCache = 0;

/* function implementations */

void
setroot()
{
        getCpuLoad(cpuCurrentLoad, &cpuWorkCache, &cpuTotalCache);
        getLoadAvg(loadAvg);
        getMem(memUsage);
        getDateTime(dateTime);

        int totalSize = strlen(dateTime) + strlen(memUsage) + strlen(loadAvg) + strlen(cpuCurrentLoad) + 1;

        char *status = (char *) malloc(totalSize);

        strcpy(status, cpuCurrentLoad);
        strcat(status, memUsage);
        strcat(status, loadAvg);
        strcat(status, dateTime);

        Display *d = XOpenDisplay(NULL);
        if (d) {
                dpy = d;
        }
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
