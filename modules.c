#include "modules.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>

/* Macros */
#define BUFFER      128
#define STATUS_SIZE 30
#define LENGTH(X)   (sizeof X / sizeof X[0])
#define TO_KBITS(b) ((float)(b) / 125)

/* variables */
static long int cpuWorkCache = 0;
static long int cpuTotalCache = 0;

static unsigned long long int netReCache = 0;
static unsigned long long int netSeCache = 0;

/* function declarations */
static char *moduleFormatter(Args *arg, int formatVal);

void
nm(Args *arg, int flag, char *buff, int bufflen)
{
    unsigned long long int bytes_re, bytes_se;
    char *template = "⬆ %.2fKbi | ⬇ %.2fKbi";

    FILE *fr = fopen("/sys/class/net/eth0/statistics/rx_bytes", "r");
    if (fr == NULL) {
        return;
    }

    FILE *fs = fopen("/sys/class/net/eth0/statistics/tx_bytes", "r");
    if (fs == NULL) {
        return;
    }

    fscanf(fr, "%llu", &bytes_re);
    fclose(fr);

    fscanf(fs, "%llu", &bytes_se);
    fclose(fs);

    unsigned long long int reDiff = bytes_re - netReCache;
    unsigned long long int seDiff = bytes_se - netSeCache;

    snprintf(buff, bufflen, template, TO_KBITS(seDiff), TO_KBITS(reDiff));

    netReCache = bytes_re;
    netSeCache = bytes_se;
}

void
ut(Args *arg, int flag, char *buff, int bufflen)
{
        char *template = "%.2lfd";

        struct sysinfo si;
        sysinfo(&si);
        double uptimeDays = (double) si.uptime / 60.0 / 60.0 / 24.0;

        snprintf(buff, bufflen, template, uptimeDays);
}

/* function implementations */
void
bm(Args *arg, int flag, char *buff, int bufflen)
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

        snprintf(buff, bufflen, template, bat0p, bat1p);
}

void
nvpn(Args *arg, int flag, char *buff, int bufflen)
{
        char textbuff[1035];
        char vpnstatus[20];
        int state;

        FILE *fp;
        fp = popen("/bin/nordvpn status", "r");

        while (fscanf(fp, " %1023s", textbuff) == 1) {
                if (strcmp(textbuff, "Status:") == 0) {
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
                        snprintf(buff, bufflen, "%s%s", arg->minArgs.icon, "ON");
                        return;
                }
                snprintf(buff, bufflen, "%s%s", arg->minArgs.icon, "OFF");
                return;
        }

        snprintf(buff, bufflen, "vpn %s", moduleFormatter(arg, state));
}

void
tm(Args *arg, int flag, char *buff, int bufflen)
{
        char *format = "%s%02d:%02d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        if (flag == 0) {
                snprintf(buff, bufflen, format,
                         moduleFormatter(arg, t->tm_hour), t->tm_hour,
                         t->tm_min);
                return;
        }
        snprintf(buff, bufflen, format, arg->minArgs.icon, t->tm_hour,
                 t->tm_min);
}

void
dm(Args *arg, int flag, char *buff, int bufflen)
{
        char *format = "%s%02d.%02d.%d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        if (flag == 0) {
                snprintf(buff, bufflen, format,
                         moduleFormatter(arg, t->tm_mon + 1), t->tm_mday,
                         t->tm_mon + 1, t->tm_year + 1900);
                return;
        }

        snprintf(buff, bufflen, format, "", t->tm_mday, t->tm_mon + 1,
                 t->tm_year + 1900);
}

void
mm(Args *arg, int flag, char *buff, int bufflen)
{
        char buffer[1024] = "";

        char *format = "%s%3.2fGb";

        int memTotal = 0;
        int memAvailable = 0;

        FILE *file = fopen("/proc/meminfo", "r");
        if (file == NULL) return;

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
                snprintf(buff, bufflen, format,
                         moduleFormatter(arg, usage), usage);
                return;
        }
        snprintf(buff, bufflen, format, arg->minArgs.icon, usage);
}

void
plm(Args *arg, int flag, char *buff, int bufflen)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i = 0;

        char *format = "%s%3.2f%%";

        char line[1][128];

        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) return;

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

        cpuWorkCache = cpuWork;
        cpuTotalCache = cpuTotal;

        if (flag == 0) {
                snprintf(buff, bufflen, format,
                         moduleFormatter(arg, cpuLoad), cpuLoad);
                return;
        } 

        snprintf(buff, bufflen, format, arg->minArgs.icon,
                 cpuLoad);
}

void
ptm(Args *arg, int flag, char *buff, int bufflen)
{
        int temperature = 0;

        char *format = "%s%d°C";
        char *fallback = "";

        FILE *file = fopen("/sys/class/hwmon/hwmon0/temp1_input", "r");
        if (file == NULL) {
                file = fopen("/sys/class/hwmon/hwmon1/temp1_input", "r");
                if (file == NULL) return;
        }

        fscanf(file, "%d", &temperature);
        fclose(file);
        int temp = temperature / 1000;

        if (flag == 0) {
                snprintf(buff, bufflen, format,
                         moduleFormatter(arg, temp), temp);
                return;
        }
        snprintf(buff, bufflen, format, arg->minArgs.icon, temp);
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
