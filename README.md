# COMPE571-Keylogger

## Requirements
The X11 based keylogger requires the installation of the following packages:
libx11-dev
libxrandr-dev
libxi-dev

The kernel module based keylogger requires installation of kernel headers build libraries. You can install them by running:
```
sudo apt-get install build-essential linux-headers-`uname -r`
```


In addition you will need python 3.6 or greater in order to run the benchmark script

## Usage
Build the X11 based keylogger and the kernel module based keylogger by running:

```
cd keylogger/
make all
cd ../kbdirq/
make all
cd ../
```

You can now run the benchmarking program by using the command:
```
sudo python3.6 benchmark.py
```
