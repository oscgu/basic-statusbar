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
char * getIcon(Args *args, int value);
int read_temp(char *path);

void
bcm(Args *arg, char *buff, int bufflen)
{
    int ch_state;

    FILE *fp = fopen("/sys/class/power_supply/AC/online", "r");
    if (!fp) return;

    fscanf(fp, "%d", &ch_state);
    fclose(fp);

    snprintf(buff, bufflen, "%s", getIcon(arg, ch_state));
}

void
nm(Args *arg, char *buff, int bufflen)
{
    unsigned long long int bytes_re, bytes_se;
    char *fmt = "⬆ %.2fKi ⬇ %.2fKi";

    FILE *fr = fopen("/sys/class/net/eth0/statistics/rx_bytes", "r");
    if (!fr) return;

    fscanf(fr, "%llu", &bytes_re);
    fclose(fr);

    FILE *fs = fopen("/sys/class/net/eth0/statistics/tx_bytes", "r");
    if (!fs) return;

    fscanf(fs, "%llu", &bytes_se);
    fclose(fs);

    unsigned long long int reDiff = bytes_re - netReCache;
    unsigned long long int seDiff = bytes_se - netSeCache;

    snprintf(buff, bufflen, fmt, TO_KBITS(seDiff), TO_KBITS(reDiff));

    netReCache = bytes_re;
    netSeCache = bytes_se;
}

void
ut(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%.2lfd";

        struct sysinfo si;
        sysinfo(&si);
        double uptimeDays = (double) si.uptime / 60.0 / 60.0 / 24.0;

        snprintf(buff, bufflen, fmt, getIcon(arg, uptimeDays), uptimeDays);
}

/* function implementations */
void
bm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s  %d%%";

        FILE *bat0 = fopen("/sys/class/power_supply/BAT0/capacity", "r");
        if (!bat0) return;

        int bat0p;
        fscanf(bat0, "%d", &bat0p);
        fclose(bat0);
        snprintf(buff, bufflen, fmt, getIcon(arg, bat0p), bat0p);

        /* TODO handle multiple bats
        FILE *bat1 = fopen("/sys/class/power_supply/BAT1/capacity", "r");
        int bat1p;
        fscanf(bat1, "%d", &bat1p);
        fclose(bat1);
        */
}

void
nvpn(Args *arg, char *buff, int bufflen)
{
        char textbuff[1035];
        char vpnstatus[20];

        FILE *fp = popen("/bin/nordvpn status", "r");
        if (!fp) return;

        while (fscanf(fp, " %1023s", textbuff) == 1) {
                if (strcmp(textbuff, "Status:") == 0) {
                        fscanf(fp, " %s", vpnstatus);
                }
        }
        pclose(fp);

        if (strcmp(vpnstatus, "Connected") == 0) {
                snprintf(buff, bufflen, "%s", getIcon(arg, 0));
        } else {
                snprintf(buff, bufflen, "%s", getIcon(arg, 1));
        }
}

void
tm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%02d:%02d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        snprintf(buff, bufflen, fmt,
                 getIcon(arg, 0), t->tm_hour,
                 t->tm_min);
}

void
dm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%02d.%02d.%d";

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        snprintf(buff, bufflen, fmt,
                 getIcon(arg, 0), t->tm_mday,
                 t->tm_mon + 1, t->tm_year + 1900);
}

void
mm(Args *arg, char *buff, int bufflen)
{
        char buffer[1024] = "";

        char *fmt = "%s%3.2fGb";

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

        snprintf(buff, bufflen, fmt,
                 getIcon(arg, usage), usage);
}

void
plm(Args *arg, char *buff, int bufflen)
{
        long int cpuTotal = 0;
        long int cpuWork = 0;
        int i = 0;

        char *fmt = "%s%3.2f%%";

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

        snprintf(buff, bufflen, fmt,
                 getIcon(arg, cpuLoad), cpuLoad);
}

void
ptm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%d°C";

        int temperature = read_temp("/sys/class/hwmon/hwmon1/temp1_input");

        snprintf(buff, bufflen, fmt, getIcon(arg, temperature), temperature);
}

void
gtm(Args *arg, char *buff, int bufflen)
{
    char *fmt = "%s%d°C";
    int temperature = read_temp("/sys/class/hwmon/hwmon1/temp1_input");

    snprintf(buff, bufflen, fmt, getIcon(arg, temperature), temperature);
}

int
read_temp(char *path)
{
    int temperature = 0;

    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    fscanf(fp, "%d", &temperature);
    fclose(fp);

    return temperature / 1000;
}

char *
getIcon(Args *args, int value) {
    int i;
    for (i=0;i<args->len; i++) {
        if (args->vals[i] > value) {
            return args->icons[i];
        }
    }

    return args->icons[i - 1];
}

