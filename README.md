# UNI-T UT181A DMM USB Communication Tool

- Features: list records, export records to CSV files
- Platform: Linux (tested on PC, Raspberry Pi 2/3, Debian 8/9)
- Ref: [How to hack UT181A protocol (Chinese)](http://www.freebuf.com/articles/terminal/145017.html)
- Ver: 0.1
- Updated: 12/13/2017
- Created: 5/11/2017
- Author: loblab

![UT181A and Raspberry Pi](https://raw.githubusercontent.com/loblab/ut181a/master/ut181a.jpg)

## Build

### On Windows

1. Download [CP2110/4 Software package for Windows, 6.7.4](https://www.silabs.com/products/development-tools/software.page=1) (I didn't try Linux version as it needs Java).
2. Run the exe to extract files, then from SiliconLabs\MCU\CP2110_4_SDK\Library\Linux, find slabhiddevice_1.0.tar.gz, slabhidtouart_1.0.tar.gz, copy to your Linux

### On Linux

```bash
sudo apt-get install build-essential pkg-config libusb-1.0-0-dev

tar -xzvf slabhiddevice_1.0.tar.gz
cd slabhiddevice
make
sudo make install

tar -xzvf slabhidtouart_1.0.tar.gz
cd slabhidtouart
make
sudo make install

cd ut181a
make
```

### Special for ARM (Raspberry Pi)

Modify slabhiddevice/Makefile, slabhidtouart/Makefile before build, using the patch file

```bash
cd slabhiddevice
patch -b Makefile <ut181a-dir>/slib_sdk/Makefile.arm.patch

cd slabhidtouart
patch -b Makefile <ut181a-dir>/slib_sdk/Makefile.arm.patch
```

BTW, use "make -f Makefile.orig" to build x86/64 version after patching.

## Configuration

### udev configration

```bash
sudo cp config/50-cp2110.rules /etc/udev/rules.d/
sudo service udev restart
```

### Library path

If cannot load library, like this:
> error while loading shared libraries: libslabhidtouart.so.1: cannot open shared object file: No such file or directory

Run
```bash
sudo ldconfig

```
as /usr/local/lib should be in /etc/ld.so.conf.d/libc.conf.

Or try
```bash
export LD_LIBRARY_PATH=/usr/local/lib

```

## Run

```bash
bin/armv7l/ut181a # Raspberry Pi
bin/x86_64/ut181a # Intel PC
```

