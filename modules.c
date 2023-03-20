#include "modules.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>

/* Macros */
#define BUFFER      128
#define STATUS_SIZE 30
#define LENGTH(X)   (sizeof X / sizeof X[0])

/* variables */
static long int cpuWorkCache = 0;
static long int cpuTotalCache = 0;

/* function declarations */
static char *moduleFormatter(Args *arg, int formatVal);

char *
ut(Args *arg, int flag)
{
    char *template = "%.2lfd";

    struct sysinfo si;
    sysinfo(&si);
    double uptimeDays = (double) si.uptime / 60.0 / 60.0 / 24.0;

    int statusSize = strlen(template) - 4 + 6; //9999.99d
    char *status = malloc(statusSize);
    snprintf(status, statusSize, template, uptimeDays);

    return status;
}

/* function implementations */
char *
bm(Args *arg, int flag)
{
        char *template = "Bat: %d%% | %d%%";

        FILE *bat0 = fopen("/sys/class/power_supply/BAT0/capacity", "r");
        int bat0p;
        fscanf(bat0, "%d", &bat0p);
        fclose(bat0);

        FILE *bat1 = fopen("/sys/class/power_supply/BAT1/capacity", "r");
        int bat1p;
        fscanf(bat1, "%d", &bat1p);
        fclose(bat1);

        int statusSize = strlen(template) - 6 + 3 + 3; //6 = placeholders, 3 = bat0|1
        char *status = malloc(statusSize);
        snprintf(status, statusSize, template, bat0p, bat1p);

        return status;
}

char *
nvpn(Args *arg, int flag)
{
        char *status = malloc(sizeof(char) * STATUS_SIZE);
        char buff[1035];
        char vpnstatus[20];
        int state;

        FILE *fp;
        fp = popen("/bin/nordvpn status", "r");

        while (fscanf(fp, " %1023s", buff) == 1) {
                if (strcmp(buff, "Status:") == 0) {
                        fscanf(fp, " %s", vpnstatus);
                }
        }
        pclose(fp);

        if (strcmp(vpnstatus, "Connected") == 0) {
                state = 0;
        } else {
                state = 1;
        }

        if (flag == 1) {
                if (!state) {
                        snprintf(status, 50, "%s%s", arg->minArgs.icon, "ON");
                        return status;
                }
                snprintf(status, 50, "%s%s", arg->minArgs.icon, "OFF");
                return status;
        }
        snprintf(status, 50, "vpn %s", moduleFormatter(arg, state));
        return status;
}

char *
tm(Args *arg, int flag)
{
        char *format = "%s%02d:%02d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int statusSize = strlen(format) - 5;
        char *timeBuff = malloc(statusSize);

        if (flag == 0) {
                snprintf(timeBuff, statusSize, format,
                         moduleFormatter(arg, t->tm_hour), t->tm_hour,
                         t->tm_min);
                return timeBuff;
        }
        snprintf(timeBuff, statusSize, format, arg->minArgs.icon, t->tm_hour,
                 t->tm_min);

        return timeBuff;
}

char *
dm(Args *arg, int flag)
{
        char *format = "%s%02d.%02d.%d";
        int statusSize = 11;
        char *dateBuff = malloc(statusSize);

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        snprintf(dateBuff, statusSize, format, "", t->tm_mday, t->tm_mon + 1,
                 t->tm_year + 1900);

        if (flag == 0) {
                snprintf(dateBuff, statusSize, format,
                         moduleFormatter(arg, t->tm_mon + 1), t->tm_mday,
                         t->tm_mon + 1, t->tm_year + 1900);
                snprintf(dateBuff, statusSize, format, arg->minArgs.icon,
                         t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
                return dateBuff;
        }

        return dateBuff;
}

char *
mm(Args *arg, int flag)
{
        char buffer[1024] = "";

        char *format = "%s%.2fGb";
        int statusSize = 8;
        char *status = malloc(statusSize);

        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");
        if (file == NULL) return status;

        while (fscanf(file, " %1023s", buffer) == 1) {
                if (strcmp(buffer, "MemTotal:") == 0) {
                        fscanf(file, " %d", &memTotal);
                }
                if (strcmp(buffer, "MemAvailable:") == 0) {
                        fscanf(file, " %d", &memAvailable);
                }
        }
        fclose(file);

        float usage = (float) (memTotal - memAvailable) * 1e-6;

        if (flag == 0) {
                snprintf(status, statusSize, format,
                         moduleFormatter(arg, usage), usage);
                return status;
        }
        snprintf(status, statusSize, format, arg->minArgs.icon, usage);

        return status;
}

char *
plm(Args *arg, int flag)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i = 0;

        char *format = "%s%.2f%%";
        int statusSize = 7;
        char *status = malloc(sizeof(char) * BUFFER);

        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) return "";

        fgets(line[0], 100, file);
        line[0][strlen(line[0]) - 1] = '\0';
        fclose(file);

        char *token = strtok(line[0], " ");

        while (token != NULL) {
                i++;
                token = strtok(NULL, " ");
                if (i > 0 && i < 7) { cpuTotal += atoi(token); }
                if (i > 0 && i < 4) { cpuWork += atoi(token); }
        }
        float cpuLoad = fabs((float) (cpuWork - cpuWorkCache) /
                             (float) (cpuTotal - cpuTotalCache) * 100);

        if (flag == 0) {
                snprintf(status, statusSize, format,
                         moduleFormatter(arg, cpuLoad), cpuLoad);
        } else {
                snprintf(status, statusSize, format, arg->minArgs.icon,
                         cpuLoad);
        }
        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        return status;
}

char *
ptm(Args *arg, int flag)
{
        int temperature = 0;

        char *format = "%s%d°C";
        int statusSize = 7;
        char *status = malloc(statusSize);
        char *fallback = "";

        FILE *file = fopen("/sys/class/hwmon/hwmon0/temp1_input", "r");
        if (file == NULL) {
                file = fopen("/sys/class/hwmon/hwmon1/temp1_input", "r");
                if (file == NULL) { return fallback; }
        }

        fscanf(file, "%d", &temperature);
        fclose(file);
        int temp = temperature / 1000;

        if (flag == 0) {
                snprintf(status, statusSize, format,
                         moduleFormatter(arg, temp), temp);
                return status;
        }
        snprintf(status, statusSize, format, arg->minArgs.icon, temp);

        return status;
}

static char *
moduleFormatter(Args *arg, int formatVal)
{
        return formatVal <= arg->maxArgs.lowVal ? arg->maxArgs.lowIcon
             : formatVal >= arg->maxArgs.lowVal &&
                       formatVal <= arg->maxArgs.highVal
                 ? arg->maxArgs.midIcon
                 : arg->maxArgs.highIcon;
}
