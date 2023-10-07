#include "bcm.h"
#include "arg.h"
#include "util.h"
#include <stdio.h>

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
