#include <stdio.h>
#include <sys/sysinfo.h>
#include "util.h"
#include "ut.h"

void ut(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%.2lfd";

        struct sysinfo si;
        sysinfo(&si);
        double uptimeDays = (double) si.uptime / 60.0 / 60.0 / 24.0;

        snprintf(buff, bufflen, fmt, getIcon(arg, uptimeDays), uptimeDays);
}

