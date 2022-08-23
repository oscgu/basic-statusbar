#include "modules.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Macros */
#define BUFFER    128
#define LENGTH(X) (sizeof X / sizeof X[0])

/* variables */
static long int cpuWorkCache = 0;
static long int cpuTotalCache = 0;

/* function declarations */
static char *moduleFormatter(Args *arg, int formatVal);

/* function implementations */
char *
bm(Args *arg, int flag)
{
        char *status = malloc(sizeof(char) * 30);
        int bat0p;
        int bat1p;

        FILE *bat0 = fopen("/sys/class/power_supply/BAT0/capacity", "r");
        FILE *bat1 = fopen("/sys/class/power_supply/BAT1/capacity", "r");

        fscanf(bat0, "%d", &bat0p);
        fscanf(bat1, "%d", &bat1p);

        fclose(bat0);
        fclose(bat1);

        snprintf(status, 30, "Bat: %d%% | %d%%", bat0p, bat1p);

        return status;
}

char *
nvpn(Args *arg, int flag)
{
        char *status = malloc(sizeof(char) * 30);
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
        char *timeBuff = malloc(sizeof(char) * BUFFER);
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        if (flag == 0) {
                snprintf(timeBuff, BUFFER, format,
                         moduleFormatter(arg, t->tm_hour), t->tm_hour,
                         t->tm_min);
                return timeBuff;
        }
        snprintf(timeBuff, BUFFER, format, arg->minArgs.icon, t->tm_hour,
                 t->tm_min);
        return timeBuff;
}

char *
dm(Args *arg, int flag)
{
        char *format = "%s%02d.%02d.%d";
        char *dateBuff = malloc(sizeof(char) * BUFFER);
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        snprintf(dateBuff, BUFFER, format, "", t->tm_mday, t->tm_mon + 1,
                 t->tm_year + 1900);

        if (flag == 0) {
                snprintf(dateBuff, BUFFER, format,
                         moduleFormatter(arg, t->tm_mon + 1), t->tm_mday,
                         t->tm_mon + 1, t->tm_year + 1900);
                return dateBuff;
        } else if (flag == 1) {
                snprintf(dateBuff, BUFFER, format, arg->minArgs.icon,
                         t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
                return dateBuff;
        }

        return dateBuff;
}

char *
mm(Args *arg, int flag)
{
        char buffer[1024] = "";
        char *memBuff = malloc(sizeof(char) * BUFFER);

        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");
        if (file == NULL) return memBuff;

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
                snprintf(memBuff, BUFFER, "%s%.2fGb",
                         moduleFormatter(arg, usage), usage);
                return memBuff;
        }
        snprintf(memBuff, BUFFER, "%s%.2fGb", arg->minArgs.icon, usage);

        return memBuff;
}

char *
plm(Args *arg, int flag)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i = 0;

        char *loadBuff = malloc(sizeof(char) * BUFFER);
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
                snprintf(loadBuff, BUFFER, "%s%.2f%%",
                         moduleFormatter(arg, cpuLoad), cpuLoad);
        } else {
                snprintf(loadBuff, BUFFER, "%s%.2f%%", arg->minArgs.icon,
                         cpuLoad);
        }
        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        return loadBuff;
}

char *
ptm(Args *arg, int flag)
{
        int temperature = 0;
        char *tempBuff = malloc(sizeof(char) * BUFFER);
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
                snprintf(tempBuff, BUFFER, "%s%d°C",
                         moduleFormatter(arg, temp), temp);
                return tempBuff;
        }
        snprintf(tempBuff, BUFFER, "%s%d°C", arg->minArgs.icon, temp);
        return tempBuff;
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
