# RyzMasterRace

Simple sensor reader for AMD Family 17h (Ryzen). In cli or qt.

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

# Credit

The MSR and the CPUID are based on the [AMD documentation](https://developer.amd.com/wp-content/resources/56255_3_03.PDF)
For others registers, there is [no documentation](https://www.reddit.com/r/Amd/comments/amovex/requesting_bios_and_kernel_developer_guide_bkdg/), so here is a list of projects that helped :
- https://github.com/tomrus88/RTCSharp
- https://github.com/FlyGoat
  - https://github.com/FlyGoat/ryzen_nb_smu
  - https://github.com/FlyGoat/RyzenAdj
- https://github.com/zamaudio/smutool
- https://github.com/irusanov
  - https://github.com/irusanov/ZenStates
  - https://github.com/irusanov/ZenStates-Linux
  - https://github.com/irusanov/ZenTimings
  - https://github.com/irusanov/SMUDebugTool
