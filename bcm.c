#include "arg.h"
#include <stdio.h>
#include "util.h"
#include "bcm.h"

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

