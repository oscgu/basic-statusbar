#include "nvpn.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

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
