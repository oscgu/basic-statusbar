#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

static void getCpuLoad();
static void getLoadAvg();
static void getDateTime();
static void getMem();
void setroot();

static Display *dpy;
static int screen;
static Window root;
static char statusbar[256] = "";
static int cpuWorkCache = 0;
long static int cpuTotalCache = 0;

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
getLoadAvg()
{
        char result[10] = " 🖥️";
        int n = 5;
        int i;

        FILE *file = fopen("/proc/loadavg", "r");
        
        while ((i = fgetc(file)) != ' ')
        {
                result[n++] = (char) i;
        }
        fclose(file);
        strcat(statusbar, result);
}

void
getCpuLoad()
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i=0;

        char result[20];
        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");

        fgets(line[0], 100, file);
        line[0][strlen(line[0]) - 1] = '\0';
        fclose(file);

        char *token = strtok(line[0], " ");

        while (token != NULL)
        {
                i++;
                token = strtok(NULL, " ");
                if (i>0 && i<7)
                {
                        cpuTotal += atoi(token);
                }
                if (i>0 && i<4)
                {
                        cpuWork += atoi(token);
                }
        }
        float cpuLoad = fabs((float)(cpuWork - cpuWorkCache) / (float)(cpuTotal - cpuTotalCache) * 100);

        sprintf(result, " %s%.2f%%",  cpuLoad < 60 ? "🧊": "🔥", cpuLoad );
        strcat(statusbar, result);
        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;
}

void
setroot()
{
        getCpuLoad();
        getLoadAvg();
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
