#include <stdio.h>
#include "util.h"
#include "ptm.h"

void
ptm(Args *arg, char *buff, int bufflen)
{
        char *fmt = "%s%d°C";

        int temperature = read_temp("/sys/class/hwmon/hwmon1/temp1_input");

        snprintf(buff, bufflen, fmt, getIcon(arg, temperature), temperature);
}

