#include "util.h"
#include <stdio.h>
#include "gtm.h"

void gtm(Args *arg, char *buff, int bufflen)
{
    char *fmt = "%s%d°C";
    int temperature = read_temp("/sys/class/hwmon/hwmon1/temp1_input");

    snprintf(buff, bufflen, fmt, getIcon(arg, temperature), temperature);
}

