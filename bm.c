#include <stdio.h>
#include "util.h"
#include "bm.h"

void bm(Args *arg, char *buff, int bufflen)
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

