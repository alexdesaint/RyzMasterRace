# RyzMasterRace

Simple sensor reader for AMD Family 17h (Ryzen)

Froked from [rapl-read-ryzen](https://github.com/djselbeck/rapl-read-ryzen)

Based on [this](https://developer.amd.com/wp-content/resources/56255_3_03.PDF)

To load the msr module to the kernel :
```
sudo modprobe msr
```
Root privileges are needed to read from MSR device files which are located at `/dev/cpu/*/msr`.
Instead you can set the msr readeble :
```sh
sudo chmod og+r /dev/cpu/ -R
```

On some Linux kernels you may still get an `Operation not permitted` error.
Set RAWIO permissions on your executable:

```sh
sudo setcap cap_sys_rawio+ep BINARY
```

