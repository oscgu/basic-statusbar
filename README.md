# basic-statusbar

<!--toc:start-->
- [basic-statusbar](#basic-statusbar)
  - [Build](#build)
    - [Dependencies](#dependencies)
  - [Tools](#tools)
  - [Libs](#libs)
    - [Configure](#configure)
    - [Make](#make)
  - [Usage](#usage)
  - [Modules](#modules)
    - [MemoryModule](#memorymodule)
    - [ProcessorLoadModule](#processorloadmodule)
    - [ProcessorTempModule](#processortempmodule)
    - [Date/TimeModule](#datetimemodule)
    - [Nordvpn](#nordvpn)
    - [Uptime](#uptime)
    - [Network](#network)
    - [BatteryModule](#batterymodule)
    - [BatteryChargingMode](#batterychargingmode)
    - [Script](#script)
<!--toc:end-->

A very basic statusbar for **[dwm](https://dwm.suckless.org)**,
I quickly threw together for my fresh install :)

![statusbar](https://github.com/oscgu/basic-statusbar/assets/94227101/7a24b018-8c61-4f9a-93d2-28d934d8516e)

## Build

### Dependencies

## Tools

- dwm
- make
- clang

## Libs

- libx11

### Configure

Edit [config.h](./config.h)

Modules are added as follows:

```
Modules modules = {
    { module_short_name, { icon_count, values[], icons[] } }
}
```

The [default formatter](./util.c) will chose icons depending on the provided values, e.g:

```
values[] = 1, 2
icons[] = "a", "b", "c"

if value < 1 then "a"
if value > 1 and value < 2 then "b"
if value > 2 then "c"
```

### Make

Build the binary:

```sh
make
```

Install the binary:

```sh
make install
```

## Usage

Simply execute the statusbar after starting [dwm](https://dwm.suckless.org)

`statusbar`

## Modules

### MemoryModule

| Shortname    | Source    |
| ------------ | --------- |
| [mm](./mm.c) | /proc/mem |

Shows the current memory usage in Gb

### ProcessorLoadModule

| Shortname      | Source      |
| -------------- | ----------- |
| [plm](./plm.c) | /proc/stats |

Shows the current cpu usage in %

### ProcessorTempModule

| Shortname      | Source                              |
| -------------- | ----------------------------------- |
| [ptm](./ptm.c) | /sys/class/hwmon/hwmon1/temp1_input |

Shows the current cpu temperature in Celsius

### Date/TimeModule

| Shortname                 | Source |
| ------------------------- | ------ |
| [dm](./dm.c)/[tm](./tm.c) | time.h |

Shows the current date/time

### Nordvpn

| Shortname        | Source              |
| ---------------- | ------------------- |
| [nvpn](./nvpn.c) | /bin/nordvpn status |

Shows wether you are connected to a vpn

### Uptime

| Shortname    | Source         |
| ------------ | -------------- |
| [ut](./ut.c) | /sys/sysinfo.h |

Shows the current uptime in days

### Network

| Shortname    | Source                                  |
| ------------ | --------------------------------------- |
| [nm](./nm.c) | /sys/class/net/eth0/statistics/rx_bytes |

Shows the current network traffic (receive & send) of eth0 (broken as of right now)

### BatteryModule

| Shortname    | Source                               |
| ------------ | ------------------------------------ |
| [bm](./bm.c) | /sys/class/power_suply/BAT0/capacity |

Shows the current battery percentage

### BatteryChargingMode

| Shortname      | Source                           |
| -------------- | -------------------------------- |
| [bcm](./bcm.c) | /sys/class/power_suply/AC/online |

Shows the current charging state

### Script

| Shortname            | Source |
| -------------------- | ------ |
| [script](./script.c) | byo    |

Runs a script and puts the output into the status.
Make sure you dont exceed the maximum module text length of 32 chars.
