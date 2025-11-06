# RTEMS Examples

## STM32H753zi

Using the `arm/nucleo-h743zi` BSP.

### Configure

First build the `arm/nucleo-h743zi` BSP from `rsb`.

`./waf configure --rtems=$HOME/Projs/rtems/rtems/6 --rtems-bsp=arm/nucleo-h743zi`

### Build

`./waf`

## Flash

`STM32_Programmer_CLI -c 'port=swd reset=HWrst' --download build/arm-rtems6-nucleo-h743zi/led/delay/delay.elf --start`

(The filename must end in `.elf` - default is `.exe` but can be changed in the app's `wscript` file.)

After flashing the GUI STM32CubeProgrammer might not be able to find the device but it can still be flashed from command-line. If it is necessary to "bring it back" to the GUI version, do a `west flash` from the Manpack v2 project.

# Custom BSP

In main source repo folder (in `~/Projs/rtems/src`: `git clone -b 6 https://gitlab.rtems.org/rtems/rtos/rtems.git`)

1. Copy existing BSP `bsps/arm/stm32h7/boards/stm/nucleo-h743zi/` to new `bsps/arm/stm32h7/boards/stm/nucleo-h753zi/`.
2. Copy YAML `spec/build/bsps/arm/stm32h7/bspnucleoh743zi.yml` to new `spec/build/bsps/arm/stm32h7/bspnucleoh753zi.yml` and modify the name and paths to point to the directory created in 1.

Verify with `./waf bsplist`.

Set environment variables if not done:

* `export PATH=$HOME/Projs/rtems/rtems/6/bin:$PATH` adds the path to binaries
* `export PREFIX=$HOME/Projs/rtems/rtems/6` sets the RTEMS prefix

Then follow steps from <https://docs.rtems.org/docs/6.1/user/start/bsp-build.html#manual-bsp-build>, but use `[arm/nucleo-h753zi]` as BSP name.

-----

_README from Hello project follows_

# RTEMS Test App

## Toolchain

* RTEMS 6.1 - setup according to <https://docs.rtems.org/docs/6.1/user/start/index.html>
* `arm/beagleboneblack` BSP
* Install `waf`: `curl https://waf.io/waf-2.0.19 > waf && chmod +x waf` (and the Git submodule should be added as well: `git submodule add https://gitlab.rtems.org/rtems/tools/rtems_waf.git rtems_waf`)

## Environment variables

* `export PATH=$HOME/Projs/rtems/rtems/6/bin:$PATH` adds the path to binaries
* `export PREFIX=$HOME/Projs/rtems/rtems/6` sets the RTEMS prefix for e.g. `waf`

## Building

1. Configure: `./waf configure --rtems-bsp=arm/beagleboneblack`
2. Build: `./waf`

## SD Card

### Prepare SD Card

Follow instructions [here](https://subscription.packtpub.com/book/iot-and-hardware/9781785289736/1/ch01lvl1sec05/creating-partitions-and-formatting-the-sd-card) - use `lsblk` to find device name for SD Card.  
Also [this script](https://github.com/YoctoForBeaglebone/BeagleScripts/blob/master/prepare_card.sh) but it has hardcoded `mmcblk0` and `p1/p2` so it requires tweaking for USB mounted SD Cards.

Also info here: <https://github.com/alanc98/rtems-cfs-demo/tree/main/sd-card>

### Device Tree

Using intructions [here](https://blog.thelunatic.dev/getting-started-bbb-2/) - NB: use commit `19a6ceb89dbacf74697d493e48c388767126d418` after checkout. Otherwise, the BeagleBone Black DTS file is not available.

### Binary Image

Copy the build `hello.exe` to `./image` and run:

```
arm-rtems5-objcopy hello.exe -O binary app.bin
gzip -9 app.bin
mkimage -A arm -O linux -T kernel -a 0x80000000 -e 0x80000000 -n RTEMS -d app.bin.gz rtems-app.img
```

### uEnv.txt

Use the following:

```
setenv bootdelay 5
uenvcmd=run boot
boot=fatload mmc 0 0x80800000 rtems-app.img ; fatload mmc 0 0x88000000 am335x-boneblack.dtb ; bootm 0x80800000 - 0x88000000
```

This is also from [this article](https://blog.thelunatic.dev/getting-started-bbb-2/), but the `boot` command is only on _one_ line.

### Copy Files

Copy the following files to the BOOT partition:

1. `am335x-boneblack.dtb`
2. `rtems-app.img`
3. `uEnv.txt`

(`MLO` is _not_ required.)

## Booting

No need to press any button while booting - insert SD card and power up with FTDI-USB connected to view console output.

```
U-Boot SPL 2019.04-00002-g31a8ae0206 (May 13 2020 - 09:26:17 -0500)
Trying to boot from MMC2
Loading Environment from EXT4... 
** Unable to use mmc 0:1 for loading the env **


U-Boot 2019.04-00002-g31a8ae0206 (May 13 2020 - 09:26:17 -0500), Build: jenkins-github_Bootloader-Builder-139

CPU  : AM335X-GP rev 2.1
I2C:   ready
DRAM:  512 MiB
No match for driver 'omap_hsmmc'
No match for driver 'omap_hsmmc'
Some drivers were not found
Reset Source: Power-on reset has occurred.
RTC 32KCLK Source: External.
MMC:   OMAP SD/MMC: 0, OMAP SD/MMC: 1
Loading Environment from EXT4... 
** Unable to use mmc 0:1 for loading the env **
Board: BeagleBone Black
<ethaddr> not set. Validating first E-fuse MAC
BeagleBone Black:
BeagleBone: cape eeprom: i2c_probe: 0x54:
BeagleBone: cape eeprom: i2c_probe: 0x55:
BeagleBone: cape eeprom: i2c_probe: 0x56:
BeagleBone: cape eeprom: i2c_probe: 0x57:
Net:   eth0: MII MODE
cpsw, usb_ether
Press SPACE to abort autoboot in 0 seconds
board_name=[A335BNLT] ...
board_rev=[00C0] ...
switch to partitions #0, OK
mmc0 is current device
SD/MMC found on device 0
switch to partitions #0, OK
mmc0 is current device
Scanning mmc 0:1...
51485 bytes read in 6 ms (8.2 MiB/s)
gpio: pin 56 (gpio 56) value is 0
gpio: pin 55 (gpio 55) value is 0
gpio: pin 54 (gpio 54) value is 0
gpio: pin 53 (gpio 53) value is 1
switch to partitions #0, OK
mmc0 is current device
gpio: pin 54 (gpio 54) value is 1
Checking for: /uEnv.txt ...
160 bytes read in 1 ms (156.3 KiB/s)
gpio: pin 55 (gpio 55) value is 1
Loaded environment from /uEnv.txt
Importing environment from mmc ...
Checking if uenvcmd is set ...
gpio: pin 56 (gpio 56) value is 1
Running uenvcmd ...
63910 bytes read in 6 ms (10.2 MiB/s)
51485 bytes read in 6 ms (8.2 MiB/s)
## Booting kernel from Legacy Image at 80800000 ...
   Image Name:   RTEMS
   Created:      2025-11-05   9:25:04 UTC
   Image Type:   ARM Linux Kernel Image (gzip compressed)
   Data Size:    63846 Bytes = 62.3 KiB
   Load Address: 80000000
   Entry Point:  80000000
   Verifying Checksum ... OK
## Flattened Device Tree blob at 88000000
   Booting using the fdt blob at 0x88000000
   Uncompressing Kernel Image ... OK
   Loading Device Tree to 8fff0000, end 8ffff91c ... OK

Starting kernel ...


RTEMS Beagleboard: am335x-based
        ARM Debug: 0x4b141000


*** BEGIN OF TEST HELLO WORLD ***
*** TEST VERSION: 6.1.0.not-released
*** TEST STATE: EXPECTED_PASS
*** TEST BUILD: RTEMS_POSIX_API
*** TEST TOOLS: 13.3.0 20240521 (RTEMS 6, RSB no-repo, Newlib 1b3dcfd)
Hello World

*** END OF TEST HELLO WORLD ***


[ RTEMS shutdown ]
RTEMS version: 6.1.0.not-released
RTEMS tools: 13.3.0 20240521 (RTEMS 6, RSB no-repo, Newlib 1b3dcfd)
executing thread ID: 0x0a010001
executing thread name: UI1 
```

after which the device reboots and output repeats.

## Flashing STM32

west flash uses:

`/opt/st/stm32cubeclt_1.19.0/STM32CubeProgrammer/bin/STM32_Programmer_CLI --connect 'port=swd reset=HWrst' --download /workspace/build/manpack-v2/target/zephyr/zephyr.hex --start`

Basic:

`STM32_Programmer_CLI -c port=SWD -w delay.elf -v -rst`

-----

_Original README follows_

RTEMS Examples
==============

This is a set of example RTEMS programs.  Each subdirectory
reflects a set of related examples.


| Directory    | Description                                             |
| ---          | ---                                                     |
| benchmarks   | Collection of various public benchmark program          |
| classic_api  | Classic API examples (no LED)                           |
| cxx          | C++ examples                                            |
| file_io      | File I/O                                                |
| filesystem   | File systems, check inside for more details             |
| gdb          | Programs to exercise GDB features                       |
| hello        | Hello World Variations                                  |
| led          | LED Blinker Varations                                   |
| lvgl_hello   | LittleVGL graphics app example using libbsd framebuffer |
| micromonitor | MicroMonitor interaction examples                       |
| misc         | Miscellaneous                                           |
| posix_api    | POSIX API examples (no led)                             |
| schedsim     | RTEMS Scheduler Simulator examples                      |
| ticker       | Ticker Variations                                       |
| uboot        | U-Boot interaction examples                             |


RTEMS Waf
=========

You can find the Waf project here:

  * https://waf.io

Simple instructions on How to set up Waf is here:

  * http://www.rtems.org/ftp/pub/rtems/people/chrisj/rtl/rtems-linker/waf.html

Steps
-----

  1. Build or install the tools. In this example the path is
     `$HOME/Development/rtems/5`.

  2. Build and install the RTEMS Board Support Packages you want to use. In this
     example the path is `$HOME/Development/rtems/build/5`.

  3. Unpack this package somewhere, anywhere on your disk and change into the top
     level directory.

  4. Populate the git submodule:

     ```shell
     $ git submodule init
     $ git submodule update
     ```

  5. Configure with your specific settings. In this case the path to the tools
     and RTEMS and provided on the command line and so do not need to be in your
     path or environment [^1] and we limit the build to a single BSP. The single
     BSP should be the `sparc/erc32` BSP since this is the easiest BSP to use
     on a simulator. After you have successfully build the executables for this
     BSP and run them on the simulator, you can repeat this exercise with the
     BSP you are most interested in.

     ```shell
     $ waf configure --rtems=$HOME/Development/rtems/build/5 \
                     --rtems-tools=$HOME/Development/rtems/5 \
                     --rtems-bsps=sparc/erc32
     ```


     You can use these to build more than one BSP at a time:

     ```shell
     $ waf configure --rtems-archs=sparc,i386
     $ waf configure --rtems-bsps=sparc/erc32,i386/pc586
     ```

  6. Build:

     ```shell
     $ waf
     ```

  7. Run the executable using the simulator

     ```shell
      $ $HOME/Development/rtems/5/bin/sparc-rtems5-run \
        ./build/sparc-rtems5-erc32/hello
     ```

[^1]: It is good practice to keep your environment as empty as possible. Setting
      paths to tools or specific values to configure or control builds is
      dangerous because settings can leak between different builds and change
      what you expect a build to do. The waf tool used here lets you specify on
      the command line the tools and RTEMS paths and this is embedded in waf's
      configuration information. If you have a few source trees working at any
      one time with different tool sets or configurations you can easly move
      between them safe in the knowledge that one build will not infect another.


Makefile
========

The Makefile system used in these examples is dependent on the
environment variable RTEMS_MAKEFILE_PATH being set. This can be
set on each command line that invokes "make" or it can be exported
into your environment.

RTEMS_MAKEFILE_PATH points to the installed BSP image you are compiling
for. It is composed as follows:

  ```shell
  @prefix@/@target@/BSP
  ```

where:

  * Prefix is the BSP install point or prefix
  * Target is the tool target (e.g. sparc-rtems5)
  * BSP is the BSP you are building for (e.g. erc32)

A coupte of examples:

  ```shell
  export RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32
  make clean
  make
  ```

OR:

  ```shell
  RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32 make clean
  RTEMS_MAKEFILE_PATH=${HOME}/rtems-work/tools/5/sparc-rtems5/erc32 make
  ```

You can switch a build from one BSP to another by changing the value
of RTEMS_MAKEFILE_PATH but be careful to "make clean" on the old BSP
before building the new one.

Deprecration Warning: This style of Makefile has been included in RTEMS
since the earliest days. However, it is being replaced by the use of waf
and something akin to pkgconfig to obtain compilation settings. The intent
is to make it easier for users to configure their preferred build system
for an RTEMS application.
