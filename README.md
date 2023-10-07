# basic-statusbar

A very basic statusbar for **[dwm](https://dwm.suckless.org)**,
I quickly threw together for my fresh install :)

![statusbar](https://github.com/oscgu/basic-statusbar/assets/94227101/7a24b018-8c61-4f9a-93d2-28d934d8516e)

## Prerequisites

Make sure you have applied the libxft-bgra patch and this removed
from `drw.c` for emojis to render

```c
FcBool iscol;

if(FcPatternGetBool(xfont->pattern, FC_COLOR, 0, &iscol) == FcResultMatch && iscol) {

    XftFontClose(drw->dpy, xfont);

    return NULL;
}
```

## Setting it up

Enter the folder and  ```make``` to compile it

Add this to your .xinitrc

```sh
exec /basic-statusbar/statusbar
```

## Where are the values from

| Module                      | Source                                  |
| --------------------------- | ----------------------------------------|
| MemoryModule        (mm)    | /proc/mem                               |
| ProcessorLoadModule (plm)   | /proc/stats                             |
| ProcessorTempModule (ptm)   | /sys/class/hwmon/hwmon1/temp1_input     |
| Date/TimeModule     (dm/tm) | time.h                                  |
| NordVPN             (nvpn)  | /bin/nordvpn status                     |
| Uptime              (ut)    | /sys/sysinfo.h                          |
| Network             (nm)    | /sys/class/net/eth0/statistics/rx_bytes |
| BatteryModule       (bm)    | /sys/class/power_suply/BAT0/capacity    |
| BatteryChargingMode (bcm)   | /sys/class/power_suply/AC/online        |
