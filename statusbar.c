#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

static void getDateTime();
static void getMem();
void setroot();

static Display *dpy;
static int screen;
static Window root;
static char statusbar[256] = "";

void
getDateTime()
{
        char timeBuffer[25];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(timeBuffer, sizeof(statusbar), " ⌛%H:%M 🗓️%d-%m-%Y", t);
        strcat(statusbar, timeBuffer);
}

void
getMem()
{
        char buffer[1024] = "";
        char result[12];
        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");

        while (fscanf(file, " %1023s", buffer) == 1)
        {
                if (strcmp(buffer, "MemTotal:") == 0)
                {
                        fscanf(file, " %d", &memTotal);
                }

                if (strcmp(buffer, "MemAvailable:") == 0)
                {
                        fscanf(file, " %d", &memAvailable);
                }
        }
        fclose(file);
        sprintf(result, " 💾%.2fGb", (float)(memTotal - memAvailable) * 1e-6);
        strcat(statusbar, result);
}

void
setroot()
{
        getMem();
        getDateTime();

        Display *d = XOpenDisplay(NULL);
        if (d) {
                dpy = d;
        }
        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);
        XStoreName(dpy, root, statusbar);
        XCloseDisplay(dpy);
}

int
main()
{
        for (;;) {
                memset(&statusbar[0], 0, sizeof(statusbar));
                setroot();
                sleep(1);
        }
}
