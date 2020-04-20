#include <MSRRegister.hpp>
#include <cmath>
#include <cstring>
#include <fcntl.h>
#include <iostream>

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

/* Regex changes :
 * - ((MSR.{4}_.{3,4}(\[.\.\.\..\])?)\s\[(.*?)\] \((.*?)\) -->)====="} }, {"$1", "$3", R"=====(* - (_lthree\[1:0\].+?)(?=MSR)MSR(.*) -->)=====", "$1", {0x$2}, R"=====(* - ($\s*(\d{1,2}):(\d{1,2})\s) --> \n$2, $3, R"=====(* - R"=====\(\s*Bits\s*Description --> \n{\n{
 * - \)====="\}, \{"   --> \n)====="}\n}, {"
 * - ($\s*(\d{1,2}):(\d{1,2})\s) --> \n$2, $3, R"=====(* - ($\s*(\d{1,2})\s) --> \n$2, $2, R"=====(* - ([^\{])\s(\d{1,2}, \d{1,2}) --> .)====="},\n{$2
 */

const std::vector<MSRRegister::Register> MSRRegister::Registers = {
        {"MSR0000_0010", "Time Stamp Counter", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
    The TSC uses a common reference for all sockets, cores and threads.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000010},
         {{63, 0, R"=====(TSC: time stamp counter. Read-write,Volatile. Reset: 0000_0000_0000_0000h. The TSC increments at the P0
    frequency. The TSC counts at the same rate in all P-states, all C states, S0, or S1. A read of this MSR in guest
            mode is affected by Core::X86::Msr::TscRateMsr. The value (TSC/TSCRatio) is the TSC P0 frequency based
    value (as if TSCRatio == 1.0) when (TSCRatio != 1.0).)====="}}},
        {"MSR0000_001B", "APIC Base Address", R"=====(Reset: 0000_0000_FEE0_0X00h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x0000001B},
         {{63, 48, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000h.)====="}, {47, 12, R"=====(ApicBar[47:12]: APIC base address register. Read-write. Reset: 0_000F_EE00h. Specifies the base address, physical address [47:12], for the APICXX register set in xAPIC mode. See 2.1.8.2.1.2 [APIC Register Space].)====="}, {11, 11, R"=====(ApicEn: APIC enable. Read-write. Reset: 0. 0=Disable Local Apic. 1=Local APIC is enabled in xAPIC mode. See 2.1.8.2.1.2 [APIC Register Space].)====="}, {10, 10, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0.)====="}, {9, 9, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0.)====="}, {8, 8, R"=====(BSC: boot strap core. Read-write,Volatile. Reset: X. 0=The core is not the boot core of the BSP. 1=The core is the boot core of the BSP.)====="}, {7, 0, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="}}},
        {"MSR0000_002A", "Cluster ID", R"=====(Reset: 0000_0000_0000_0000h.
    Writes to this register result in a GP fault with error code 0.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x0000002A},
         {{63, 18, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000_0000_0000h.)====="}, {17, 16, R"=====(ClusterID. Read,Error-on-write. Reset: 0h. The field does not affect hardware.)====="}, {15, 0, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000h.)====="}}},
        {"MSR0000_008B", "Patch Level", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0x0000008B},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(PatchLevel. Read,Error-on-write,Volatile. Reset: 0000_0000h. This returns an identification number for the
            microcode patch that has been loaded. If no patch has been loaded, this returns 0.)====="}}},
        {"MSR0000_00E7", "Max Performance Frequency Clock Count", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000000E7},
         {{63, 0, R"=====(MPERF: maximum core clocks counter. Read-write,Volatile. Reset: 0000_0000_0000_0000h. Incremented by
    hardware at the P0 frequency while the core is in C0. This register does not increment when the core is in the
            stop-grant state. In combination with Core::X86::Msr::APERF, this is used to determine the effective frequency
            of the core. A read of this MSR in guest mode is affected by Core::X86::Msr::TscRateMsr. This field uses
            software P-state numbering. See Core::X86::Msr::HWCR[EffFreqCntMwait], 2.1.2 [Effective Frequency])====="}}},
        {"MSR0000_00E8", "Actual Performance Frequency Clock Count", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000000E8},
         {{63, 0, R"=====(APERF: actual core clocks counter. Read-write,Volatile. Reset: 0000_0000_0000_0000h. This register
            increments in proportion to the actual number of core clocks cycles while the core is in C0. The register does not
    increment when the core is in the stop-grant state. See Core::X86::Msr::MPERF.)====="}}},
        {"MSR0000_00FE", "MTRR Capabilities", R"=====(Read,Error-on-write. Reset: 0000_0000_0000_0508h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0x000000FE},
         {{63, 11, R"=====(Reserved.)====="}, {10, 10, R"=====(MtrrCapWc: write-combining memory type. Read,Error-on-write. Reset: 1. 1=The write combining memory
    type is supported.)====="},
          {9, 9, R"=====(Reserved.)====="},
          {8, 8, R"=====(MtrrCapFix: fixed range register. Read,Error-on-write. Reset: 1. 1=Fixed MTRRs are supported.)====="},
          {7, 0, R"=====(MtrrCapVCnt: variable range registers count. Read,Error-on-write. Reset: 08h. Specifies the number of
    variable MTRRs supported.)====="}}},
        {"MSR0000_0174", "SYSENTER CS", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000174},
         {{63, 16, R"=====(Reserved.)====="}, {15, 0, R"=====(SysEnterCS: SYSENTER target CS. Read-write. Reset: 0000h. Holds the called procedure code segment.)====="}}},
        {"MSR0000_0175", "SYSENTER ESP", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000175},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(SysEnterESP: SYSENTER target SP. Read-write. Reset: 0000_0000h. Holds the called procedure stack
            pointer.)====="}}},
        {"MSR0000_0176", "SYSENTER EIP", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000176},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(SysEnterEIP: SYSENTER target IP. Read-write. Reset: 0000_0000h. Holds the called procedure instruction
            pointer.)====="}}},
        {"MSR0000_0179", "Global Machine Check Capabilities", R"=====(Reset: 0000_0000_0000_01XXh.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000179},
         {{63, 9, R"=====(Reserved.)====="}, {8, 8, R"=====(McgCtlP: MCG_CTL register present. Read-only,Error-on-write. Reset: Fixed,1. 1=The machine check control
    registers (MCi_CTL) are present. See 3 [Machine Check Architecture].)====="},
          {7, 0, R"=====(Count. Read-only,Error-on-write,Volatile. Reset: XXh. Indicates the number of error reporting banks visible to
            each core.)====="}}},
        {"MSR0000_017A", "Global Machine Check Status", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
    See 3 [Machine Check Architecture].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x0000017A},
         {{63, 3, R"=====(Reserved.)====="}, {2, 2, R"=====(MCIP: machine check in progress. Read-write,Volatile. Reset: 0. 1=A machine check is in progress. Machine
            check progress.)====="},
          {1, 1, R"=====(EIPV: error instruction pointer valid. Read-write,Volatile. Reset: 0. 1=The instruction pointer that was pushed
    onto the stack by the machine check mechanism references the instruction that caused the machine check error.)====="},
          {0, 0, R"=====(RIPV: restart instruction pointer valid. Read-write,Volatile. Reset: 0. 0=The interrupt was not precise and/or
    the process (task) context may be corrupt; continued operation of this process may not be possible without
            intervention, however system processing or other processes may be able to continue with appropriate software
            clean up. 1=Program execution can be reliably restarted at the EIP address on the stack.)====="}}},
        {"MSR0000_017B", "Global Machine Check Exception Reporting Control", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
    This register controls enablement of the individual error reporting banks; see 3 [Machine Check Architecture]. When a
    machine check register bank is not enabled in MCG_CTL, errors for that bank are not logged or reported, and actions
            enabled through the MCA are not taken; each MCi_CTL register identifies which errors are still corrected when
            MCG_CTL[i] is disabled.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x0000017B},
         {{63, 23, R"=====(Reserved.)====="}, {22, 0, R"=====(MCnEn. Read-write. Reset: 00_0000h. 1=The MC0 machine check register bank is enabled.)====="}}},
        {"MSR0000_01D9", "Debug Control", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000001D9},
         {{63, 7, R"=====(Reserved.)====="}, {6, 6, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0.)====="}, {5, 2, R"=====(PB: performance monitor pin control. Read-write. Reset: 0h. This field does not control any hardware.)====="}, {1, 1, R"=====(BTF. Read-write. Reset: 0. 1=Enable branch single step.)====="}, {0, 0, R"=====(LBR. Read-write. Reset: 0. 1=Enable last branch record.)====="}}},
        {"MSR0000_01DB", "Last Branch From IP", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000001DB},
         {{63, 0, R"=====(LastBranchFromIP. Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h. Loaded with the segment
    offset of the branch instruction.)====="}}},
        {"MSR0000_01DC", "Last Branch To IP", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000001DC},
         {{63, 0, R"=====(LastBranchToIP. Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h. Holds the target RIP of the last
            branch that occurred before an exception or interrupt.)====="}}},
        {"MSR0000_01DD", "Last Exception From IP", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000001DD},
         {{63, 0, R"=====(LastIntFromIP. Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h. Holds the source RIP of the last
            branch that occurred before the exception or interrupt.)====="}}},
        {"MSR0000_01DE", "Last Exception To IP", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x000001DE},
         {{63, 0, R"=====(LastIntToIP. Read,Error-on-write,Volatile. Reset: 0000_0000_0000_0000h. Holds the target RIP of the last
            branch that occurred before the exception or interrupt.)====="}}},
        {"MSR0000_020[0...E]", "Variable-Size MTRRs Base", R"=====(Reset: 0000_XXXX_XXXX_X00Xh.
    Each MTRR (Core::X86::Msr::MtrrVarBase, Core::X86::Msr::MtrrFix_64K through Core::X86::Msr::MtrrFix_4K_7, or
    Core::X86::Msr::MTRRdefType) specifies a physical address range and a corresponding memory type (MemType)
    associated with that range. Setting the memory type to an unsupported value results in a #GP.

    The variable-size MTRRs come in pairs of base and mask registers (MSR0000_0200 and MSR0000_0201 are the first
    pair, etc.). Variables MTRRs are enabled through Core::X86::Msr::MTRRdefType[MtrrDefTypeEn]. A core access--with
            address CPUAddr--is determined to be within the address range of a variable-size MTRR if the following equation is
    true:
    CPUAddr[47:12] & PhyMask[47:12] == PhyBase[47:12] & PhyMask[47:12].

    For example, if the variable MTRR spans 256 KB and starts at the 1 MB address the PhyBase would be set to
    0_0010_0000h and the PhyMask to F_FFFC_0000h (with zeros filling in for bits[11:0]). This results in a range from
    0_0010_0000h to 0_0013_FFFFh.)=====",
         "_lthree[1:0]_core[3:0]_n[7:0]; ",
         {0x0000E020, 0x0000C020, 0x0000A020, 0x00008020, 0x00006020, 0x00004020, 0x00002020, 0x00000020},
         {{63, 48, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000h.)====="}, {47, 12, R"=====(PhyBase: base address. Read-write. Reset: X_XXXX_XXXXh.)====="}, {11, 3, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,000h.)====="}, {2, 0, R"=====(MemType: memory type. Read-write. Reset: XXXb. Address range from 00000h to 0FFFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_020[1...F]", "Variable-Size MTRRs Mask", R"=====(Reset: 0000_XXXX_XXXX_XX00h.)=====",
         "_lthree[1:0]_core[3:0]_n[7:0]; ",
         {0x0000F020, 0x0000D020, 0x0000B020, 0x00009020, 0x00007020, 0x00005020, 0x00003020, 0x00001020},
         {{63, 48, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000h.)====="}, {47, 12, R"=====(PhyMask: address mask. Read-write. Reset: X_XXXX_XXXXh.)====="}, {11, 11, R"=====(Valid: valid. Read-write. Reset: X. 1=The variable-size MTRR pair is enabled.)====="}, {10, 0, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,000h.)====="}}},
        {"MSR0000_0250", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE64K; ",
         {0x00000250},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_64K_70000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_64K_70000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_64K_70000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_64K_60000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_64K_60000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_64K_60000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_64K_50000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_64K_50000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_64K_50000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_64K_40000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_64K_40000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_64K_40000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_64K_30000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_64K_30000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_64K_30000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_64K_20000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_64K_20000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_64K_20000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_64K_10000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_64K_10000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_64K_10000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_64K_00000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from 00000h to 0FFFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_64K_00000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from 00000h to 0FFFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_64K_00000: memory type. Read-write. Reset: XXXb. Address range from 00000h to 0FFFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_0258", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE16K0; ",
         {0x00000258},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_16K_9C000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_16K_9C000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_16K_9C000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_16K_98000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_16K_98000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_16K_98000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_16K_94000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_16K_94000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_16K_94000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_16K_90000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_16K_90000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_16K_90000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_16K_8C000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_16K_8C000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_16K_8C000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_16K_88000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_16K_88000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_16K_88000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_16K_84000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_16K_84000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_16K_84000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_16K_80000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from 80000h to 83FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_16K_80000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from 80000h to 83FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_16K_80000: memory type. Read-write. Reset: XXXb. Address range from 80000h to 83FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h         WB or write back.
    7h         Reserved.)====="}}},
        {"MSR0000_0259", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE16K1; ",
         {0x00000259},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_16K_BC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_16K_BC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_16K_BC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_16K_B8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_16K_B8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_16K_B8000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_16K_B4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_16K_B4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_16K_B4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_16K_B0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_16K_B0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_16K_B0000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_16K_AC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_16K_AC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_16K_AC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_16K_A8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_16K_A8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_16K_A8000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_16K_A4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_16K_A4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_16K_A4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_16K_A0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to
            the range are marked as destined for DRAM. Address range from A0000h to A3FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_16K_A0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from A0000h to A3FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_16K_A0000: memory type. Read-write. Reset: XXXb. Address range from A0000h to A3FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_0268", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K0; ",
         {0x00000268},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_C7000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_C7000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_C7000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_C6000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_C6000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_C6000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_C5000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_C5000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_C5000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_C4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_C4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_C4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_C3000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_C3000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_C3000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_C2000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_C2000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_C2000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_C1000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_C1000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_C1000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_C0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from C0000h to C0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_C0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from C0000h to C0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_C0000: memory type. Read-write. Reset: XXXb. Address range from C0000h to C0FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_0269", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K1; ",
         {0x00000269},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_CF000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_CF000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_CF000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_CE000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_CE000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_CE000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_CD000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_CD000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_CD000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_CC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_CC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_CC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_CB000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_CB000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_CB000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_CA000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_CA000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_CA000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_C9000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_C9000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_C9000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_C8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from C8000 to C8FFF.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_C8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from C8000 to C8FFF.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_C8000: memory type. Read-write. Reset: XXXb. Address range from C8000 to C8FFF.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_026A", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K2; ",
         {0x0000026A},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_D7000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_D7000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_D7000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_D6000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_D6000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_D6000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_D5000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_D5000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_D5000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_D4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_D4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_D4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_D3000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_D3000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_D3000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_D2000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_D2000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_D2000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_D1000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_D1000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_D1000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_D0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from D0000h to D0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_D0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from D0000h to D0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_D0000: memory type. Read-write. Reset: XXXb. Address range from D0000h to D0FFFh.
    ValidValues:
    Value      Description
    0h         UC or uncacheable.
    1h         WC or write combining.
    3h-2h      Reserved.
    4h         WT or write through.
    5h         WP or write protect.
    6h         WB or write back.
    7h         Reserved.)====="}}},
        {"MSR0000_026B", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K3; ",
         {0x0000026B},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_DF000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_DF000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_DF000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_DE000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_DE000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_DE000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_DD000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_DD000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_DD000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_DC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_DC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_DC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_DB000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_DB000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_DB000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_DA000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_DA000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_DA000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_D9000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_D9000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_D9000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_D8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from D8000h to D8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_D8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from D8000h to D8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_D8000: memory type. Read-write. Reset: XXXb. Address range from D8000h to D8FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_026C", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K4; ",
         {0x0000026C},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_E7000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_E7000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_E7000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_E6000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_E6000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_E6000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_E5000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_E5000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_E5000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_E4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_E4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_E4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_E3000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_E3000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_E3000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_E2000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_E2000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_E2000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_E1000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_E1000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_E1000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_E0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from E0000h to E0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_E0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from E0000h to E0FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_E0000: memory type. Read-write. Reset: XXXb. Address range from E0000h to E0FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_026D", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K5; ",
         {0x0000026D},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_EF000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_EF000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_EF000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_EE000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_EE000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_EE000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_ED000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_ED000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_ED000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_EC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_EC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_EC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_EB000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_EB000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_EB000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_EA000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_EA000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_EA000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_E9000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_E9000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_E9000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_E8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from E8000h to E8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_E8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from E8000h to E8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_E8000: memory type. Read-write. Reset: XXXb. Address range from E8000h to E8FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h         Reserved.)====="}}},
        {"MSR0000_026E", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K6; ",
         {0x0000026E},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_F7000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_F7000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_F7000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_F6000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_F6000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_F6000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_F5000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_F5000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_F5000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_F4000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_F4000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_F4000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_F3000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_F3000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_F3000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_F2000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_F2000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_F2000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_F1000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_F1000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_F1000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_F0000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from F0000h to F0FFF.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_F0000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from F0000h to F0FFF.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_F0000: memory type. Read-write. Reset: XXXb. Address range from F0000h to F0FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_026F", "Fixed-Size MTRRs", R"=====(See Core::X86::Msr::MtrrVarBase for general MTRR information. Fixed MTRRs are enabled through
            Core::X86::Msr::MTRRdefType[MtrrDefTypeFixEn,MtrrDefTypeEn]. For addresses below 1MB, the appropriate Fixed
            MTRRs override the default access destination. Each fixed MTRR includes two bits, RdDram and WrDram, that
            determine the destination based on the access type. Writing reserved MemType values causes an error-on-write.)=====",
         "_lthree[1:0]_core[3:0]_nSIZE4K7; ",
         {0x0000026F},
         {{63, 61, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="}, {60, 60, R"=====(RdDram_4K_FF000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {59, 59, R"=====(WrDram_4K_FF000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {58, 56, R"=====(MemType_4K_FF000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {55, 53, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {52, 52, R"=====(RdDram_4K_FE000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {51, 51, R"=====(WrDram_4K_FE000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {50, 48, R"=====(MemType_4K_FE000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {47, 45, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {44, 44, R"=====(RdDram_4K_FD000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {43, 43, R"=====(WrDram_4K_FD000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {42, 40, R"=====(MemType_4K_FD000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {39, 37, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {36, 36, R"=====(RdDram_4K_FC000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {35, 35, R"=====(WrDram_4K_FC000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {34, 32, R"=====(MemType_4K_FC000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {31, 29, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {28, 28, R"=====(RdDram_4K_FB000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {27, 27, R"=====(WrDram_4K_FB000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {26, 24, R"=====(MemType_4K_FB000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {23, 21, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {20, 20, R"=====(RdDram_4K_FA000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {19, 19, R"=====(WrDram_4K_FA000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {18, 16, R"=====(MemType_4K_FA000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {15, 13, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {12, 12, R"=====(RdDram_4K_F9000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {11, 11, R"=====(WrDram_4K_F9000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {10, 8, R"=====(MemType_4K_F9000: memory type. Read-write. Reset: XXXb.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="},
          {7, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {4, 4, R"=====(RdDram_4K_F8000: read DRAM. 0=Read accesses to the range are marked as MMIO. 1=Read accesses to the
    range are marked as destined for DRAM. Address range from F8000h to F8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {3, 3, R"=====(WrDram_4K_F8000: write DRAM. 0=Write accesses to the range are marked as MMIO. 1=Write accesses to
            the range are marked as destined for DRAM. Address range from F8000h to F8FFFh.
    Core::X86::Msr::SYS_CFG[MtrrFixDramEn,MtrrFixDramModEn] masks reads of the stored value.
    AccessType: Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? Read-write : Read,Error-on-write-1. Reset:
    Core::X86::Msr::SYS_CFG[MtrrFixDramModEn] ? X : Fixed,0.)====="},
          {2, 0, R"=====(MemType_4K_F8000: memory type. Read-write. Reset: XXXb. Address range from F8000h to F8FFFh.
    ValidValues:
    Value     Description
    0h        UC or uncacheable.
    1h        WC or write combining.
    3h-2h     Reserved.
    4h        WT or write through.
    5h        WP or write protect.
    6h        WB or write back.
    7h        Reserved.)====="}}},
        {"MSR0000_0277", "Page Attribute Table", R"=====(Reset: 0007_0406_0007_0406h.
    This register specifies the memory type based on the PAT, PCD, and PWT bits in the virtual address page tables.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0x00000277},
         {{63, 59, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="}, {58, 56, R"=====(PA7MemType. Read-write. Reset: 0h. Default UC. MemType for {PAT, PCD, PWT} = 7h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {55, 51, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {50, 48, R"=====(PA6MemType. Read-write. Reset: 7h. Default UC. MemType for {PAT, PCD, PWT} = 6h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {47, 43, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {42, 40, R"=====(PA5MemType. Read-write. Reset: 4h. Default WT. MemType for {PAT, PCD, PWT} = 5h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {39, 35, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {34, 32, R"=====(PA4MemType. Read-write. Reset: 6h. Default WB. MemType for {PAT, PCD, PWT} = 4h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {31, 27, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {26, 24, R"=====(PA3MemType. Read-write. Reset: 0h. Default UC. MemType for {PAT, PCD, PWT} = 3h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {23, 19, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {18, 16, R"=====(PA2MemType. Read-write. Reset: 7h. Default UC. MemType for {PAT, PCD, PWT} = 2h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {15, 11, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {10, 8, R"=====(PA1MemType. Read-write. Reset: 4h. Default WT. MemType for {PAT, PCD, PWT} = 1h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {7, 3, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00h.)====="},
          {2, 0, R"=====(PA0MemType. Read-write. Reset: 6h. MemType for {PAT, PCD, PWT} = 0h.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="}}},
        {"MSR0000_02FF", "MTRR Default Memory Type", R"=====(Reset: 0000_0000_0000_0000h.
See Core::X86::Msr::MtrrVarBase for general MTRR information.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0x000002FF},
         {{63, 12, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0_0000_0000_0000h.)====="}, {11, 11, R"=====(MtrrDefTypeEn: variable and fixed MTRR enable. Read-write. Reset: 0. 0=Fixed and variable MTRRs are not
enabled. 1=Core::X86::Msr::MtrrVarBase, and Core::X86::Msr::MtrrFix_64K through
Core::X86::Msr::MtrrFix_4K_7 are enabled.)====="},
          {10, 10, R"=====(MtrrDefTypeFixEn: fixed MTRR enable. Read-write. Reset: 0. 0=Core::X86::Msr::MtrrFix_64K through
Core::X86::Msr::MtrrFix_4K_7 are not enabled. 1=Core::X86::Msr::MtrrFix_64K through
Core::X86::Msr::MtrrFix_4K_7 are enabled. This field is ignored (and the fixed MTRRs are not enabled) if
Core::X86::Msr::MTRRdefType[MtrrDefTypeEn] == 0.)====="},
          {9, 8, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0h.)====="},
          {7, 0, R"=====(MemType: memory type. Read-write. Reset: 00h.
Description: If MtrrDefTypeEn == 1 then MemType specifies the memory type for memory space that is not
specified by either the fixed or variable range MTRRs. If MtrrDefTypeEn == 0 then the default memory type for
all of memory is UC.
Valid encodings are {00000b, Core::X86::Msr::MtrrFix_64K through Core::X86::Msr::MtrrFix_4K_7[2:0]}.
Other write values cause a GP(0).


2.1.10.2       MSRs - MSRC000_0xxx

        See 1.3.3 [Register Mnemonics] for a description of the register naming convention. MSRs are accessed through x86
        WRMSR and RDMSR instructions.)====="}}},
        {"MSRC000_0080", "Extended Feature Enable", R"=====(Reset: 0000_0000_0000_0000h.
SKINIT Execution: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000080},
         {{63, 16, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0000_0000_0000h.)====="}, {15, 15, R"=====(Reserved.)====="}, {14, 14, R"=====(FFXSE: fast FXSAVE/FRSTOR enable. Read-write. Reset: 0. 1=Enables the fast FXSAVE/FRSTOR
        mechanism. A 64-bit operating system may enable the fast FXSAVE/FRSTOR mechanism if
(Core::X86::Cpuid::FeatureExtIdEdx[FFXSR] == 1). This bit is set once by the operating system and its value is
not changed afterwards.)====="},
          {13, 13, R"=====(LMSLE: long mode segment limit enable. Read-write. Reset: 0. 1=Enables the long mode segment limit check
        mechanism.)====="},
          {12, 12, R"=====(SVME: secure virtual machine (SVM) enable. Reset: Fixed,0. 1=SVM features are enabled.
AccessType: Core::X86::Msr::VM_CR[SvmeDisable] ? Read-only,Error-on-write-1 : Read-write.)====="},
          {11, 11, R"=====(NXE: no-execute page enable. Read-write. Reset: 0. 1=The no-execute page protection feature is enabled.)====="},
          {10, 10, R"=====(LMA: long mode active. Read-only. Reset: 0. 1=Indicates that long mode is active.)====="},
          {9, 9, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,0.)====="},
          {8, 8, R"=====(LME: long mode enable. Read-write. Reset: 0. 1=Long mode is enabled.)====="},
          {7, 1, R"=====(Reserved. Read-only. Reset: Fixed,00h.)====="},
          {0, 0, R"=====(SYSCALL: system call extension enable. Read-write. Reset: 0. 1=SYSCALL and SYSRET instructions are
enabled. This adds the SYSCALL and SYSRET instructions which can be used in flat addressed operating
systems as low latency system calls and returns.)====="}}},
        {"MSRC000_0081", "SYSCALL Target Address", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
This register holds the target address used by the SYSCALL instruction and the code and stack segment selector bases
used by the SYSCALL and SYSRET instructions.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000081},
         {{63, 48, R"=====(SysRetSel: SYSRET CS and SS. Read-write. Reset: 0000h.)====="}, {47, 32, R"=====(SysCallSel: SYSCALL CS and SS. Read-write. Reset: 0000h.)====="}, {31, 0, R"=====(Target: SYSCALL target address. Read-write. Reset: 0000_0000h.)====="}}},
        {"MSRC000_0082", "Long Mode SYSCALL Target Address", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000082},
         {{63, 0, R"=====(LSTAR: long mode target address. Read-write. Reset: 0000_0000_0000_0000h. Target address for 64-bit mode
calling programs. The address stored in this register must be in canonical form (if not canonical, a #GP fault
occurs).)====="}}},
        {"MSRC000_0083", "Compatibility Mode SYSCALL Target Address", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000083},
         {{63, 0, R"=====(CSTAR: compatibility mode target address. Read-write. Reset: 0000_0000_0000_0000h. Target address for
compatibility mode. The address stored in this register must be in canonical form (if not canonical, a #GP fault
occurs).)====="}}},
        {"MSRC000_0084", "SYSCALL Flag Mask", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000084},
         {{63, 32, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}, {31, 0, R"=====(Mask: SYSCALL flag mask. Read-write. Reset: 0000_0000h. This register holds the EFLAGS mask used by the
SYSCALL instruction. 1=Clear the corresponding EFLAGS bit when executing the SYSCALL instruction.)====="}}},
        {"MSRC000_00E7", "Read-Only Max Performance Frequency Clock Count", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC00000E7},
         {{63, 0, R"=====(MPerfReadOnly: read-only maximum core clocks counter. Reset: 0000_0000_0000_0000h. Incremented by
hardware at the P0 frequency while the core is in C0. In combination with Core::X86::Msr::APerfReadOnly, this
is used to determine the effective frequency of the core. A read of this MSR in guest mode is affected by
        Core::X86::Msr::TscRateMsr. This field uses software P-state numbering. See
        Core::X86::Msr::HWCR[EffFreqCntMwait], 2.1.2 [Effective Frequency]. This register is not affected by writes to
Core::X86::Msr::MPERF.
AccessType: Core::X86::Msr::HWCR[EffFreqReadOnlyLock] ? Read-only,Volatile : Read-write,Volatile.)====="}}},
        {"MSRC000_00E8", "Read-Only Actual Performance Frequency Clock Count", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC00000E8},
         {{63, 0, R"=====(APerfReadOnly: read-only actual core clocks counter. Reset: 0000_0000_0000_0000h. This register
        increments in proportion to the actual number of core clocks cycles while the core is in C0. See
        Core::X86::Msr::MPerfReadOnly. This register is not affected by writes to Core::X86::Msr::APERF.
AccessType: Core::X86::Msr::HWCR[EffFreqReadOnlyLock] ? Read-only,Volatile : Read-write,Volatile.)====="}}},
        {"MSRC000_00E9", "Instructions Retired Performance Count", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC00000E9},
         {{63, 0, R"=====(IRPerfCount: instructions retired counter. Reset: 0000_0000_0000_0000h. Dedicated Instructions Retired
register increments on once for every instruction retired. See Core::X86::Msr::HWCR[IRPerfEn].
AccessType: Core::X86::Msr::HWCR[EffFreqReadOnlyLock] ? Read-only,Volatile : Read-write,Volatile.)====="}}},
        {"MSRC000_0100", "FS Base", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000100},
         {{63, 0, R"=====(FSBase: expanded FS segment base. Read-write. Reset: 0000_0000_0000_0000h. This register provides access
        to the expanded 64-bit FS segment base. The address stored in this register must be in canonical form (if not
canonical, a #GP fault fill occurs).)====="}}},
        {"MSRC000_0101", "GS Base", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000101},
         {{63, 0, R"=====(GSBase: expanded GS segment base. Read-write. Reset: 0000_0000_0000_0000h. This register provides access
        to the expanded 64-bit GS segment base. The address stored in this register must be in canonical form (if not
canonical, a #GP fault fill occurs).)====="}}},
        {"MSRC000_0102", "Kernel GS Base", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000102},
         {{63, 0, R"=====(KernelGSBase: kernel data structure pointer. Read-write. Reset: 0000_0000_0000_0000h. This register holds
the kernel data structure pointer which can be swapped with the GS_BASE register using the SwapGS instruction.
The address stored in this register must be in canonical form (if not canonical, a #GP fault occurs).)====="}}},
        {"MSRC000_0103", "Auxiliary Time Stamp Counter", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000103},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(TscAux: auxiliary time stamp counter data. Read-write,Volatile. Reset: 0000_0000h. It is expected that this is
        initialized by privileged software to a meaningful value, such as a processor ID. This value is returned in the
RDTSCP instruction.)====="}}},
        {"MSRC000_0104", "Time Stamp Counter Ratio", R"=====(Reset: 0000_0001_0000_0000h.
Core::X86::Msr::TscRateMsr allows the hypervisor to control the guest's view of the Time Stamp Counter. It provides a
multiplier that scales the value returned when Core::X86::Msr::TSC[TSC], Core::X86::Msr::MPERF[MPERF], and
        Core::X86::Msr::MPerfReadOnly[MPerfReadOnly] are read by a guest running under virtualization. This allows the
        hypervisor to provide a consistent TSC, MPERF, and MPerfReadOnly rate for a guest process when moving that process
        between cores that have a differing P0 rate. The TSC Ratio MSR does not affect the value read from the TSC, MPERF,
and MPerfReadOnly MSRs when read when in host mode or when virtualization is not being used or when accessed by
        code executed in system management mode (SMM) unless the SMM code is executed within a guest container. The TSC
Ratio value does not affect the rate of the underlying TSC, MPERF, and MPerfReadOnly counters, or the value that gets
written to the TSC, MPERF, and MPerfReadOnly MSRs counters on a write by either the host or the guest. The TSC
Ratio MSR contains a fixed-point number in 8.32 format, which is 8 bits of integer and 32 bits of fraction. This number
is the ratio of the desired P0 frequency to the P0 frequency of the core. The reset value of the TSC Ratio MSR is 1.0,
which results in a guest frequency matches the core P0 frequency.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0000104},
         {{63, 40, R"=====(Reserved. Read-only,Error-on-write-1. Reset: Fixed,00_0000h.)====="}, {39, 32, R"=====(TscRateMsrInt: time stamp counter rate integer. Read-write. Reset: 01h. Specifies the integer part of the MSR
        TSC ratio value.)====="},
          {31, 0, R"=====(TscRateMsrFrac: time stamp counter rate fraction. Read-write. Reset: 0000_0000h. Specifies the fractional
        part of the MSR TSC ratio value.)====="}}},
        {"MSRC000_0410",
         "MCA Interrupt Configuration",
         R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "",
         {0xC0000410},
         {{63, 16, R"=====(Reserved.)====="},
          {15, 12, R"=====(ThresholdLvtOffset. Read-write. Reset: 0h. For error thresholding interrupts, specifies the address of the LVT entry in the APIC registers as follows: LVT address = (LvtOffset shifted left 4 bits) + 500h (see Core::X86::Apic::ExtendedInterruptLvtEntries).)====="},
          {11, 8, R"=====(Reserved.)====="},
          {7, 4, R"=====(DeferredLvtOffset. Read-write. Reset: 0h.
Description: For deferred error interrupts, specifies the address of the LVT entry in the APIC registers as follows: LVT address = (LvtOffset shifted left 4 bits) + 500h (see APIC[530:500]).)====="},
          {3, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_000[0...3]", "Performance Event Select [3:0]", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
The legacy alias of Core::X86::Msr::PERF_CTL. See Core::X86::Msr::PERF_CTL.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[3:0]; ",
         {0xC0013000, 0xC0012000, 0xC0011000, 0xC0010000},
         {{63, 42, R"=====(Reserved.)====="}, {41, 40, R"=====(HostGuestOnly: count only host/guest events. Read-write. Reset: 0h.)====="}, {39, 36, R"=====(Reserved.)====="}, {35, 32, R"=====(EventSelect[11:8]: performance event select. Read-write. Reset: 0h.)====="}, {31, 24, R"=====(CntMask: counter mask. Read-write. Reset: 00h.)====="}, {23, 23, R"=====(Inv: invert counter mask. Read-write. Reset: 0.)====="}, {22, 22, R"=====(En: enable performance counter. Read-write. Reset: 0.)====="}, {21, 21, R"=====(Reserved.)====="}, {20, 20, R"=====(Int: enable APIC interrupt. Read-write. Reset: 0.)====="}, {19, 19, R"=====(Reserved.)====="}, {18, 18, R"=====(Edge: edge detect. Read-write. Reset: 0.)====="}, {17, 16, R"=====(OsUserMode: OS and user mode. Read-write. Reset: 0h.)====="}, {15, 8, R"=====(UnitMask: event qualification. Read-write. Reset: 00h. When selecting an event for which not all UnitMask bits
        are defined, the undefined UnitMask bits should be set to zero.)====="},
          {7, 0, R"=====(EventSelect[7:0]: event select. Read-write. Reset: 00h.)====="}}},
        {"MSRC001_000[4...7]", "Performance Event Counter [3:0]", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
The legacy alias of Core::X86::Msr::PERF_CTR. See Core::X86::Msr::PERF_CTR.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[3:0]; ",
         {0xC0017000, 0xC0016000, 0xC0015000, 0xC0014000},
         {{63, 48, R"=====(Reserved.)====="}, {47, 0, R"=====(CTR: performance counter value. Read-write,Volatile. Reset: 0000_0000_0000h.)====="}}},
        {"MSRC001_0010", "System Configuration", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010010},
         {{63, 24, R"=====(Reserved.)====="}, {23, 23, R"=====(SMEE: secure memory encryption enable. Reset: 0. Init: BIOS,1. 0=Memory encryption features are disabled.
1=Memory encryption features are enabled. Once Core::X86::Msr::HWCR[SmmLock] is set, this bit cannot be
        cleared until a reset.
AccessType: Core::X86::Msr::HWCR_thread0[SmmLock] ? Read,Write-1-only : Read-write.)====="},
          {22, 22, R"=====(Tom2ForceMemTypeWB: top of memory 2 memory type write back. Read-write. Reset: 0. 1=The default
memory type of memory between 4GB and TOM2 is write back instead of the memory type defined by
        Core::X86::Msr::MTRRdefType[MemType]. For this bit to have any effect,
Core::X86::Msr::MTRRdefType[MtrrDefTypeEn] must be 1. MTRRs and PAT can be used to override this
memory type.)====="},
          {21, 21, R"=====(MtrrTom2En: MTRR top of memory 2 enable. Read-write. Reset: 0. 0=Core::X86::Msr::TOM2 is disabled. 1=
Core::X86::Msr::TOM2 is enabled.)====="},
          {20, 20, R"=====(MtrrVarDramEn: MTRR variable DRAM enable. Read-write. Reset: 0. Init: BIOS,1.
0=Core::X86::Msr::TOP_MEM and IORRs are disabled. 1=These registers are enabled.)====="},
          {19, 19, R"=====(MtrrFixDramModEn: MTRR fixed RdDram and WrDram modification enable. Read-write. Reset: 0.
Check: 0. 0=Core::X86::Msr::MtrrFix_64K through Core::X86::Msr::MtrrFix_4K_7 [RdDram,WrDram] read
        values is masked 00b; writing does not change the hidden value. 1=Core::X86::Msr::MtrrFix_64K through
Core::X86::Msr::MtrrFix_4K_7 [RdDram,WrDram] access type is Read-write. Not shared between threads.
Controls access to Core::X86::Msr::MtrrFix_64K through Core::X86::Msr::MtrrFix_4K_7 [RdDram ,WrDram].
This bit should be set to 1 during BIOS initialization of the fixed MTRRs, then cleared to 0 for operation.)====="},
          {18, 18, R"=====(MtrrFixDramEn: MTRR fixed RdDram and WrDram attributes enable. Read-write. Reset: 0. Init: BIOS,1.
1=Enables the RdDram and WrDram attributes in Core::X86::Msr::MtrrFix_64K through
Core::X86::Msr::MtrrFix_4K_7.)====="},
          {17, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_0015", "Hardware Configuration", R"=====(Reset: 0000_0000_0100_0010h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010015},
         {{63, 31, R"=====(Reserved.)====="}, {30, 30, R"=====(IRPerfEn: enable instructions retired counter. Read-write. Reset: 0. 1=Enable Core::X86::Msr::IRPerfCount.)====="}, {29, 28, R"=====(Reserved.)====="}, {27, 27, R"=====(EffFreqReadOnlyLock: read-only effective frequency counter lock. Write-1-only. Reset: 0. Init: BIOS,1.
1=Core::X86::Msr::MPerfReadOnly, Core::X86::Msr::APerfReadOnly and Core::X86::Msr::IRPerfCount are
        read-only.)====="},
          {26, 26, R"=====(EffFreqCntMwait: effective frequency counting during mwait. Read-write. Reset: 0. 0=The registers do not
increment. 1=The registers increment. Specifies whether Core::X86::Msr::MPERF and Core::X86::Msr::APERF
increment while the core is in the monitor event pending state. See 2.1.2 [Effective Frequency].)====="},
          {25, 25, R"=====(CpbDis: core performance boost disable. Read-write. Reset: 0. 0=CPB is requested to be enabled. 1=CPB is
disabled. Specifies whether core performance boost is requested to be enabled or disabled. If core performance
        boost is disabled while a core is in a boosted P-state, the core automatically transitions to the highest performance
non-boosted P-state.)====="},
          {24, 24, R"=====(TscFreqSel: TSC frequency select. Read-only. Reset: 1. 1=The TSC increments at the P0 frequency.)====="},
          {23, 22, R"=====(Reserved.)====="},
          {21, 21, R"=====(LockTscToCurrentP0: lock the TSC to the current P0 frequency. Read-write. Reset: 0. 0=The TSC will count
at the P0 frequency. 1=The TSC frequency is locked to the current P0 frequency at the time this bit is set and
        remains fixed regardless of future changes to the P0 frequency.)====="},
          {20, 20, R"=====(IoCfgGpFault: IO-space configuration causes a GP fault. Read-write. Reset: 0. 1=IO-space accesses to
        configuration space cause a GP fault. The fault is triggered if any part of the IO read/write address range is
between CF8h and CFFh, inclusive. These faults only result from single IO instructions, not to string and REP IO
instructions. This fault takes priority over the IO trap mechanism described by
        Core::X86::Msr::SMI_ON_IO_TRAP_CTL_STS.)====="},
          {19, 19, R"=====(Reserved.)====="},
          {18, 18, R"=====(McStatusWrEn: machine check status write enable. Read-write. Reset: 0. 0=MCi_STATUS registers are
        readable; writing a non-zero pattern to these registers causes a general protection fault. 1=MCi_STATUS registers
are read-write, including reserved fields; do not cause general protection faults; such writes update all
implemented bits in these registers; All fields of all threshold registers are Read-write when accessed from MSR
        space, including Locked, except BlkPtr which is always read-only; McStatusWrEn does not change the access
        type for the thresholding registers accessed via configuration space.
Description: McStatusWrEn can be used to debug machine check exception and interrupt handlers.
See 3 [Machine Check Architecture].)====="},
          {17, 17, R"=====(Wrap32Dis: 32-bit address wrap disable. Read-write. Reset: 0. 1=Disable 32-bit address wrapping. Software
        can use Wrap32Dis to access physical memory above 4 Gbytes without switching into 64-bit mode. To do so,
software should write a greater-than 4 Gbyte address to Core::X86::Msr::FS_BASE and
Core::X86::Msr::GS_BASE. Then it would address ±2 Gbytes from one of those bases using normal memory
        reference instructions with a FS or GS override prefix. However, the INVLPG, FST, and SSE store instructions
generate 32-bit addresses in legacy mode, regardless of the state of Wrap32Dis.)====="},
          {16, 15, R"=====(Reserved.)====="},
          {14, 14, R"=====(RsmSpCycDis: RSM special bus cycle disable. Reset: 0. 0=A link special bus cycle, SMIACK, is generated on
        a resume from SMI.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Read-only : Read-write.)====="},
          {13, 13, R"=====(SmiSpCycDis: SMI special bus cycle disable. Reset: 0. 0=A link special bus cycle, SMIACK, is generated when
        an SMI interrupt is taken.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Read-only : Read-write.)====="},
          {12, 11, R"=====(Reserved.)====="},
          {10, 10, R"=====(MonMwaitUserEn: MONITOR/MWAIT user mode enable. Read-write. Reset: 0. 0=The MONITOR and
MWAIT instructions are supported only in privilege level 0; these instructions in privilege levels 1 to 3 cause a
#UD exception. 1=The MONITOR and MWAIT instructions are supported in all privilege levels. The state of this
bit is ignored if MonMwaitDis is set.)====="},
          {9, 9, R"=====(MonMwaitDis: MONITOR and MWAIT disable. Read-write. Reset: 0. 1=The MONITOR and MWAIT
        opcodes become invalid. This affects what is reported back through Core::X86::Cpuid::FeatureIdEcx[Monitor].)====="},
          {8, 8, R"=====(IgnneEm: IGNNE port emulation enable. Read-write. Reset: 0. 1=Enable emulation of IGNNE port.)====="},
          {7, 7, R"=====(AllowFerrOnNe: allow FERR on NE. Read-write. Reset: 0. 0=Disable legacy FERR signaling and generate
        FERR exception directly. 1=Legacy FERR signaling.)====="},
          {6, 5, R"=====(Reserved.)====="},
          {4, 4, R"=====(INVDWBINVD: INVD to WBINVD conversion. Read-write. Reset: 1. Check: 1. 1=Convert INVD to
        WBINVD.
Description: This bit is required to be set for normal operation when any of the following are true:
• An L2 is shared by multiple threads.
• An L3 is shared by multiple cores.
• CC6 is enabled.
• Probe filter is enabled.)====="},
          {3, 3, R"=====(TlbCacheDis: cacheable memory disable. Read-write. Reset: 0. 1=Disable performance improvement that
assumes that the PML4, PDP, PDE and PTE entries are in cacheable WB DRAM.
Description: Operating systems that maintain page tables in any other memory type must set the TlbCacheDis bit
to insure proper operation.
• TlbCacheDis does not override the memory type specified by the SMM ASeg and TSeg memory regions
controlled by Core::X86::Msr::SMMAddr Core::X86::Msr::SMMMask.)====="},
          {2, 1, R"=====(Reserved.)====="},
          {0, 0, R"=====(SmmLock: SMM code lock. Read,Write-1-only. Reset: 0. Init: BIOS,1. Check: 1. 1=SMM code in the ASeg and
        TSeg range and the SMM registers are read-only and SMI interrupts are not intercepted in SVM. See 2.1.8.1.10
[Locking SMM].)====="}}},
        {"MSRC001_001[6...8]", "IO Range Base", R"=====(Read-write. Reset: 0000_XXXX_XXXX_X0XXh.
Core::X86::Msr::IORR_BASE and Core::X86::Msr::IORR_MASK combine to specify the two sets of base and mask
        pairs for two IORR ranges. A core access, with address CPUAddr, is determined to be within IORR address range if the
        following equation is true:
CPUAddr[47:12] & PhyMask[47:12] == PhyBase[47:12] & PhyMask[47:12].
BIOS can use the IORRs to create an IO hole within a range of addresses that would normally be mapped to DRAM. It
        can also use the IORRs to re-assert a DRAM destination for a range of addresses that fall within a bigger IO hole that
overlays DRAM.)=====",
         "_lthree[1:0]_core[3:0]_n[1:0]; ",
         {0xC0018001, 0xC0017001, 0xC0016001},
         {{63, 48, R"=====(Reserved.)====="}, {47, 12, R"=====(PhyBase: physical base address. Read-write. Reset: X_XXXX_XXXXh.)====="}, {11, 5, R"=====(Reserved.)====="}, {4, 4, R"=====(RdMem: read from memory. Read-write. Reset: X. 0=Read accesses to the range are directed to IO. 1=Read
        accesses to the range are directed to system memory.)====="},
          {3, 3, R"=====(WrMem: write to memory. Read-write. Reset: X. 0=Write accesses to the range are directed to IO. 1=Write
        accesses to the range are directed to system memory.)====="},
          {2, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_001[7...9]", "IO Range Mask", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
See Core::X86::Msr::IORR_BASE.)=====",
         "_lthree[1:0]_core[3:0]_n[1:0]; ",
         {0xC0019001, 0xC0018001, 0xC0017001},
         {{63, 48, R"=====(Reserved.)====="}, {47, 12, R"=====(PhyMask: physical address mask. Read-write. Reset: 0_0000_0000h.)====="}, {11, 11, R"=====(Valid. Read-write. Reset: 0. 1=The pair of registers that specifies an IORR range is valid.)====="}, {10, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_001A", "Top Of Memory", R"=====(Read-write. Reset: 0000_XXXX_XXX0_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC001001A},
         {{63, 48, R"=====(Reserved.)====="}, {47, 23, R"=====(TOM[47:23]: top of memory. Read-write. Reset: XXX_XXXXh. Specifies the address that divides between
MMIO and DRAM. This value is normally placed below 4G. From TOM to 4G is MMIO; below TOM is DRAM.
See 2.1.3.3 [System Address Map].)====="},
          {22, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_001D", "Top Of Memory 2", R"=====(Read-write. Reset: 0000_XXXX_XXX0_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC001001D},
         {{63, 48, R"=====(Reserved.)====="}, {47, 23, R"=====(TOM2[47:23]: second top of memory. Read-write. Reset: XXX_XXXXh. Specifies the address divides between
        MMIO and DRAM. This value is normally placed above 4G. From 4G to TOM2 - 1 is DRAM; TOM2 and above
is MMIO. See 2.1.3.3 [System Address Map]. This register is enabled by
Core::X86::Msr::SYS_CFG[MtrrTom2En].)====="},
          {22, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_0022", "Machine Check Exception Redirection", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
This register can be used to redirect machine check exceptions (MCEs) to SMIs or vectored interrupts. If both
RedirSmiEn and RedirVecEn are set, then undefined behavior results.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010022},
         {{63, 10, R"=====(Reserved.)====="}, {9, 9, R"=====(RedirSmiEn. Read-write. Reset: 0. 1=Redirect MCEs (that are directed to this core) to generate an SMI-trigger
        IO cycle via Core::X86::Msr::SmiTrigIoCycle. The status is stored in
        Core::X86::Smm::LocalSmiStatus[MceRedirSts].)====="},
          {8, 8, R"=====(RedirVecEn. Read-write. Reset: 0. 1=Redirect MCEs (that are directed to this core) to generate a vectored
interrupt, using the interrupt vector specified in RedirVector.)====="},
          {7, 0, R"=====(RedirVector. Read-write. Reset: 00h. See RedirVecEn.)====="}}},
        {"MSRC001_003[0...5]", "Processor Name String", R"=====(Read-write. Reset: XXXX_XXXX_XXXX_XXXXh.
These 6 registers hold the CPUID name string in ASCII. The state of these registers are returned by CPUID instructions,
        Core::X86::Cpuid::ProcNameStr0Eax through Core::X86::Cpuid::ProcNameStr2Edx. BIOS should set these registers to
the product name for the processor as provided by AMD. Each register contains a block of 8 ASCII characters; the least
byte corresponds to the first ASCII character of the block; the most-significant byte corresponds to the last character of
the block. MSRC001_0030 contains the first block of the name string; MSRC001_0035 contains the last block of the
        name string.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[5:0]; ",
         {0xC0015003, 0xC0014003, 0xC0013003, 0xC0012003, 0xC0011003, 0xC0010003},
         {{63, 56, R"=====(CpuNameString7. Read-write. Reset: XXh.)====="}, {55, 48, R"=====(CpuNameString6. Read-write. Reset: XXh.)====="}, {47, 40, R"=====(CpuNameString5. Read-write. Reset: XXh.)====="}, {39, 32, R"=====(CpuNameString4. Read-write. Reset: XXh.)====="}, {31, 24, R"=====(CpuNameString3. Read-write. Reset: XXh.)====="}, {23, 16, R"=====(CpuNameString2. Read-write. Reset: XXh.)====="}, {15, 8, R"=====(CpuNameString1. Read-write. Reset: XXh.)====="}, {7, 0, R"=====(CpuNameString0. Read-write. Reset: XXh.)====="}}},
        {"MSRC001_005[0...3]", "IO Trap", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
Core::X86::Msr::SMI_ON_IO_TRAP and Core::X86::Msr::SMI_ON_IO_TRAP_CTL_STS provide a mechanism for
executing the SMI handler if a an access to one of the specified addresses is detected. Access address and access type
checking is performed before IO instruction execution. If the access address and access type match one of the specified
        IO address and access types, then: (1) the IO instruction is not executed; (2) any breakpoint, other than the single-step
        breakpoint, set on the IO instruction is not taken (the single-step breakpoint is taken after resuming from SMM); and (3)
issue the SMI-trigger IO cycle specified by Core::X86::Msr::SmiTrigIoCycle if enabled. The status is stored in
        Core::X86::Smm::LocalSmiStatus[IoTrapSts].
IO-space configuration accesses are special IO accesses. An IO access is defined as an IO-space configuration access
        when IO instruction address bits[31:0] are CFCh, CFDh, CFEh, or CFFh when IO-space configuration is enabled
        (IO::IoCfgAddr[ConfigEn]). The access address for a configuration space access is the current value of
        IO::IoCfgAddr[BusNo,Device,Function,RegNo]. The access address for an IO access that is not a configuration access is
equivalent to the IO instruction address, bits[31:0].
The access address is compared with SmiAddr, and the instruction access type is compared with the enabled access types
        defined by ConfigSMI, SmiOnRdEn, and SmiOnWrEn. Access address bits[23:0] can be masked with SmiMask. IO and
        configuration space trapping to SMI applies only to single IO instructions; it does not apply to string and REP IO
        instructions. The conditional GP fault described by Core::X86::Msr::HWCR[IoCfgGpFault] takes priority over this trap.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[3:0]; ",
         {0xC0013005, 0xC0012005, 0xC0011005, 0xC0010005},
         {{63, 63, R"=====(SmiOnRdEn: enable SMI on IO read. Read-write. Reset: 0. 1=Enables SMI generation on a read access.)====="}, {62, 62, R"=====(SmiOnWrEn: enable SMI on IO write. Read-write. Reset: 0. 1=Enables SMI generation on a write access.)====="}, {61, 61, R"=====(ConfigSmi: configuration space SMI. Read-write. Reset: 0. 0=IO access (that is not an IO-space configuration
access). 1=Configuration access.)====="},
          {60, 56, R"=====(Reserved.)====="},
          {55, 32, R"=====(SmiMask[23:0]. Read-write. Reset: 00_0000h. 1=Do not mask address bit. 0=Mask address bit. SMI IO trap
        mask.)====="},
          {31, 0, R"=====(SmiAddr[31:0]. Read-write. Reset: 0000_0000h. SMI IO trap address.)====="}}},
        {"MSRC001_0054", "IO Trap Control", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010054},
         {{63, 32, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}, {31, 16, R"=====(Reserved.)====="}, {15, 15, R"=====(IoTrapEn: IO trap enable. Read-write. Reset: 0. 1=Enable IO and configuration space trapping specified by
        Core::X86::Msr::SMI_ON_IO_TRAP and Core::X86::Msr::SMI_ON_IO_TRAP_CTL_STS.)====="},
          {14, 8, R"=====(Reserved.)====="},
          {7, 7, R"=====(SmiEn3. Read-write. Reset: 0. 1=The trap Core::X86::Msr::SMI_ON_IO_TRAP_n[3] is enabled.)====="},
          {6, 6, R"=====(Reserved.)====="},
          {5, 5, R"=====(SmiEn2. Read-write. Reset: 0. 1=The trap Core::X86::Msr::SMI_ON_IO_TRAP_n[2] is enabled.)====="},
          {4, 4, R"=====(Reserved.)====="},
          {3, 3, R"=====(SmiEn1. Read-write. Reset: 0. 1=The trap Core::X86::Msr::SMI_ON_IO_TRAP_n[1] is enabled.)====="},
          {2, 2, R"=====(Reserved.)====="},
          {1, 1, R"=====(SmiEn0. Read-write. Reset: 0. 1=The trap Core::X86::Msr::SMI_ON_IO_TRAP_n[0] is enabled.)====="},
          {0, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_0055", "Reserved.", R"=====(Read-only. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010055},
         {{63, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000_0000_0000h.)====="}}},
        {"MSRC001_0056", "SMI Trigger IO Cycle", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
See 2.1.8.1.3 [SMI Sources And Delivery]. This register specifies an IO cycle that may be generated when a local SMI
        trigger event occurs. If IoCycleEn is set and there is a local SMI trigger event, then the IO cycle generated is a byte read
or write, based on IoRd, to address IoPortAddress. If the cycle is a write, then IoData contains the data written. If the
cycle is a read, the value read is discarded. If IoCycleEn is clear and a local SMI trigger event occurs, then undefined
        behavior results.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010056},
         {{63, 27, R"=====(Reserved.)====="}, {26, 26, R"=====(IoRd: IO Read. Read-write. Reset: 0. 0=IO write. 1=IO read.)====="}, {25, 25, R"=====(IoCycleEn: IO cycle enable. Read-write. Reset: 0. 1=The SMI trigger IO cycle is enabled to be generated.)====="}, {24, 24, R"=====(Reserved.)====="}, {23, 16, R"=====(IoData. Read-write. Reset: 00h.)====="}, {15, 0, R"=====(IoPortAddress. Read-write. Reset: 0000h.)====="}}},
        {"MSRC001_0058", "MMIO Configuration Base Address", R"=====(Reset: 0000_XXXX_XXX0_0000h.
See 2.1.4 [Configuration Space] for a description of MMIO configuration space.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010058},
         {{63, 48, R"=====(Reserved. Read-only. Reset: Fixed,0000h.)====="}, {47, 20, R"=====(MmioCfgBaseAddr[47:20]: MMIO configuration base address bits[47:20]. Read-write. Reset:
XXX_XXXXh. Specifies the base address of the MMIO configuration range.)====="},
          {19, 6, R"=====(Reserved. Read-only. Reset: Fixed,0000h.)====="},
          {5, 2, R"=====(BusRange: bus range identifier. Read-write. Reset: 0h. Specifies the number of buses in the MMIO
configuration space range. The size of the MMIO configuration space is 1 MB times the number of buses.
ValidValues:
Value    Description
0h       1
1h       2
2h       4
3h       8
4h       16
5h       32
6h       64
7h       128
8h       256
Fh-9h    Reserve.)====="},
          {1, 1, R"=====(Reserved.)====="},
          {0, 0, R"=====(Enable. Read-write. Reset: 0. 1=MMIO configuration space is enabled.)====="}}},
        {"MSRC001_0061", "P-state Current Limit", R"=====(Reset: 0000_0000_0000_00XXh.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010061},
         {{63, 7, R"=====(Reserved.)====="}, {6, 4, R"=====(PstateMaxVal: P-state maximum value. Read,Error-on-write,Volatile. Reset: XXXb. Specifies the lowest-
performance non-boosted P-state (highest non-boosted value) allowed. Attempts to change
        Core::X86::Msr::PStateCtl[PstateCmd] to a lower-performance P-state (higher value) are clipped to the value of
this field.)====="},
          {3, 3, R"=====(Reserved. Read-only. Reset: Fixed,0.)====="},
          {2, 0, R"=====(CurPstateLimit: current P-state limit. Read,Error-on-write,Volatile. Reset: XXXb. Specifies the highest-
performance P-state (lowest value) allowed. CurPstateLimit is always bounded by
        Core::X86::Msr::PStateCurLim[PstateMaxVal]. Attempts to change the CurPstateLimit to a value greater (lower
performance) than Core::X86::Msr::PStateCurLim[PstateMaxVal] leaves CurPstateLimit unchanged.)====="}}},
        {"MSRC001_0062", "P-state Control", R"=====()=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010062},
         {{63, 3, R"=====(Reserved. Read,Error-on-write-1.)====="}, {2, 0, R"=====(PstateCmd: P-state change command. Read-write. Reset: XXXb. Cold reset value varies by product; after a
warm reset, value initializes to the P-state the core was in prior to the reset. Writes to this field cause the core to
        change to the indicated non-boosted P-state number, specified by Core::X86::Msr::PStateDef. 0=P0, 1=P1, etc. P-
state limits are applied to any P-state requests made through this register. Reads from this field return the last
written value, regardless of whether any limits are applied.)====="}}},
        {"MSRC001_0063", "P-state Status", R"=====(Read,Error-on-write,Volatile. Reset: 0000_0000_0000_000Xh.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010063},
         {{63, 3, R"=====(Reserved.)====="}, {2, 0, R"=====(CurPstate: current P-state. Read,Error-on-write,Volatile. Reset: XXXb. This field provides the frequency
        component of the current non-boosted P-state of the core (regardless of the source of the P-state change, including
Core::X86::Msr::PStateCtl[PstateCmd]. 0=P0, 1=P1, etc. The value of this field is updated when the COF
transitions to a new value associated with a P-state.)====="}}},
        {"MSRC001_006[4...B]", "P-state [7:0]", R"=====(Read-write. Reset: X000_0000_XXXX_XXXXh.
Each of these registers specify the frequency and voltage associated with each of the core P-states.
The CpuVid field in these registers is required to be programmed to the same value in all cores of a processor, but are
allowed to be different between processors in a multi-processor system. All other fields in these registers are required to
        be programmed to the same value in each core of the coherent fabric.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[7:0]; ",
         {0xC001B006, 0xC001A006, 0xC0019006, 0xC0018006, 0xC0017006, 0xC0016006, 0xC0015006, 0xC0014006},
         {{63, 63, R"=====(PstateEn. Read-write. Reset: X. 0=The P-state specified by this MSR is not valid. 1=The P-state specified by this
MSR is valid. The purpose of this register is to indicate if the rest of the P-state information in the register is valid
after a reset; it controls no hardware.)====="},
          {62, 32, R"=====(Reserved.)====="},
          {31, 30, R"=====(IddDiv: current divisor. Read-write. Reset: XXb. See IddValue.)====="},
          {29, 22, R"=====(IddValue: current value. Read-write. Reset: XXXXXXXXb. After a reset, IddDiv and IddValue combine to
specify the expected maximum current dissipation of a single core that is in the P-state corresponding to the MSR
        number. These values are intended to be used to create ACPI-defined _PSS objects. The values are expressed in
        amps; they are not intended to convey final product power levels; they may not match the power levels specified
        in the Power and Thermal Datasheets.)====="},
          {21, 14, R"=====(CpuVid[7:0]: core VID. Read-write. Reset: XXXXXXXXb.)====="},
          {13, 8, R"=====(CpuDfsId: core divisor ID. Read-write. Reset: XXXXXXb. Specifies the core frequency divisor; see CpuFid.
For values [1Ah:08h], 1/8th integer divide steps supported down to VCO/3.25 (Note, L3/L2 fifo logic related to
4-cycle data heads-up requires core to be 1/3 of L3 frequency or higher). For values [30h:1Ch], 1/4th integer
        divide steps supported down to VCO/6 (DID[0] should zero if DID[5:0]>1Ah). (Note, core and L3 frequencies
        below 400MHz are not supported by the architecture). Core supports DID up to 30h, but L3 must be 2Ch
(VCO/5.5) or less.
ValidValues:
Value      Description
00h        Off
07h-01h Reserved.
08h        VCO/1
09h        VCO/1.125
1Ah-       VCO/<Value/8>
0Ah
1Bh        Reserved.
1Ch        VCO/<Value/8>
1Dh        Reserved.
1Eh        VCO/<Value/8>
1Fh        Reserved.
20h        VCO/<Value/8>
21h        Reserved.
22h        VCO/<Value/8>
23h        Reserved.
24h        VCO/<Value/8>
25h        Reserved.
26h       VCO/<Value/8>
27h       Reserved.
28h       VCO/<Value/8>
29h       Reserved.
2Ah       VCO/<Value/8>
2Bh       Reserved.
2Ch       VCO/<Value/8>
3Fh-      Reserved.
2D.)====="},
          {7, 0, R"=====(CpuFid[7:0]: core frequency ID. Read-write. Reset: XXh. Specifies the core frequency multiplier. The core
COF is a function of CpuFid and CpuDid, and defined by CoreCOF.
ValidValues:
Value     Description
0Fh-00h Reserved.
FFh-      <Value>*25
10h)====="}}},
        {"MSRC001_0073", "C-state Base Address", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010073},
         {{63, 16, R"=====(Reserved.)====="}, {15, 0, R"=====(CstateAddr: C-state address. Read-write. Reset: 0000h. Specifies the IO addresses trapped by the core for C-
state entry requests. A value of 0 in this field specifies that the core does not trap any IO addresses for C-state
        entry. Writing values greater than FFF8h into this field result in undefined behavior. All other values cause the
        core to trap IO addresses CstateAddr through CstateAddr+7.)====="}}},
        {"MSRC001_0074", "CPU Watchdog Timer", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010074},
         {{63, 7, R"=====(Reserved.)====="}, {6, 3, R"=====(CpuWdtCountSel: CPU watchdog timer count select. Read-write. Reset: 0h. CpuWdtCountSel and
        CpuWdtTimeBase together specify the time period required for the WDT to expire. The time period is ((the
multiplier specified by CpuWdtCountSel) * (the time base specified by CpuWdtTimeBase)). The actual timeout
        period may be anywhere from zero to one increment less than the values specified, due to non-deterministic
        behavior.
ValidValues:
Value     Description
0h        4095
1h        2047
2h        1023
3h        511
4h        255
5h        127
6h        63
7h        31
8h        8191
9h        16383
Fh-Ah Reserve.)====="},
          {2, 1, R"=====(CpuWdtTimeBase: CPU watchdog timer time base. Read-write. Reset: 0h. Specifies the time base for the
        timeout period specified in CpuWdtCountSel.
ValidValues:
Value    Description
0h       1.31ms
1h       1.28us
3h-2h    Reserve.)====="},
          {0, 0, R"=====(CpuWdtEn: CPU watchdog timer enable. Read-write. Reset: 0. Init: BIOS,1. 1=The WDT is enabled.)====="}}},
        {"MSRC001_0111", "SMM Base Address", R"=====(Reset: 0000_0000_0003_0000h.
This holds the base of the SMM memory region. The value of this register is stored in the save state on entry into SMM
        (see 2.1.8.1.5 [SMM Save State]) and it is restored on returning from SMM. The 16-bit CS (code segment) selector is
loaded with SmmBase[19:4] on entering SMM. SmmBase[3:0] is required to be 0. The SMM base address can be
changed in two ways:
• The SMM base address, at offset FF00h in the SMM state save area, may be changed by the SMI handler. The
        RSM instruction updates SmmBase with the new value.
• Normal WRMSR access to this register.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010111},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(SmmBase. Reset: 0003_0000h.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Read-only : Read-write.)====="}}},
        {"MSRC001_0112", "SMM TSeg Base Address", R"=====(Configurable. Reset: 0000_0000_0000_0000h.
See 2.1.8.1 [System Management Mode (SMM)] and 2.1.3.3.1 [Memory Access to the Physical Address Space]. See
        Core::X86::Msr::SMMMask for more information about the ASeg and TSeg address ranges.

Each CPU access, directed at CPUAddr, is determined to be in the TSeg range if the following is true:

CPUAddr[47:17] & TSegMask[47:17] == TSegBase[47:17] & TSegMask[47:17].

For example, if TSeg spans 256 KB and starts at the 1 MB address. The Core::X86::Msr::SMMAddr[TSegBase[47:17]]
would be set to 0010_0000h and the Core::X86::Msr::SMMMask[TSegMask[47:17]] to FFFC_0000h (with zeros filling
in for bits[16:0]). This results in a TSeg range from 0010_0000 to 0013_FFFFh.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010112},
         {{63, 48, R"=====(Reserved.)====="}, {47, 17, R"=====(TSegBase[47:17]: TSeg address range base. Configurable. Reset: 0000_0000h. AccessType:
(Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {16, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_0113", "SMM TSeg Mask", R"=====(Configurable. Reset: 0000_0000_0000_0000h.
See 2.1.8.1 [System Management Mode (SMM)].
The ASeg address range is located at a fixed address from A0000h–BFFFFh. The TSeg range is located at a variable base
(specified by Core::X86::Msr::SMMAddr[TSegBase[47:17]]) with a variable size (specified by
        Core::X86::Msr::SMMMask[TSegMask[47:17]]). These ranges provide a safe location for SMM code and data that is
not readily accessible by non-SMM applications. The SMI handler can be located in one of these two ranges, or it can be
        located outside these ranges. These ranges must never overlap each other.
This register specifies how accesses to the ASeg and TSeg address ranges are controlled as follows:

•   If [A,T]Valid == 1, then:
• If in SMM, then:
• If [A, T]Close == 0, then the accesses are directed to DRAM with memory type as specified in
[A, T]MTypeDram.
• If [A, T]Close == 1, then instruction accesses are directed to DRAM with memory type as
        specified in [A, T]MTypeDram and data accesses are directed at MMIO space and with
        attributes based on [A, T]MTypeIoWc.
• If not in SMM, then the accesses are directed at MMIO space with attributes based on
[A,T]MTypeIoWc.
•   See 2.1.3.3.1.1 [Determining Memory Type].)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0010113},
         {{63, 48, R"=====(Reserved.)====="}, {47, 17, R"=====(TSegMask[47:17]: TSeg address range mask. Configurable. Reset: 0000_0000h. See
        Core::X86::Msr::SMMAddr. AccessType: (Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {16, 15, R"=====(Reserved.)====="},
          {14, 12, R"=====(TMTypeDram: TSeg address range memory type. Configurable. Reset: 0h. Specifies the memory type for
SMM accesses to the TSeg range that are directed to DRAM. AccessType:
(Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {11, 11, R"=====(Reserved.)====="},
          {10, 8, R"=====(AMTypeDram: ASeg Range Memory Type. Configurable. Reset: 0h. Specifies the memory type for SMM
        accesses to the ASeg range that are directed to DRAM. AccessType: (Core::X86::Msr::HWCR[SmmLock]) ?
Read-only : Read-write.
ValidValues:
Value     Description
0h        UC or uncacheable.
1h        WC or write combining.
3h-2h     Reserved.
4h        WT or write through.
5h        WP or write protect.
6h        WB or write back.
7h        Reserved.)====="},
          {7, 6, R"=====(Reserved.)====="},
          {5, 5, R"=====(TMTypeIoWc: non-SMM TSeg address range memory type. Configurable. Reset: 0. 0=UC (uncacheable).
1=WC (write combining). Specifies the attribute of TSeg accesses that are directed to MMIO space. AccessType:
(Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {4, 4, R"=====(AMTypeIoWc: non-SMM ASeg address range memory type. Configurable. Reset: 0. 0=UC (uncacheable).
1=WC (write combining). Specifies the attribute of ASeg accesses that are directed to MMIO space. AccessType:
(Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {3, 3, R"=====(TClose: send TSeg address range data accesses to MMIO. Configurable. Reset: 0. 1=When in SMM, direct
        data accesses in the TSeg address range to MMIO space. See AClose. AccessType:
(Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {2, 2, R"=====(AClose: send ASeg address range data accesses to MMIO. Configurable. Reset: 0. 1=When in SMM, direct
        data accesses in the ASeg address range to MMIO space. [A,T]Close allows the SMI handler to access the MMIO
        space located in the same address region as the [A,T]Seg. When the SMI handler is finished accessing the MMIO
        space, it must clear the bit. Failure to do so before resuming from SMM causes the CPU to erroneously read the
save state from MMIO space. AccessType: (Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {1, 1, R"=====(TValid: enable TSeg SMM address range. Configurable. Reset: 0. 1=The TSeg address range SMM enabled.
AccessType: (Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="},
          {0, 0, R"=====(AValid: enable ASeg SMM address range. Configurable. Reset: 0. 1=The ASeg address range SMM enabled.
AccessType: (Core::X86::Msr::HWCR[SmmLock]) ? Read-only : Read-write.)====="}}},
        {"MSRC001_0114", "Virtual Machine Control", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010114},
         {{63, 32, R"=====(Reserved.)====="}, {31, 5, R"=====(Reserved. Read-only,Error-on-write-1. Reset: 000_0000h.)====="}, {4, 4, R"=====(SvmeDisable: SVME disable. Configurable. Reset: 0. 0=Core::X86::Msr::EFER[SVME] is read-write.
1=Core::X86::Msr::EFER[SVME] is Read-only,Error-on-write-1. See Lock for the access type of this field.
Attempting to set this field when (Core::X86::Msr::EFER[SVME]==1) causes a #GP fault, regardless of the state
        of Lock. See the APM2 section titled “Enabling SVM" for software use of this field.)====="},
          {3, 3, R"=====(Lock: SVM lock. Read-only,Volatile. Reset: 0. 0=SvmeDisable is read-write. 1=SvmeDisable is read-only. See
        Core::X86::Msr::SvmLockKey[SvmLockKey] for the condition that causes hardware to clear this field.)====="},
          {2, 2, R"=====(Reserved.)====="},
          {1, 1, R"=====(InterceptInit: intercept INIT. Read-write,Volatile. Reset: 0. 0=INIT delivered normally. 1=INIT translated into
        a SX interrupt. This bit controls how INIT is delivered in host mode. This bit is set by hardware when the SKINIT
        instruction is executed.)====="},
          {0, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_0115", "IGNNE", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010115},
         {{63, 32, R"=====(Reserved.)====="}, {31, 1, R"=====(Reserved. Read-only,Error-on-write-1. Reset: 0000_0000h.)====="}, {0, 0, R"=====(IGNNE: current IGNNE state. Read-write. Reset: 0. This bit controls the current state of the processor internal
IGNNE signal.)====="}}},
        {"MSRC001_0116", "SMM Control", R"=====(Reset: 0000_0000_0000_0000h.
The bits in this register are processed in the order of: SmmEnter, SmiCycle, SmmDismiss, RsmCycle and SmmExit.
However, only the following combination of bits may be set in a single write (all other combinations result in undefined
        behavior):
• SmmEnter and SmiCycle.
• SmmEnter and SmmDismiss.
• SmmEnter, SmiCycle and SmmDismiss.
• SmmExit and RsmCycle.
Software is responsible for ensuring that SmmEnter and SmmExit operations are properly matched and are not nested.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010116},
         {{63, 5, R"=====(Reserved. Error-on-read,Error-on-write-1. Reset: 000_0000_0000_0000h.)====="}, {4, 4, R"=====(RsmCycle: send RSM special cycle. Reset: 0. 1=Send a RSM special cycle.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Error-on-read,Error-on-write : Write-only,Error-on-read.)====="},
          {3, 3, R"=====(SmmExit: exit SMM. Reset: 0. 1=Exit SMM.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Error-on-read,Error-on-write : Write-only,Error-on-read.)====="},
          {2, 2, R"=====(SmiCycle: send SMI special cycle. Reset: 0. 1=Send a SMI special cycle.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Error-on-read,Error-on-write : Write-only,Error-on-read.)====="},
          {1, 1, R"=====(SmmEnter: enter SMM. Reset: 0. 1=Enter SMM.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Error-on-read,Error-on-write : Write-only,Error-on-read.)====="},
          {0, 0, R"=====(SmmDismiss: clear SMI. Reset: 0. 1=Clear the SMI pending flag.
AccessType: Core::X86::Msr::HWCR[SmmLock] ? Error-on-read,Error-on-write : Write-only,Error-on-read.)====="}}},
        {"MSRC001_0117", "Virtual Machine Host Save Physical Address", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010117},
         {{63, 48, R"=====(Reserved. Read-only,Error-on-write-1. Reset: 0000h.)====="}, {47, 12, R"=====(VM_HSAVE_PA: physical address of host save area. Read-write. Reset: 0_0000_0000h. This register contains
the physical address of a 4-KB region where VMRUN saves host state and where vm-exit restores host state from.
Writing this register causes a #GP if (FFFF_FFFF_Fh >= VM_HSAVE_PA >= FFFD_0000_0h) or if either the
TSEG or ASEG regions overlap with the range defined by this register.)====="},
          {11, 0, R"=====(Reserved. Read-only,Error-on-write-1. Reset: 000h.)====="}}},
        {"MSRC001_0118", "SVM Lock Key", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010118},
         {{63, 0, R"=====(SvmLockKey: SVM lock key. Read-write. Reset: Fixed,0000_0000_0000_0000h. Writes to this register when
(Core::X86::Msr::VM_CR[Lock] == 0) modify SvmLockKey. If ((Core::X86::Msr::VM_CR[Lock] == 1) &&
(SvmLockKey!=0) && (The write value == The value stored in SvmLockKey)) for a write to this register then
        hardware updates Core::X86::Msr::VM_CR[Lock]=0.)====="}}},
        {"MSRC001_011A", "Local SMI Status", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
This register returns the same information that is returned in Core::X86::Smm::LocalSmiStatus portion of the SMM save
        state. The information in this register is only updated when Core::X86::Msr::SMM_CTL[SmmDismiss] is set by
        software.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001011A},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(LocalSmiStatus. Read-write. Reset: 0000_0000h. See Core::X86::Smm::LocalSmiStatus.)====="}}},
        {"MSRC001_011B", "AVIC Doorbell", R"=====(Write-only,Error-on-read. Reset: 0000_0000_0000_0000h.
The ApicId is a physical APIC Id; not valid for logical APIC ID.
Enable: (Core::X86::Cpuid::SvmRevFeatIdEdx[AVIC]==1).)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001011B},
         {{63, 8, R"=====(Reserved. Write-only,Error-on-read. Reset: 00_0000_0000_0000h.)====="}, {7, 0, R"=====(ApicId: APIC ID [7:0]. Write-only,Error-on-read. Reset: 00h.)====="}}},
        {"MSRC001_011E", "VM Page Flush", R"=====(Reset: XXXX_XXXX_XXXX_XXXXh.
Writes to this MSR cause 4KB of encrypted, guest-tagged data to be flushed from caches if present.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001011E},
         {{63, 12, R"=====(VirtualAddr. Reset: X_XXXX_XXXX_XXXXh. Guest physical address of page to flush.
AccessType: Core::X86::Msr::SYS_CFG[SMEE] ? Write-only,Error-on-read : Error-on-read,Error-on-write.)====="},
          {11, 0, R"=====(ASID. Reset: XXXh. ASID to use for flush. Writing reserved values generates #GP.
AccessType: Core::X86::Msr::SYS_CFG[SMEE] ? Write-only,Error-on-read : Error-on-read,Error-on-write.
ValidValues:
Value     Description
00Fh-     Valid ASID
000h
FFFh- Reserved.
010h)====="}}},
        {"MSRC001_0130", "Guest Host Communication Block", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
If Core::X86::Msr::GHCB is accessed in hypervisor mode, #GP is generated.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010130},
         {{63, 0, R"=====(GHCBPA. Read-write. Reset: 0000_0000_0000_0000h. Guest physical address of GHCB.)====="}}},
        {"MSRC001_0131", "SEV Status", R"=====(Read,Error-on-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010131},
         {{63, 2, R"=====(Reserved.)====="}, {1, 1, R"=====(SevEsEnabled. Read,Error-on-write. Reset: 0. 1=The guest was launched with the Sev-ES feature enabled in
VMCB offset 90h.)====="},
          {0, 0, R"=====(SevEnabled. Read,Error-on-write. Reset: 0. 1=The guest was launched with SEV feature enabled in VMCB
offset 90h.)====="}}},
        {"MSRC001_0140", "OS Visible Work-around Length", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010140},
         {{63, 16, R"=====(Reserved.)====="}, {15, 0, R"=====(OSVWIdLength: OS visible work-around ID length. Read-write. Reset: 0000h. See the Revision Guide for the
        definition of this field; see 1.2 [Reference Documents].)====="}}},
        {"MSRC001_0141", "OS Visible Work-around Status", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0010141},
         {{63, 0, R"=====(OsvwStatusBits: OS visible work-around status bits. Read-write. Reset: 0000_0000_0000_0000h. See the
Revision Guide for the definition of this field; see 1.2 [Reference Documents].)====="}}},
        {"MSRC001_020[0...A]", "Performance Event Select [5:0]", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
See 2.1.11 [Performance Monitor Counters]. Core::X86::Msr::PERF_LEGACY_CTL is an alias of
        MSRC001_020[6,4,2,0].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[5:0]; ",
         {0xC001A020, 0xC0018020, 0xC0016020, 0xC0014020, 0xC0012020, 0xC0010020},
         {{63, 42, R"=====(Reserved.)====="}, {41, 40, R"=====(HostGuestOnly: count only host/guest events. Read-write. Reset: 0h.
ValidValues:
Value      Description
0h         Count all events, irrespective of guest/host.
1h         Count guest events if [SVME] == 1.
2h         Count host events if [SVME] == 1.
3h         Count all guest and host events if [SVME] == 1.)====="},
          {39, 36, R"=====(Reserved.)====="},
          {35, 32, R"=====(EventSelect[11:8]: performance event select. Read-write. Reset: 0h.)====="},
          {31, 24, R"=====(CntMask: counter mask. Read-write. Reset: 00h. Controls the number of events counted per clock cycle.
ValidValues:
Value      Description
00h        The corresponding PERF_CTR[5:0] register increments by the number of events occurring in a clock
cycle. Maximum number of events in one cycle is 32.
7Fh-01h When Inv == 0, the corresponding PERF_CTR[5:0] register increments by 1, if the number of events
occurring in a clock cycle is greater than or equal to the CntMask value. When Inv == 1, the
        corresponding PERF_CTR[5:0] register increments by 1, if the number of events occurring in a clock
cycle is less than CntMask value.
FFh-       Reserved.
80.)====="},
          {23, 23, R"=====(Inv: invert counter mask. Read-write. Reset: 0. See CntMask.)====="},
          {22, 22, R"=====(En: enable performance counter. Read-write. Reset: 0. 1=Performance event counter is enabled.)====="},
          {21, 21, R"=====(Reserved.)====="},
          {20, 20, R"=====(Int: enable APIC interrupt. Read-write. Reset: 0. 1=APIC performance counter LVT interrupt is enabled to
generate an interrupt via Core::X86::Apic::PerformanceCounterLvtEntry when the performance counter
        overflows.)====="},
          {19, 19, R"=====(Reserved.)====="},
          {18, 18, R"=====(Edge: edge detect. Read-write. Reset: 0. 0=Level detect. 1=Edge detect. Read-write. The edge count mode
increments the counter when a transition happens on the monitored event. If the event selected is changed without
        disabling the counter, an extra edge is falsely detected when the first event is a static 0 and the second event is a
static one. To avoid this false edge detection, disable the counter when changing the event and then enable the
        counter with a second MSR write.)====="},
          {17, 16, R"=====(OsUserMode: OS and user mode. Read-write. Reset: 0h.
ValidValues:
Value      Description
0h         Count no events.
1h         Count user events (CPL>0).
2h         Count OS events (CPL=0).
3h         Count all events, irrespective of the CPL.)====="},
          {15, 8, R"=====(UnitMask: event qualification. Read-write. Reset: 00h. Each UnitMask bit further specifies or qualifies the
event specified by EventSelect. All events selected by UnitMask are simultaneously monitored. Unless otherwise
stated, the UnitMask values shown may be combined (logically ORed) to select any desired combination of the
        sub-events for a given event. In some cases, certain combinations can result in misleading counts, or the
        UnitMask value is an ordinal rather than a bit mask. These situations are described where applicable, or should be
obvious from the event descriptions. For events where no UnitMask table is shown, the UnitMask is Unused.
When selecting an event for which not all UnitMask bits are defined, the undefined UnitMask bits should be set
        to zero.)====="},
          {7, 0, R"=====(EventSelect[7:0]: event select. Read-write. Reset: 00h. EventSelect[11:0] = {EventSelect[11:8],
EventSelect[7:0]}. EventSelect specifies the event or event duration in a processor unit to be counted by the
        corresponding PERF_CTR[5:0] register. The events are specified in 2.1.11.3 [Core Performance Monitor
        Counters]. Some events are reserved; when a reserved event is selected, the results are undefined.)====="}}},
        {"MSRC001_020[1...B]", "Performance Event Counter [5:0]", R"=====(Reset: 0000_0000_0000_0000h.
See Core::X86::Msr::PERF_CTL. Core::X86::Msr::PERF_LEGACY_CTR is an alias of MSRC001_020[7,5,3,1]. Also
        can be read via x86 instructions RDPMC ECX=[05:00].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]_n[5:0]; ",
         {0xC001B020, 0xC0019020, 0xC0017020, 0xC0015020, 0xC0013020, 0xC0011020},
         {{63, 48, R"=====(Reserved. Read-only. Reset: Fixed,0000h.)====="}, {47, 0, R"=====(CTR: performance counter value. Read-write,Volatile. Reset: 0000_0000_0000h.)====="}}},
        {"MSRC001_023[0...A]", "L3 Performance Event Select [5:0]", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
.)=====",
         "_lthree[1:0]_n[5:0]; ",
         {0xC001A023, 0xC0018023, 0xC0016023, 0xC0014023, 0xC0012023, 0xC0010023},
         {{63, 56, R"=====(ThreadMask. Read-write. Reset: 00h. Controls which of the 8 threads in the complex are being counted.
ValidValues:
Bit        Description
[0]        Core 0 Thread 0 mask.
[1]        Core 0 Thread 1 mask.
[2]        Core 1 Thread 0 mask.
[3]        Core 1 Thread 1 mask.
[4]        Core 2 Thread 0 mask.
[5]        Core 2 Thread 1 mask.
[6]        Core 3 Thread 0 mask.
[7]        Core 3 Thread 1 mask.)====="},
          {55, 52, R"=====(Reserved.)====="},
          {51, 48, R"=====(SliceMask. Read-write. Reset: 0h. Controls which L3 slices are counting this event.
ValidValues:
Bit        Description
[0]        L3 Slice 0 mask.
[1]        L3 Slice 1 mask.
[2]        L3 Slice 2 mask.
[3]        L3 Slice 3 mask.)====="},
          {47, 23, R"=====(Reserved.)====="},
          {22, 22, R"=====(Enable: Enable L3 performance counter. Read-write. Reset: 0. 1=Enable.)====="},
          {21, 16, R"=====(Reserved.)====="},
          {15, 8, R"=====(UnitMask: event qualification. Read-write. Reset: 00h. Each UnitMask bit further specifies or qualifies the
event specified by EventSelect. All events selected by UnitMask are simultaneously monitored. Unless otherwise
stated, the UnitMask values shown may be combined (logically ORed) to select any desired combination of the
        sub-events for a given event. In some cases, certain combinations can result in misleading counts, or the
        UnitMask value is an ordinal rather than a bit mask. These situations are described where applicable, or should be
obvious from the event descriptions. For events where no UnitMask table is shown, the UnitMask is Unused.
When selecting an event for which not all UnitMask bits are defined, the undefined UnitMask bits should be set
        to zero.)====="},
          {7, 0, R"=====(EventSel: event select. Read-write. Reset: 00h.)====="}}},
        {"MSRC001_023[1...B]", "L3 Performance Event Counter [5:0]", R"=====(Reset: 0000_0000_0000_0000h.
Also can be read via x86 instructions RDPMC ECX=[0F:0A].)=====",
         "_lthree[1:0]_n[5:0]; ",
         {0xC001B023, 0xC0019023, 0xC0017023, 0xC0015023, 0xC0013023, 0xC0011023},

         {{63, 49, R"=====(Reserved.)====="}, {48, 48, R"=====(Overflow. Read-write. Reset: 0.)====="}, {47, 32, R"=====(CountHi. Read-write,Volatile. Reset: 0000h.)====="}, {31, 0, R"=====(CountLo. Read-write,Volatile. Reset: 0000_0000h.)====="}}},
        {"MSRC001_0299", "RAPL Power Unit", R"=====(Read-only,Volatile. Reset: 0000_0000_000A_1003h.)=====",
         "_lthree[1:0]; ",
         {0xC0010299},
         {
                 {63, 20, R"=====(Reserved.)====="},
                 {19, 16, R"=====(TU: Time Units in seconds. Read-only,Volatile. Reset: Ah.
Time information (in Seconds) is based on the multiplier, 1/ 2^TU; where TU is an unsigned integer. Default value is 1010b, indicating time unit is in 976 microseconds increment.
ValidValues:
Value     Description
Fh-0h     1/2^<Value> Second.)====="},
          {15, 13, R"=====(Reserved.)====="},
          {12, 8, R"=====(ESU: Energy Status Units. Read-only,Volatile. Reset: 10h.
Energy information (in Joules) is based on the multiplier, 1/2^ESU; where ESU is an unsigned integer. Default value is 10000b, indicating energy status unit is in 15.3 micro-Joules increment.
ValidValues:
Value     Description
1Fh-00h 1/2^<Value> Joule.)====="},
          {7, 4, R"=====(Reserved.)====="},
          {3, 0, R"=====(PU: Power Units. Read-only,Volatile. Reset: 3h.
Power information (in Watts) is based on the multiplier, 1/2^PU; where PU is an unsigned integer. Default value is 0011b, indicating power unit is in 1/8 Watts increment.
ValidValues:
Value     Description
Fh-0h     1/2^<Value> Watts)====="}}},
        {"MSRC001_029A", "Core Energy Status", R"=====(Read-only,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC001029A},
         {
                 {63, 32, R"=====(Reserved.)====="},
                 {31, 0, R"=====(TotalEnergyConsumed. Read-only,Volatile. Reset: 0000_0000h.)====="}}},
        {"MSRC001_029B", "Package Energy Status", R"=====(Read-only,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]; ",
         {0xC001029B},
         {
                 {63, 32, R"=====(Reserved.)====="},
                 {31, 0, R"=====(TotalEnergyConsumed. Read-only,Volatile. Reset: 0000_0000h.)====="}}},
        {"MSRC001_1002", "CPUID Features for CPUID Fn00000007_E[A,B]X", R"=====(Read-write.
Core::X86::Msr::CPUID_7_Features[63:32] provides control over values read from
Core::X86::Cpuid::StructExtFeatIdEax0; Core::X86::Msr::CPUID_7_Features[31:0] provides control over values read
        from Core::X86::Cpuid::StructExtFeatIdEbx0.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011002},
         {{63, 30, R"=====(Reserved.)====="}, {29, 29, R"=====(SHA. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[SHA].)====="}, {28, 24, R"=====(Reserved.)====="}, {23, 23, R"=====(CLFSHOPT. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[CLFSHOPT].)====="}, {22, 21, R"=====(Reserved.)====="}, {20, 20, R"=====(SMAP. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[SMAP].)====="}, {19, 19, R"=====(ADX. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[ADX].)====="}, {18, 18, R"=====(RDSEED. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[RDSEED].)====="}, {17, 9, R"=====(Reserved.)====="}, {8, 8, R"=====(BMI2. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[BMI2].)====="}, {7, 7, R"=====(SMEP. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[SMEP].)====="}, {6, 6, R"=====(Reserved.)====="}, {5, 5, R"=====(AVX2. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[AVX2].)====="}, {4, 4, R"=====(Reserved.)====="}, {3, 3, R"=====(BMI1. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[BMI1].)====="}, {2, 1, R"=====(Reserved.)====="}, {0, 0, R"=====(FSGSBASE. Read-write. Reset: Core::X86::Cpuid::StructExtFeatIdEbx0[FSGSBASE].)====="}}},
        {"MSRC001_1003", "Thermal and Power Management CPUID Features", R"=====(Read-write.
Core::X86::Msr::CPUID_PWR_THERM provides control over values read from
        Core::X86::Cpuid::ThermalPwrMgmtEcx.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011003},
         {{63, 1, R"=====(Reserved.)====="}, {0, 0, R"=====(EffFreq. Read-write. Reset: Core::X86::Cpuid::ThermalPwrMgmtEcx[EffFreq].)====="}}},
        {"MSRC001_1004", "CPUID Features for CPUID Fn00000001_E[C,D]X", R"=====(Read-write.
Core::X86::Msr::CPUID_Features[63:32] provides control over values read from Core::X86::Cpuid::FeatureIdEcx;
Core::X86::Msr::CPUID_Features[31:0] provides control over values read from Core::X86::Cpuid::FeatureIdEdx.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011004},
         {{63, 63, R"=====(Reserved.)====="}, {62, 62, R"=====(RDRAND. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[RDRAND].)====="}, {61, 61, R"=====(F16C. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[F16C].)====="}, {60, 60, R"=====(AVX. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[AVX].)====="}, {59, 59, R"=====(OSXSAVE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[OSXSAVE]. Modifies
        Core::X86::Cpuid::FeatureIdEcx[OSXSAVE] only if CR4[OSXSAVE].)====="},
          {58, 58, R"=====(XSAVE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[XSAVE].)====="},
          {57, 57, R"=====(AES. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[AES]. Modifies
        Core::X86::Cpuid::FeatureIdEcx[AES] only if the reset value is 1.)====="},
          {56, 56, R"=====(Reserved.)====="},
          {55, 55, R"=====(POPCNT. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[POPCNT].)====="},
          {54, 54, R"=====(MOVBE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[MOVBE].)====="},
          {53, 53, R"=====(Reserved.)====="},
          {52, 52, R"=====(SSE42. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[SSE42].)====="},
          {51, 51, R"=====(SSE41. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[SSE41].)====="},
          {50, 50, R"=====(Reserved.)====="},
          {49, 49, R"=====(PCID. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[PCID].)====="},
          {48, 46, R"=====(Reserved.)====="},
          {45, 45, R"=====(CMPXCHG16B. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[CMPXCHG16B].)====="},
          {44, 44, R"=====(FMA. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[FMA].)====="},
          {43, 42, R"=====(Reserved.)====="},
          {41, 41, R"=====(SSSE3. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[SSSE3].)====="},
          {40, 36, R"=====(Reserved.)====="},
          {35, 35, R"=====(Monitor. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[Monitor]. Modifies
        Core::X86::Cpuid::FeatureIdEcx[Monitor] only if ~Core::X86::Msr::HWCR[MonMwaitDis].)====="},
          {34, 34, R"=====(Reserved.)====="},
          {33, 33, R"=====(PCLMULQDQ. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[PCLMULQDQ]. Modifies
        Core::X86::Cpuid::FeatureIdEcx[PCLMULQDQ] only if the reset value is 1.)====="},
          {32, 32, R"=====(SSE3. Read-write. Reset: Core::X86::Cpuid::FeatureIdEcx[SSE3].)====="},
          {31, 29, R"=====(Reserved.)====="},
          {28, 28, R"=====(HTT. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[HTT].)====="},
          {27, 27, R"=====(Reserved.)====="},
          {26, 26, R"=====(SSE2. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[SSE2].)====="},
          {25, 25, R"=====(SSE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[SSE].)====="},
          {24, 24, R"=====(FXSR. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[FXSR].)====="},
          {23, 23, R"=====(MMX: MMX™ instructions. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[MMX].)====="},
          {22, 20, R"=====(Reserved.)====="},
          {19, 19, R"=====(CLFSH. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[CLFSH].)====="},
          {18, 18, R"=====(Reserved.)====="},
          {17, 17, R"=====(PSE36. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[PSE36].)====="},
          {16, 16, R"=====(PAT. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[PAT].)====="},
          {15, 15, R"=====(CMOV. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[CMOV].)====="},
          {14, 14, R"=====(MCA. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[MCA].)====="},
          {13, 13, R"=====(PGE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[PGE].)====="},
          {12, 12, R"=====(MTRR. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[MTRR].)====="},
          {11, 11, R"=====(SysEnterSysExit. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[SysEnterSysExit].)====="},
          {10, 10, R"=====(Reserved.)====="},
          {9, 9, R"=====(APIC. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[APIC]. Modifies
        Core::X86::Cpuid::FeatureIdEdx[APIC] only if Core::X86::Msr::APIC_BAR[ApicEn].)====="},
          {8, 8, R"=====(CMPXCHG8B. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[CMPXCHG8B].)====="},
          {7, 7, R"=====(MCE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[MCE].)====="},
          {6, 6, R"=====(PAE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[PAE].)====="},
          {5, 5, R"=====(MSR. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[MSR].)====="},
          {4, 4, R"=====(TSC. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[TSC].)====="},
          {3, 3, R"=====(PSE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[PSE].)====="},
          {2, 2, R"=====(DE. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[DE].)====="},
          {1, 1, R"=====(VME. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[VME].)====="},
          {0, 0, R"=====(FPU. Read-write. Reset: Core::X86::Cpuid::FeatureIdEdx[FPU].)====="}}},
        {"MSRC001_1005", "CPUID Features for CPUID Fn80000001_E[C,D]X", R"=====(Read-write.
Core::X86::Msr::CPUID_ExtFeatures[63:32] provides control over values read from
Core::X86::Cpuid::FeatureExtIdEcx; Core::X86::Msr::CPUID_ExtFeatures[31:0] provides control over values read from
Core::X86::Cpuid::FeatureExtIdEdx.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011005},
         {{63, 62, R"=====(Reserved.)====="}, {61, 61, R"=====(MwaitExtended. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[MwaitExtended].)====="}, {60, 60, R"=====(PerfCtrExtLLC. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[PerfCtrExtLLC].)====="}, {59, 59, R"=====(PerfTsc. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[PerfTsc].)====="}, {58, 58, R"=====(DataBreakpointExtension. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[DataBreakpointExtension].)====="}, {57, 57, R"=====(Reserved.)====="}, {56, 56, R"=====(PerfCtrExtDF. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[PerfCtrExtDF].)====="}, {55, 55, R"=====(PerfCtrExtCore. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[PerfCtrExtCore].)====="}, {54, 54, R"=====(TopologyExtensions. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[TopologyExtensions].)====="}, {53, 50, R"=====(Reserved.)====="}, {49, 49, R"=====(TCE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[TCE].)====="}, {48, 48, R"=====(FMA4. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[FMA4]. Init: 0.)====="}, {47, 47, R"=====(LWP. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[LWP].)====="}, {46, 46, R"=====(Reserved.)====="}, {45, 45, R"=====(WDT. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[WDT].)====="}, {44, 44, R"=====(SKINIT. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[SKINIT].)====="}, {43, 43, R"=====(XOP. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[XOP].)====="}, {42, 42, R"=====(IBS. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[IBS]. Init: BIOS,0. To enable the IBS feature, use
        BIOS setup option.)====="},
          {41, 41, R"=====(OSVW. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[OSVW].)====="},
          {40, 40, R"=====(ThreeDNowPrefetch. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[ThreeDNowPrefetch].)====="},
          {39, 39, R"=====(MisAlignSse. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[MisAlignSse].)====="},
          {38, 38, R"=====(SSE4A. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[SSE4A].)====="},
          {37, 37, R"=====(ABM. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[ABM].)====="},
          {36, 36, R"=====(AltMovCr8. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[AltMovCr8].)====="},
          {35, 35, R"=====(ExtApicSpace. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[ExtApicSpace].)====="},
          {34, 34, R"=====(SVM. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[SVM].)====="},
          {33, 33, R"=====(CmpLegacy. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[CmpLegacy].)====="},
          {32, 32, R"=====(LahfSahf. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEcx[LahfSahf].)====="},
          {31, 31, R"=====(ThreeDNow: 3DNow!™ instructions. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[ThreeDNow].)====="},
          {30, 30, R"=====(ThreeDNowExt. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[ThreeDNowExt].)====="},
          {29, 29, R"=====(LM. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[LM].)====="},
          {28, 28, R"=====(Reserved.)====="},
          {27, 27, R"=====(RDTSCP. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[RDTSCP].)====="},
          {26, 26, R"=====(Page1GB. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[Page1GB].)====="},
          {25, 25, R"=====(FFXSR. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[FFXSR].)====="},
          {24, 24, R"=====(FXSR. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[FXSR].)====="},
          {23, 23, R"=====(MMX: MMX™ instructions. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MMX].)====="},
          {22, 22, R"=====(MmxExt. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MmxExt].)====="},
          {21, 21, R"=====(Reserved.)====="},
          {20, 20, R"=====(NX. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[NX].)====="},
          {19, 18, R"=====(Reserved.)====="},
          {17, 17, R"=====(PSE36. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[PSE36].)====="},
          {16, 16, R"=====(PAT. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[PAT].)====="},
          {15, 15, R"=====(CMOV. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[CMOV].)====="},
          {14, 14, R"=====(MCA. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MCA].)====="},
          {13, 13, R"=====(PGE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[PGE].)====="},
          {12, 12, R"=====(MTRR. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MTRR].)====="},
          {11, 11, R"=====(SysCallSysRet. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[SysCallSysRet].)====="},
          {10, 10, R"=====(Reserved.)====="},
          {9, 9, R"=====(APIC. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[APIC].)====="},
          {8, 8, R"=====(CMPXCHG8B. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[CMPXCHG8B].)====="},
          {7, 7, R"=====(MCE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MCE].)====="},
          {6, 6, R"=====(PAE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[PAE].)====="},
          {5, 5, R"=====(MSR. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[MSR].)====="},
          {4, 4, R"=====(TSC. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[TSC].)====="},
          {3, 3, R"=====(PSE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[PSE].)====="},
          {2, 2, R"=====(DE. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[DE].)====="},
          {1, 1, R"=====(VME. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[VME].)====="},
          {0, 0, R"=====(FPU. Read-write. Reset: Core::X86::Cpuid::FeatureExtIdEdx[FPU].)====="}}},
        {"MSRC001_1019", "Address Mask For DR1 Breakpoint", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
Support indicated by Core::X86::Cpuid::FeatureExtIdEcx[DataBreakpointExtension].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011019},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(AddrMask: mask for DR linear address data breakpoint DR1. Read-write. Reset: 0000_0000h. 1=Exclude bit
into address compare. 0=Include bit into address compare. See Core::X86::Msr::DR1_ADDR_MASK.
AddrMask[11:0] qualifies the DR1 linear address instruction breakpoint, allowing the DR1 instruction breakpoint
        on a range of addresses in memory.)====="}}},
        {"MSRC001_101A", "Address Mask For DR2 Breakpoint", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
Support indicated by Core::X86::Cpuid::FeatureExtIdEcx[DataBreakpointExtension].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001101A},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(AddrMask: mask for DR linear address data breakpoint DR2. Read-write. Reset: 0000_0000h. 1=Exclude bit
into address compare. 0=Include bit into address compare. See Core::X86::Msr::DR0_ADDR_MASK.
AddrMask[11:0] qualifies the DR2 linear address instruction breakpoint, allowing the DR2 instruction breakpoint
        on a range of addresses in memory.)====="}}},
        {"MSRC001_101B", "Address Mask For DR3 Breakpoint", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
Support indicated by Core::X86::Cpuid::FeatureExtIdEcx[DataBreakpointExtension].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001101B},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(AddrMask: mask for DR linear address data breakpoint DR3. Read-write. Reset: 0000_0000h. 1=Exclude bit
into address compare. 0=Include bit into address compare. See Core::X86::Msr::DR0_ADDR_MASK.
AddrMask[11:0] qualifies the DR3 linear address instruction breakpoint, allowing the DR3 instruction breakpoint
        on a range of addresses in memory.)====="}}},
        {"MSRC001_1023", "Table Walker Configuration", R"=====(Read-write. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]; ",
         {0xC0011023},
         {{63, 50, R"=====(Reserved.)====="}, {49, 49, R"=====(TwCfgCombineCr0Cd: combine CR0_CD for both threads of a core. Read-write. Reset: 0. Init: BIOS,1.
1=The host Cr0_Cd values from the two threads are OR'd together and used by both threads.)====="},
          {48, 0, R"=====(Reserved.)====="}}},
        {"MSRC001_1027", "Address Mask For DR0 Breakpoints", R"=====(Read-write. Reset: 0000_0000_0000_0000h.
Support for DR0[31:12] is indicated by Core::X86::Cpuid::FeatureExtIdEcx[DataBreakpointExtension]. See
        Core::X86::Msr::DR1_ADDR_MASK.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011027},
         {{63, 32, R"=====(Reserved.)====="}, {31, 0, R"=====(DR0: mask for DR0 linear address data breakpoint. Read-write. Reset: 0000_0000h. 1=Exclude bit into
        address compare. 0=Include bit into address compare. See Core::X86::Msr::DR1_ADDR_MASK. This field
qualifies the DR0 linear address data breakpoint, allowing the DR0 data breakpoint on a range of addresses in
        memory. AddrMask[11:0] qualifies the DR0 linear address instruction breakpoint, allowing the DR0 instruction
breakpoint on a range of addresses in memory. DR0[31:12] is only valid for data breakpoints. The legacy DR0
        breakpoint function is provided by DR0[31:0] == 0000_0000h). The mask bits are active high. DR0 is always
        used, and it can be used in conjunction with any debug function that uses DR0.)====="}}},
        {"MSRC001_1030", "IBS Fetch Control", R"=====(Reset: 0000_0000_0000_0000h.
See 2.1.12 [Instruction Based Sampling (IBS)].
The IBS fetch sampling engine is described as follows:
• The periodic fetch counter is an internal 20-bit counter:
• The periodic fetch counter [19:4] is set to IbsFetchCnt[19:4] and the periodic fetch counter [3:0] is set
according to IbsRandEn when IbsFetchEn is changed from 0 to 1.
• It increments for every fetch cycle that completes when IbsFetchEn == 1 and IbsFetchVal == 0.
• The periodic fetch counter is undefined when IbsFetchEn == 0 or IbsFetchVal == 1.
• When IbsFetchCnt[19:4] is read it returns the current value of the periodic fetch counter [19:4].
• When the periodic fetch counter reaches {IbsFetchMaxCnt[19:4],0h} and the selected instruction fetch
completes or is aborted:
• IbsFetchVal is set to 1.
• Drivers can't assume that IbsFetchCnt[19:4] is 0 when IbsFetchVal == 1.
• The status of the operation is written to the IBS fetch registers (this register,
Core::X86::Msr::IBS_FETCH_LINADDR and Core::X86::Msr::IBS_FETCH_PHYSADDR).
• An interrupt is generated as specified by Core::X86::Msr::IBS_CTL. The interrupt service routine associated
        with this interrupt is responsible for saving the performance information stored in IBS execution registers.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011030},
         {{63, 59, R"=====(Reserved.)====="}, {58, 58, R"=====(IbsFetchL2Miss: L2 cache miss for the sampled fetch. Read-only,Volatile. Reset: 0. 1=The instruction fetch
        missed in the L2 Cache. Qualified by (IbsFetchComp == 1).)====="},
          {57, 57, R"=====(IbsRandEn: random instruction fetch tagging enable. Read-write. Reset: 0. 0=Bits[3:0] of the fetch counter
are set to 0h when IbsFetchEn is set to start the fetch counter. 1=Bits[3:0] of the fetch counter are randomized
when IbsFetchEn is set to start the fetch counter.)====="},
          {56, 56, R"=====(IbsL2TlbMiss: instruction cache L2TLB miss. Read-only,Volatile. Reset: 0. 1=The instruction fetch missed in
        the L2 TLB.)====="},
          {55, 55, R"=====(IbsL1TlbMiss: instruction cache L1TLB miss. Read-only,Volatile. Reset: 0. 1=The instruction fetch missed in
        the L1 TLB.)====="},
          {54, 53, R"=====(IbsL1TlbPgSz: instruction cache L1TLB page size. Read-only,Volatile. Reset: 0h. Indicates the page size of the
translation in the L1 TLB. This field is only valid if IbsPhyAddrValid == 1.
ValidValues:
Value       Description
0h          4 KB
1h          2 MB
2h          1 GB
3h          Reserve.)====="},
          {52, 52, R"=====(IbsPhyAddrValid: instruction fetch physical address valid. Read-only,Volatile. Reset: 0. 1=The physical
address in Core::X86::Msr::IBS_FETCH_PHYSADDR and the IbsL1TlbPgSz field are valid for the instruction
fetch.)====="},
          {51, 51, R"=====(IbsIcMiss: instruction cache miss. Read-only,Volatile. Reset: 0. 1=The instruction fetch missed in the
instruction cache.)====="},
          {50, 50, R"=====(IbsFetchComp: instruction fetch complete. Read-only,Volatile. Reset: 0. 1=The instruction fetch completed and
the data is available for use by the instruction decoder.)====="},
          {49, 49, R"=====(IbsFetchVal: instruction fetch valid. Read-only,Volatile. Reset: 0. 1=New instruction fetch data available. When
this bit is set, the fetch counter stops counting and an interrupt is generated as specified by
Core::X86::Msr::IBS_CTL. This bit must be cleared for the fetch counter to start counting. When clearing this bit,
software can write 0000h to IbsFetchCnt[19:4] to start the fetch counter at IbsFetchMaxCnt[19:4].)====="},
          {48, 48, R"=====(IbsFetchEn: instruction fetch enable. Read-write. Reset: 0. 1=Instruction fetch sampling is enabled.)====="},
          {47, 32, R"=====(IbsFetchLat: instruction fetch latency. Read-only,Volatile. Reset: 0000h. Indicates the number of clock cycles
from when the instruction fetch was initiated to when the data was delivered to the core. If the instruction fetch is
        abandoned before the fetch completes, this field returns the number of clock cycles from when the instruction
        fetch was initiated to when the fetch was abandoned.)====="},
          {31, 16, R"=====(IbsFetchCnt[19:4]. Read-write,Volatile. Reset: 0000h. Provides read/write access to bits[19:4] of the periodic
        fetch counter. Programming this field to a value greater than or equal to IbsFetchMaxCnt[19:4] results in
undefined behavior.)====="},
          {15, 0, R"=====(IbsFetchMaxCnt[19:4]. Read-write. Reset: 0000h. Specifies bits[19:4] of the maximum count value of the
        periodic fetch counter. Programming this field to 0000h and setting IbsFetchEn results in undefined behavior.
Bits[3:0] of the maximum count are always 0000b.)====="}}},
        {"MSRC001_1031", "IBS Fetch Linear Address", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011031},
         {{63, 0, R"=====(IbsFetchLinAd: instruction fetch linear address. Read-write,Volatile. Reset: 0000_0000_0000_0000h.
Provides the linear address in canonical form for the tagged instruction fetch.)====="}}},
        {"MSRC001_1032", "IBS Fetch Physical Address", R"=====(Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011032},
         {{63, 48, R"=====(Reserved. Read-only. Reset: Fixed,0000h.)====="}, {47, 0, R"=====(IbsFetchPhysAd: instruction fetch physical address. Read-write,Volatile. Reset: 0000_0000_0000h. Provides
        the physical address for the tagged instruction fetch. The lower 12 bits are not modified by address translation, so
they are always the same as the linear address. This field contains valid data only if
Core::X86::Msr::IBS_FETCH_CTL[IbsPhyAddrValid] is asserted.)====="}}},
        {"MSRC001_1033", "IBS Execution Control", R"=====(Reset: 0000_0000_0000_0000h.
See 2.1.12 [Instruction Based Sampling (IBS)].
The IBS execution sampling engine is described as follows for IbsOpCntCtl == 1. If IbsOpCntCtl == 1n then references
to “periodic op counter" mean “periodic cycle counter".
• The periodic op counter is an internal 27-bit counter:
• It is set to IbsOpCurCnt[26:0] when IbsOpEn is changed from 0 to 1.
• It increments every dispatched op when IbsOpEn == 1 and IbsOpVal == 0.
• The periodic op counter is undefined when IbsOpEn == 0 or IbsOpVal == 1.
• When IbsOpCurCnt[26:0] is read then it returns the current value of the periodic micro-op counter
[26:0].
• When the periodic micro-op counter reaches IbsOpMaxCnt:
• The next dispatched micro-op is tagged if IbsOpCntCtl == 1. A valid op in the next dispatched line is
        tagged if IbsOpCntCtl == 0. See IbsOpCntCtl.
• The periodic micro-op counter [26:7]=0; [6:0] is randomized by hardware.
• The periodic micro-op counter is not modified when a tagged micro-op is flushed.
• When a tagged micro-op is retired:
• IbsOpVal is set to 1.
• Drivers can't assume that IbsOpCurCnt is 0 when IbsOpVal == 1.
• The status of the operation is written to the IBS execution registers (this register, Core::X86::Msr::IBS_OP_RIP,
Core::X86::Msr::IBS_OP_DATA, Core::X86::Msr::IBS_OP_DATA2, Core::X86::Msr::IBS_OP_DATA3,
Core::X86::Msr::IBS_DC_LINADDR and Core::X86::Msr::IBS_DC_PHYSADDR).
• An interrupt is generated as specified by Core::X86::Msr::IBS_CTL. The interrupt service routine associated
        with this interrupt is responsible for saving the performance information stored in IBS execution registers.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011033},
         {{63, 59, R"=====(Reserved.)====="}, {58, 32, R"=====(IbsOpCurCnt[26:0]: periodic op counter current count. Read-write,Volatile. Reset: 000_0000h. Returns the
current value of the periodic op counter.)====="},
          {31, 27, R"=====(Reserved.)====="},
          {26, 20, R"=====(IbsOpMaxCnt[26:20]: periodic op counter maximum count. Read-write. Reset: 00h. See IbsOpMaxCnt[19:4].)====="},
          {19, 19, R"=====(IbsOpCntCtl: periodic op counter count control. Read-write. Reset: 0. 0=Count clock cycles; a 1-of-4 round-
robin counter selects an op in the next dispatch line; if the op pointed to by the round-robin counter is invalid,
        then the next younger valid op is selected. 1=Count dispatched Micro-Ops; when a roll-over occurs, the counter is
preloaded with a pseudorandom 7 bit value between 1 and 127.)====="},
          {18, 18, R"=====(IbsOpVal: micro-op sample valid. Read-write,Volatile. Reset: 0. 1=New instruction execution data available;
the periodic op counter is disabled from counting. An interrupt may be generated when this bit is set as specified
        by Core::X86::Msr::IBS_CTL[LvtOffset].)====="},
          {17, 17, R"=====(IbsOpEn: micro-op sampling enable. Read-write. Reset: 0. 1=Instruction execution sampling enabled.)====="},
          {16, 16, R"=====(Reserved.)====="},
          {15, 0, R"=====(IbsOpMaxCnt[19:4]: periodic op counter maximum count. Read-write. Reset: 0000h. IbsOpMaxCnt[26:0] =
{IbsOpMaxCnt[26:20], IbsOpMaxCnt[19:4], 0000b}. Specifies maximum count value of the periodic op counter.
Bits [3:0] of the maximum count are always 0000b.
ValidValues:
Value     Description
0008h- Reserved.
0000h
FFFFh- <Value> *16 Ops.
0009h)====="}}},
        {"MSRC001_1034", "IBS Op Logical Address", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011034},
         {{63, 0, R"=====(IbsOpRip: micro-op linear address. Read-write,Volatile. Reset: 0000_0000_0000_0000h. Linear address in
        canonical form for the instruction that contains the tagged micro-op.)====="}}},
        {"MSRC001_1035", "IBS Op Data", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011035},
         {{63, 41, R"=====(Reserved.)====="}, {40, 40, R"=====(IbsOpMicrocode. Read-write,Volatile. Reset: 0. 1=Tagged operation from microcode.)====="}, {39, 39, R"=====(IbsOpBrnFuse: fused branch micro-op. Read-write,Volatile. Reset: 0. 1=Tagged operation was a fused branch
micro-op. Support indicated by Core::X86::Cpuid::IbsIdEax[OpBrnFuse].)====="},
          {38, 38, R"=====(IbsRipInvalid: RIP is invalid. Read-write,Volatile. Reset: 0. 1=Tagged operation RIP is invalid. Support
        indicated by Core::X86::Cpuid::IbsIdEax[RipInvalidChk].)====="},
          {37, 37, R"=====(IbsOpBrnRet: branch micro-op retired. Read-write,Volatile. Reset: 0. 1=Tagged operation was a branch micro-
op that retired.)====="},
          {36, 36, R"=====(IbsOpBrnMisp: mispredicted branch micro-op. Read-write,Volatile. Reset: 0. 1=Tagged operation was a
branch micro-op that was mispredicted. Qualified by IbsOpBrnRet == 1.)====="},
          {35, 35, R"=====(IbsOpBrnTaken: taken branch micro-op. Read-write,Volatile. Reset: 0. 1=Tagged operation was a branch
        micro-op that was taken. Qualified by IbsOpBrnRet == 1.)====="},
          {34, 34, R"=====(IbsOpReturn: return micro-op. Read-write,Volatile. Reset: 0. 1=Tagged operation was return micro-op.
Qualified by (IbsOpBrnRet == 1).)====="},
          {33, 32, R"=====(Reserved.)====="},
          {31, 16, R"=====(IbsTagToRetCtr: micro-op tag to retire count. Read-write,Volatile. Reset: 0000h. This field returns the number
        of cycles from when the micro-op was tagged to when the micro-op was retired. This field is equal to
        IbsCompToRetCtr when the tagged micro-op is a NOP.)====="},
          {15, 0, R"=====(IbsCompToRetCtr: micro-op completion to retire count. Read-write,Volatile. Reset: 0000h. This field returns
        the number of cycles from when the micro-op was completed to when the micro-op was retired.)====="}}},
        {"MSRC001_1036", "IBS Op Data 2", R"=====(Reset: 0000_0000_0000_0000h.
Data is only valid for load operations that miss both the L1 data cache and the L2 cache. If a load operation crosses a
cache line boundary, the data returned in this register is the data for the access to the lower cache line.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011036},
         {{63, 6, R"=====(Reserved.)====="}, {5, 5, R"=====(CacheHitSt: IBS cache hit state. Read-write,Volatile. Reset: 0. 0=M State. 1=O State. Valid when the data
source type is Cache(2h).)====="},
          {4, 4, R"=====(RmtNode: IBS request destination node. Read-write,Volatile. Reset: 0. 0=The request is serviced by the NB in
the same node as the core. 1=The request is serviced by the NB in a different node than the core. Valid when
NbIbsReqSrc is non-zero.)====="},
          {3, 3, R"=====(Reserved.)====="},
          {2, 0, R"=====(DataSrc: northbridge IBS request data source. Read-write. Reset: 0h.
ValidValues:
Value    Description
0h       No valid status.
1h          Reserved.
2h          Cache: data returned from another cores cache.
3h          DRAM: data returned from DRAM.
4h          Reserved for remote cache.
6h-5h       Reserved.
7h          Other: data returned from MMIO/Config/PCI/APIC.)====="}}},
        {"MSRC001_1037", "IBS Op Data 3", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
If a load or store operation crosses a 256-bit boundary, the data returned in this register is the data for the access to the
data below the 256-bit boundary.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011037},
         {{63, 48, R"=====(IbsTlbRefillLat: L1 DTLB refill latency. Read-write,Volatile. Reset: 0000h. The number of cycles from when a
        L1 DTLB refill is triggered by a tagged op to when the L1 DTLB fill has been completed.)====="},
          {47, 32, R"=====(IbsDcMissLat: data cache miss latency. Read-write,Volatile. Reset: 0000h. Indicates the number of clock cycles
from when a miss is detected in the data cache to when the data was delivered to the core. The value returned by
this counter is not valid for data cache writes or prefetch instructions.)====="},
          {31, 26, R"=====(IbsOpDcMissOpenMemReqs: outstanding memory requests on DC fill. Read-write,Volatile. Reset: 00h. The
        number of allocated, valid DC MABs when the MAB corresponding to a tagged DC miss op is deallocated.
Includes the MAB allocated by the sampled op. 00000b=No information provided.)====="},
          {25, 22, R"=====(IbsOpMemWidth: load/store size in bytes. Read-write,Volatile. Reset: 0h. Report the number of bytes the load
or store is attempting to access.
ValidValues:
Value      Description
0h         No information provided.
1h         Byte.
2h         Word.
3h         DW.
4h         QW.
5h         OW.
Fh-6h      Reserved.)====="},
          {21, 21, R"=====(IbsSwPf: software prefetch. Read-write,Volatile. Reset: 0. 1=The op is a software prefetch.)====="},
          {20, 20, R"=====(IbsL2Miss: L2 cache miss for the sampled operation. Read-write,Volatile. Reset: 0. 1=The operation missed in
the L2, regardless of whether the op initiated the request to the L2.)====="},
          {19, 19, R"=====(IbsDcL2TlbHit1G: data cache L2TLB hit in 1G page. Read-write,Volatile. Reset: 0. 1=The physical address
for the tagged load or store operation was present in a 1G page table entry in the data cache L2TLB.)====="},
          {18, 18, R"=====(IbsDcPhyAddrValid: data cache physical address valid. Read-write,Volatile. Reset: 0. 1=The physical address
        in Core::X86::Msr::IBS_DC_PHYSADDR is valid for the load or store operation.)====="},
          {17, 17, R"=====(IbsDcLinAddrValid: data cache linear address valid. Read-write,Volatile. Reset: 0. 1=The linear address in
Core::X86::Msr::IBS_DC_LINADDR is valid for the load or store operation.)====="},
          {16, 16, R"=====(DcMissNoMabAlloc: DC miss with no MAB allocated. Read-write,Volatile. Reset: 0. 1=The tagged load or
store operation hit on an already allocated MAB.)====="},
          {15, 15, R"=====(IbsDcLockedOp: locked operation. Read-write,Volatile. Reset: 0. 1=Tagged load or store operation is a locked
        operation.)====="},
          {14, 14, R"=====(IbsDcUcMemAcc: UC memory access. Read-write,Volatile. Reset: 0. 1=Tagged load or store operation accessed
        uncacheable memory.)====="},
          {13, 13, R"=====(IbsDcWcMemAcc: WC memory access. Read-write,Volatile. Reset: 0. 1=Tagged load or store operation
accessed write combining memory.)====="},
          {12, 9, R"=====(Reserved.)====="},
          {8, 8, R"=====(IbsDcMisAcc: misaligned access. Read-write,Volatile. Reset: 0. 1=The tagged load or store operation crosses a
256 bit address boundary.)====="},
          {7, 7, R"=====(IbsDcMiss: data cache miss. Read-write,Volatile. Reset: 0. 1=The cache line used by the tagged load or store
        was not present in the data cache.)====="},
          {6, 6, R"=====(IbsDcL2tlbHit2M: data cache L2TLB hit in 2M page. Read-write,Volatile. Reset: 0. 1=The physical address
for the tagged load or store operation was present in a 2M page table entry in the data cache L2TLB.)====="},
          {5, 5, R"=====(IbsDcL1TlbHit1G: data cache L1TLB hit in 1G page. Read-write,Volatile. Reset: 0. 1=The physical address
for the tagged load or store operation was present in a 1G page table entry in the data cache L1TLB.)====="},
          {4, 4, R"=====(IbsDcL1TlbHit2M: data cache L1TLB hit in 2M page. Read-write,Volatile. Reset: 0. 1=The physical address
for the tagged load or store operation was present in a 2M page table entry in the data cache L1TLB.)====="},
          {3, 3, R"=====(IbsDcL2TlbMiss: data cache L2TLB miss. Read-write,Volatile. Reset: 0. 1=The physical address for the tagged
load or store operation was not present in the data cache L2TLB.)====="},
          {2, 2, R"=====(IbsDcL1tlbMiss: data cache L1TLB miss. Read-write,Volatile. Reset: 0. 1=The physical address for the tagged
load or store operation was not present in the data cache L1TLB.)====="},
          {1, 1, R"=====(IbsStOp: store op. Read-write,Volatile. Reset: 0. 1=Tagged operation is a store operation.)====="},
          {0, 0, R"=====(IbsLdOp: load op. Read-write,Volatile. Reset: 0. 1=Tagged operation is a load operation.)====="}}},
        {"MSRC001_1038", "IBS DC Linear Address", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011038},
         {{63, 0, R"=====(IbsDcLinAd. Read-write,Volatile. Reset: 0000_0000_0000_0000h. Provides the linear address in canonical form
for the tagged load or store operation. This field contains valid data only if
Core::X86::Msr::IBS_OP_DATA3[IbsDcLinAddrValid] is asserted.)====="}}},
        {"MSRC001_1039", "IBS DC Physical Address", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC0011039},
         {{63, 48, R"=====(Reserved.)====="}, {47, 0, R"=====(IbsDcPhysAd: load or store physical address. Read-write,Volatile. Reset: 0000_0000_0000h. Provides the
physical address for the tagged load or store operation. The lower 12 bits are not modified by address translation,
        so they are always the same as the linear address. This field contains valid data only if
Core::X86::Msr::IBS_OP_DATA3[IbsDcPhyAddrValid] is asserted.)====="}}},
        {"MSRC001_103A", "IBS Control", R"=====(Read,Error-on-write. Reset: 0000_0000_0000_0X0Xh.)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001103A},
         {{63, 9, R"=====(Reserved.)====="}, {8, 8, R"=====(LvtOffsetVal: local vector table offset valid. Read,Error-on-write. Reset: X.)====="}, {7, 4, R"=====(Reserved.)====="}, {3, 0, R"=====(LvtOffset: local vector table offset. Read,Error-on-write. Reset: Xh.)====="}}},
        {"MSRC001_103B", "IBS Branch Target Address", R"=====(Read-write,Volatile. Reset: 0000_0000_0000_0000h.
Support for this register indicated by Core::X86::Cpuid::IbsIdEax[BrnTrgt].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001103B},
         {{63, 0, R"=====(IbsBrTarget. Read-write,Volatile. Reset: 0000_0000_0000_0000h. The logical address in canonical form for the
        branch target. Contains a valid target if non-0. Qualified by Core::X86::Msr::IBS_OP_DATA[IbsOpBrnRet] == 1.)====="}}},
        {"MSRC001_103C", "IBS Fetch Control Extended", R"=====(Read-only,Volatile. Reset: 0000_0000_0000_0000h.
Support for this register indicated by Core::X86::Cpuid::IbsIdEax[IbsFetchCtlExtd].)=====",
         "_lthree[1:0]_core[3:0]_thread[1:0]; ",
         {0xC001103C},
         {{63, 16, R"=====(Reserved.)====="}, {15, 0, R"=====(IbsItlbRefillLat: ITLB Refill Latency for the sampled fetch, if there is a reload. Read-only,Volatile. Reset:
0000h. The number of cycles when the fetch engine is stalled for an ITLB reload for the sampled fetch. If there is no reload, the latency is 0.)====="}}}};

std::ostream &operator<<(std::ostream &os, const MSRRegister &m) {

    os << "Number of registers : " << MSRRegister::Registers.size() << std::endl;

    for (auto &r : MSRRegister::Registers) {
        printf("%-50s", r.name.c_str());

        for(auto &o : r.offsets){
            printf(" 0x%08lX", o);
        }
        os << std::endl;

        for(auto &v : r.values) {
            printf("  %02zi %02zi 0x%016lX %s\n", v.start, v.end, v.mask, v.getName().c_str());
        }
    }

    return os;
}

void MSRRegister::open() {
    bool msrFound = false;
    for (;;) {
        std::string fileName = "/dev/cpu/" + std::to_string(numOfCPU) + "/msr";

        if (access(fileName.c_str(), R_OK) == -1)
            break;
        else
            numOfCPU++;
    }

    if(numOfCPU == 0)
        throw MSRException("Unable to find any SMR file");
    else
        printf("found %u CPU SMR\n", numOfCPU);

    filesStreams.resize(numOfCPU);

    for (int i = 0; i < numOfCPU; i++) {
        std::string fileName = "/dev/cpu/" + std::to_string(i) + "/msr";
        filesStreams[i] = ::open(fileName.c_str(), O_RDONLY);
        if (filesStreams[i] < 0) {
            throw MSRException("Unable to open the file " + fileName + " : " + +strerror(errno));
        }
    }
}

uint64_t MSRRegister::read(int cpu, unsigned int addr) {

    uint64_t data;
    auto r = pread(filesStreams[cpu], &data, sizeof data, addr);
    if(r == -1)
        throw MSRException(std::string("Unable to read value : ")  + strerror(errno));

    if (r != sizeof data) {
        printf("Warning : not the full size has been read\n");
    }

    return data;
}

void MSRRegister::readSMR() {
    std::cout << MSR::RAPL_PWR_UNIT << std::endl;
    open();

    uint32_t core_energy_raw;
    int package_raw;
    // Read per core energy values
    std::cout << Registers[MSR::CORE_ENERGY_STAT].name;
    for (int i = 0; i < numOfCPU; i++) {
        //printf("%i %lx\n", i, Registers[MSR::CORE_ENERGY_STAT].offsets[0]);
        std::cout << "    " << (uint32_t) read(i, Registers[MSR::CORE_ENERGY_STAT].offsets[0]);
    }

    std::cout << std::endl << Registers[MSR::PKG_ENERGY_STAT].name;
    for (int i = 0; i < numOfCPU; i++)
        std::cout << "    " << (uint32_t)read(i, Registers[MSR::PKG_ENERGY_STAT].offsets[0]);

    std::cout << std::endl;
}

MSRRegister::Value::Value(size_t start, size_t end, std::string description) : start(start), end(end), description(std::move(description)) {
    if(start == 63 && end == 0)
        mask = 0xFFFFFFFFFFFFFFFF;
    else if(start != end) {
        mask = std::pow(2, (start - end) + 1) - 1;
    }
}
std::string MSRRegister::Value::getName() const {
    auto pos = description.find_first_of("\n.:[");
    return description.substr(0, pos);
}
