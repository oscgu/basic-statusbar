#include "nvpn.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

void
nvpn(Args *arg, char *buff, int bufflen)
{
        char textbuff[19];
        char country[20] = {0};
        char vpnstatus[40];

        FILE *fp = popen("/bin/nordvpn status", "r");
        if (!fp) return;

        while (fscanf(fp, " %18s", textbuff) == 1) {
                if (strcmp(textbuff, "Status:") == 0) {
                        fscanf(fp, " %s", vpnstatus);
                }
                if (strcmp(textbuff, "Country:") == 0) {
                        fscanf(fp, " %s", country);
                }
        }
        pclose(fp);

        if (strcmp(vpnstatus, "Connected") == 0) {
                snprintf(buff, bufflen, "%s %s", getIcon(arg, 0), country);
        } else {
                snprintf(buff, bufflen, "%s %s", getIcon(arg, 1), country);
        }
}
