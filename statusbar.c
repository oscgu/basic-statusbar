#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "config.h"
#include "modules.h"

/* function declarations */
void getCpuLoad();
void getLoadAvg();
void getDateTime();
void getMem();
void setroot();

/* variables */
static Display *dpy;
static int screen;
static Window root;
static char statusbar[256] = "";
static int cpuWorkCache = 0;
long static int cpuTotalCache = 0;

/* Caches */
char dateTime[25];
char memUsage[12];
char loadAvg[10];
char cpuCurrentLoad[20];

/* function implementations */

void
setroot()
{
        getCpuLoad(&cpuCurrentLoad);
        getLoadAvg(&loadAvg);
        getMem(&memUsage);
        getDateTime(&dateTime);

        int totalSize = strlen(dateTime) + strlen(memUsage) + strlen(loadAvg) + strlen(cpuCurrentLoad) + 1;
        printf("%d\n", totalSize);

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
