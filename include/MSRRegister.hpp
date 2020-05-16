#pragma once

#include <array>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include <Register.hpp>
#include <RegisterException.hpp>

class MSRRegister {
public:
    enum MSR {
        TSC = 0,
        APIC_BAR,
        EBL_CR_POWERON,
        PATCH_LEVEL,
        MPERF,
        APERF,
        MTRRcap,
        SYSENTER_CS,
        SYSENTER_ESP,
        SYSENTER_EIP,
        MCG_CAP,
        MCG_STAT,
        MCG_CTL,
        DBG_CTL_MSR,
        BR_FROM,
        BR_TO,
        LastExcpFromIp,
        LastExcpToIp,
        MtrrVarBase,
        MtrrVarMask,
        MtrrFix_64K,
        MtrrFix_16K_0,
        MtrrFix_16K_1,
        MtrrFix_4K_0,
        MtrrFix_4K_1,
        MtrrFix_4K_2,
        MtrrFix_4K_3,
        MtrrFix_4K_4,
        MtrrFix_4K_5,
        MtrrFix_4K_6,
        MtrrFix_4K_7,
        PAT,
        MTRRdefType,
        EFER,
        STAR,
        STAR64,
        STARCOMPAT,
        SYSCALL_FLAG_MASK,
        MPerfReadOnly,
        APerfReadOnly,
        IRPerfCount,
        FS_BASE,
        GS_BASE,
        KernelGSbase,
        TSC_AUX,
        TscRateMsr,
        McaIntrCfg,
        PERF_LEGACY_CTL,
        PERF_LEGACY_CTR,
        SYS_CFG,
        HWCR,
        IORR_BASE,
        IORR_MASK,
        TOP_MEM,
        TOM2,
        McExcepRedir,
        ProcNameString,
        SMI_ON_IO_TRAP,
        SMI_ON_IO_TRAP_CTL_STS,
        IntPend,
        SmiTrigIoCycle,
        MmioCfgBaseAddr,
        PStateCurLim,
        PStateCtl,
        PStateStat,
        PStateDef,
        CStateBaseAddr,
        CpuWdtCfg,
        SMM_BASE,
        SMMAddr,
        SMMMask,
        VM_CR,
        IGNNE,
        SMM_CTL,
        VM_HSAVE_PA,
        SvmLockKey,
        LocalSmiStatus,
        AvicDoorbell,
        VMPAGE_FLUSH,
        GHCB,
        SEV_Status,
        OSVW_ID_Length,
        OSVW_Status,
        PERF_CTL,
        PERF_CTR,
        ChL3PmcCfg,
        ChL3Pmc,
        RAPL_PWR_UNIT,
        CORE_ENERGY_STAT,
        PKG_ENERGY_STAT,
        CPUID_7_Features,
        CPUID_PWR_THERM,
        CPUID_Features,
        CPUID_ExtFeatures,
        DR1_ADDR_MASK,
        DR2_ADDR_MASK,
        DR3_ADDR_MASK,
        TW_CFG,
        DR0_ADDR_MASK,
        IBS_FETCH_CTL,
        IBS_FETCH_LINADDR,
        IBS_FETCH_PHYSADDR,
        IBS_OP_CTL,
        IBS_OP_RIP,
        IBS_OP_DATA,
        IBS_OP_DATA2,
        IBS_OP_DATA3,
        IBS_DC_LINADDR,
        IBS_DC_PHYSADDR,
        IBS_CTL,
        BP_IBSTGT_RIP,
        IC_IBS_EXTD_CTL,
    };

    class Register : public ::Register {
    public:
        char const *MSRname, *lthree;

        Register(char const *MSRname, char const *name, char const *comments, char const *lthree, std::vector<size_t> offsets, std::vector<BitDescriptor> values);
    };

    static std::vector<Register> Registers;

    static unsigned int numOfCPU;
    static std::vector<int> filesStreams;
    static double joulesScale;

    static void open();

    static void read();
    static uint64_t read(int cpu, unsigned int addr);

    friend std::ostream &operator<<(std::ostream &os, MSRRegister const &m);
};