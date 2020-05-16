#include <CPUIDRegister.hpp>

/*
 * (.*)\s\[(.*)\]\s\((Core::X86::Cpuid::.*)\)\s(.*\s)*?(_.*);\s(.*)\s*Bits\s*Description
 *)====="}}}, {"$1", "$2", "$3", R"(=====$4=====)", "$5", "$6", {{
 *
 * For the bits description
 * - ($\s*(\d{1,2}):(\d{1,2})\s) --> \n$2, $3, R"=====(* - ($\s*(\d{1,2})\s) --> \n$2, $2, R"=====(* - ([^\{])\s(\d{1,2}, \d{1,2}) -->)====="},\n{$2
 */

std::vector<CPUIDRegister::Register> CPUIDRegister::Registers = {
        {"CPUID_Fn00000000_EAX", "Processor Vendor and Largest Standard Function Number", "Core::X86::Cpuid::LargFuncNum", R"(=====Read-only. Reset: 0000_000Dh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000000_EAX",
         {{31, 0, R"=====(LFuncStd: largest standard function. Read-only. Reset: Fixed,0000_000Dh. The largest CPUID standard
function input value supported by the processor implementation.)====="}}},
        {"CPUID_Fn00000000_EBX", "Processor Vendor (ASCII Bytes3:0])", "Core::X86::Cpuid::ProcVendEbx", R"(=====Core::X86::Cpuid::ProcVendEbx and Core::X86::Cpuid::ProcVendExtEbx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000000_EBX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,6874_7541h. ASCII Bytes [3:0] (“h t u A") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn00000000_ECX", "Processor Vendor (ASCII Bytes11:8])", "Core::X86::Cpuid::ProcVendEcx", R"(=====Core::X86::Cpuid::ProcVendEcx and Core::X86::Cpuid::ProcVendExtEcx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000000_ECX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,444D_4163h. ASCII Bytes [11:8] (“D M A c") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn00000000_EDX", "Processor Vendor (ASCII Bytes7:4])", "Core::X86::Cpuid::ProcVendEdx", R"(=====Core::X86::Cpuid::ProcVendEdx and Core::X86::Cpuid::ProcVendExtEdx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000000_EDX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,6974_6E65h. ASCII Bytes [7:4] (“i t n e") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn00000001_EAX", "Family, Model, Stepping Identifiers", "Core::X86::Cpuid::FamModStep", R"(=====Model numbers are are assigned a letter, 0h = "A", 1h = "B", and so on. Model and Stepping form the Revision. E.g., B2.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000001_EAX",
         {{31, 28, R"=====(Reserved)====="}, {27, 20, R"=====(ExtFamily: extended family. Read-only. Reset: 08h)====="}, {19, 16, R"=====(ExtModel: extended model. Read-only. Reset: 0h)====="}, {15, 12, R"=====(Reserved)====="}, {11, 8, R"=====(BaseFamily. Read-only. Reset: Fh)====="}, {7, 4, R"=====(BaseModel. Read-only. Reset: Xh. Model numbers vary with product)====="}, {3, 0, R"=====(Stepping. Read-only. Reset: 2h.)====="}}},
        {"CPUID_Fn00000001_EBX", "LocalApicId, LogicalProcessorCount, CLFlush", "Core::X86::Cpuid::FeatureIdEbx", R"(=====Read-only.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000001_EBX",
         {{31, 24, R"=====(LocalApicId: initial local APIC physical ID. Read-only. Reset: XXh)====="}, {23, 16, R"=====(LogicalProcessorCount: logical processor count. Read-only. Reset: Fixed,(Core::X86::Cpuid::SizeId[NC] + 1).
Specifies the number of threads in the processor as Core::X86::Cpuid::SizeId[NC]+1)====="},
          {15, 8, R"=====(CLFlush: CLFLUSH size in quadwords. Read-only. Reset: Fixed,08h)====="},
          {7, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn00000001_ECX", "Feature Identifiers", "Core::X86::Cpuid::FeatureIdEcx", R"(=====These values can be over-written by Core::X86::Msr::CPUID_Features.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000001_ECX",
         {{31, 31, R"=====(Reserved. Read-only. Reset: Fixed,0. Reserved for use by hypervisor to indicate guest status)====="}, {30, 30, R"=====(RDRAND: RDRAND instruction support. Read-only. Reset: Fixed,1)====="}, {29, 29, R"=====(F16C: half-precision convert instruction support. Read-only. Reset: Fixed,1)====="}, {28, 28, R"=====(AVX: AVX instruction support. Read-only. Reset: Fixed,1)====="}, {27, 27, R"=====(OSXSAVE: OS enabled support for XGETBV/XSETBV. Read-only. Reset: X. 1=The OS has enabled support
for XGETBV/XSETBV instructions to query processor extended states)====="},
          {26, 26, R"=====(XSAVE: XSAVE (and related) instruction support. Read-only. Reset: Fixed,1. 1=Support provided for the
        XSAVE, XRSTOR, XSETBV, and XGETBV instructions and the XFEATURE_ENABLED_MASK register)====="},
          {25, 25, R"=====(AES: AES instruction support. Read-only. Reset: X)====="},
          {24, 24, R"=====(Reserved)====="},
          {23, 23, R"=====(POPCNT: POPCNT instruction. Read-only. Reset: Fixed,1)====="},
          {22, 22, R"=====(MOVBE: MOVBE instruction support. Read-only. Reset: Fixed,1)====="},
          {21, 21, R"=====(X2APIC: x2APIC capability. Read-only. Reset: Fixed,0)====="},
          {20, 20, R"=====(SSE42: SSE4.2 instruction support. Read-only. Reset: Fixed,1)====="},
          {19, 19, R"=====(SSE41: SSE4.1 instruction support. Read-only. Reset: Fixed,1)====="},
          {18, 18, R"=====(Reserved)====="},
          {17, 17, R"=====(PCID: process context identifiers support. Read-only. Reset: Fixed,0)====="},
          {16, 14, R"=====(Reserved)====="},
          {13, 13, R"=====(CMPXCHG16B: CMPXCHG16B instruction. Read-only. Reset: Fixed,1)====="},
          {12, 12, R"=====(FMA: FMA instruction support. Read-only. Reset: Fixed,1)====="},
          {11, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(SSSE3: supplemental SSE3 extensions. Read-only. Reset: Fixed,1)====="},
          {8, 4, R"=====(Reserved)====="},
          {3, 3, R"=====(Monitor: Monitor/Mwait instructions. Read-only. Reset: !Core::X86::Msr::HWCR[MonMwaitDis])====="},
          {2, 2, R"=====(Reserved)====="},
          {1, 1, R"=====(PCLMULQDQ: PCLMULQDQ instruction support. Read-only. Reset: X)====="},
          {0, 0, R"=====(SSE3: SSE3 extensions. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn00000001_EDX", "Feature Identifiers", "Core::X86::Cpuid::FeatureIdEdx", R"(=====These values can be over-written by Core::X86::Msr::CPUID_Features.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000001_EDX",
         {{31, 29, R"=====(Reserved)====="}, {28, 28, R"=====(HTT: hyper-threading technology. Read-only. Reset: Fixed,(Core::X86::Cpuid::SizeId[NC] != 0). 0=Single
        thread product (Core::X86::Cpuid::SizeId[NC]==0). 1=Multi thread product (Core::X86::Cpuid::SizeId[NC] !=
0))====="},
          {27, 27, R"=====(Reserved)====="},
          {26, 26, R"=====(SSE2: SSE2 extensions. Read-only. Reset: Fixed,1)====="},
          {25, 25, R"=====(SSE: SSE extensions. Read-only. Reset: Fixed,1)====="},
          {24, 24, R"=====(FXSR: FXSAVE and FXRSTOR instructions. Read-only. Reset: Fixed,1)====="},
          {23, 23, R"=====(MMX: MMX™ instructions. Read-only. Reset: Fixed,1)====="},
          {22, 20, R"=====(Reserved)====="},
          {19, 19, R"=====(CLFSH: CLFLUSH instruction. Read-only. Reset: Fixed,1)====="},
          {18, 18, R"=====(Reserved)====="},
          {17, 17, R"=====(PSE36: page-size extensions. Read-only. Reset: Fixed,1)====="},
          {16, 16, R"=====(PAT: page attribute table. Read-only. Reset: Fixed,1)====="},
          {15, 15, R"=====(CMOV: conditional move instructions, CMOV, FCOMI, FCMOV. Read-only. Reset: Fixed,1)====="},
          {14, 14, R"=====(MCA: machine check architecture, MCG_CAP. Read-only. Reset: Fixed,1)====="},
          {13, 13, R"=====(PGE: page global extension, CR4.PGE. Read-only. Reset: Fixed,1)====="},
          {12, 12, R"=====(MTRR: memory-type range registers. Read-only. Reset: Fixed,1)====="},
          {11, 11, R"=====(SysEnterSysExit: SYSENTER and SYSEXIT instructions. Read-only. Reset: Fixed,1)====="},
          {10, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(APIC: advanced programmable interrupt controller (APIC) exists and is enabled. Read-only. Reset: X.
Core::X86::Msr::APIC_BAR[ApicEn])====="},
          {8, 8, R"=====(CMPXCHG8B: CMPXCHG8B instruction. Read-only. Reset: Fixed,1)====="},
          {7, 7, R"=====(MCE: machine check exception, CR4.MCE. Read-only. Reset: Fixed,1)====="},
          {6, 6, R"=====(PAE: physical-address extensions (PAE). Read-only. Reset: Fixed,1)====="},
          {5, 5, R"=====(MSR: AMD model-specific registers (MSRs), with RDMSR and WRMSR instructions. Read-only. Reset:
Fixed,1)====="},
          {4, 4, R"=====(TSC: time stamp counter, RDTSC/RDTSCP instructions, CR4.TSD. Read-only. Reset: Fixed,1)====="},
          {3, 3, R"=====(PSE: page-size extensions (4 MB pages). Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(DE: debugging extensions, IO breakpoints, CR4.DE. Read-only. Reset: Fixed,1)====="},
          {1, 1, R"=====(VME: virtual-mode enhancements. Read-only. Reset: Fixed,1)====="},
          {0, 0, R"=====(FPU: x87 floating point unit on-chip. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn00000005_EAX", "Monitor/MWait", "Core::X86::Cpuid::MonMWaitEax", R"(=====Read-only. Reset: 0000_0040h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000005_EAX",
         {{31, 16, R"=====(Reserved)====="}, {15, 0, R"=====(MonLineSizeMin: smallest monitor-line size in bytes. Read-only. Reset: Fixed,0040h.)====="}}},
        {"CPUID_Fn00000005_EBX", "Monitor/MWait", "Core::X86::Cpuid::MonMWaitEbx", R"(=====Read-only. Reset: 0000_0040h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000005_EBX",
         {{31, 16, R"=====(Reserved)====="}, {15, 0, R"=====(MonLineSizeMax: largest monitor-line size in bytes. Read-only. Reset: Fixed,0040h.)====="}}},
        {"CPUID_Fn00000005_ECX", "Monitor/MWait", "Core::X86::Cpuid::MonMWaitEcx", R"(=====Read-only. Reset: 0000_0003h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000005_ECX",
         {{31, 2, R"=====(Reserved)====="}, {1, 1, R"=====(IBE: interrupt break-event. Read-only. Reset: Fixed,1)====="}, {0, 0, R"=====(EMX: enumerate MONITOR/MWAIT extensions. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn00000005_EDX", "Monitor/MWait", "Core::X86::Cpuid::MonMWaitEdx", R"(=====Read-only. Reset: 0000_0011h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000005_EDX",
         {{31, 8, R"=====(Reserved)====="}, {7, 4, R"=====(MWaitC1SubStates: number of C1 sub-cstates supported by MWAIT. Read-only. Reset: Fixed,1h)====="}, {3, 0, R"=====(MWaitC0SubStates: number of C0 sub-cstates supported by MWAIT. Read-only. Reset: Fixed,1h.)====="}}},
        {"CPUID_Fn00000006_EAX", "Thermal and Power Management", "Core::X86::Cpuid::ThermalPwrMgmtEax", R"(=====Read-only. Reset: 0000_0004h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000006_EAX",
         {{31, 3, R"=====(Reserved)====="}, {2, 2, R"=====(ARAT: always running APIC timer. Read-only. Reset: Fixed,1. 1=Indicates support for APIC timer always
        running feature)====="},
          {1, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn00000006_EBX", "Thermal and Power Management", "Core::X86::Cpuid::ThermalPwrMgmtEbx", R"(==========)", "_lthree[1:0]_core[3:0]_thread[1:0]", "CPUID_Fn00000006_EBX", {{31, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn00000006_ECX", "Thermal and Power Management", "Core::X86::Cpuid::ThermalPwrMgmtEcx", R"(=====These values can be over-written by Core::X86::Msr::CPUID_PWR_THERM.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000006_ECX",
         {{31, 1, R"=====(Reserved)====="}, {0, 0, R"=====(EffFreq: effective frequency interface. Read-only. Reset: Fixed,1. 1=Indicates presence of
        Core::X86::Msr::MPERF and Core::X86::Msr::APERF.)====="}}},
        {"CPUID_Fn00000006_EDX", "Thermal and Power Management", "Core::X86::Cpuid::ThermalPwrMgmtEdx", R"(==========)", "_lthree[1:0]_core[3:0]_thread[1:0]", "CPUID_Fn00000006_EDX", {{31, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn00000007_EAX_x00", "Structured Extended Feature Identifiers", "Core::X86::Cpuid::StructExtFeatIdEax0", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000007_EAX_x00",
         {{31, 0, R"=====(StructExtFeatIdMax. Read-only. Reset: Fixed,0000_0000h. Valid ECX sub-leaf values starting from 0.)====="}}},
        {"CPUID_Fn00000007_EBX_x00", "Structured Extended Feature Identifiers", "Core::X86::Cpuid::StructExtFeatIdEbx0", R"(=====Read-only. Reset: 209C_01A9h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000007_EBX_x00",
         {{31, 30, R"=====(Reserved)====="}, {29, 29, R"=====(SHA. Read-only. Reset: Fixed,1)====="}, {28, 24, R"=====(Reserved)====="}, {23, 23, R"=====(CLFSHOPT. Read-only. Reset: Fixed,1)====="}, {22, 21, R"=====(Reserved)====="}, {20, 20, R"=====(SMAP: Secure Mode Access Prevention is supported. Read-only. Reset: Fixed,1)====="}, {19, 19, R"=====(ADX: ADCX and ADOX are present. Read-only. Reset: Fixed,1)====="}, {18, 18, R"=====(RDSEED: RDSEED is present. Read-only. Reset: Fixed,1)====="}, {17, 9, R"=====(Reserved)====="}, {8, 8, R"=====(BMI2: bit manipulation group 2 instruction support. Read-only. Reset: Fixed,1)====="}, {7, 7, R"=====(SMEP: Supervisor Mode Execution protection. Read-only. Reset: Fixed,1)====="}, {6, 6, R"=====(Reserved)====="}, {5, 5, R"=====(AVX2: AVX extension support. Read-only. Reset: Fixed,1)====="}, {4, 4, R"=====(Reserved)====="}, {3, 3, R"=====(BMI1: bit manipulation group 1 instruction support. Read-only. Reset: Fixed,1)====="}, {2, 1, R"=====(Reserved)====="}, {0, 0, R"=====(FSGSBASE: FS and GS base read write instruction support. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn00000007_ECX_x00", "Structured Extended Feature Identifier", "Core::X86::Cpuid::StructExtFeatIdEcx0", R"(==========)", "_lthree[1:0]_core[3:0]_thread[1:0]", "CPUID_Fn00000007_ECX_x00", {{31, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn00000007_EDX_x00", "Structured Extended Feature Identifiers", "Core::X86::Cpuid::StructExtFeatIdEdx0", R"(=====
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn00000007_EDX_x00",
         {{31, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn0000000B_EAX", "Extended Topology Enumeration", "Core::X86::Cpuid::ExtTopEnumEax", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000B_EAX",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000B_EBX", "Extended Topology Enumeration", "Core::X86::Cpuid::ExtTopEnumEbx", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000B_EBX",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000B_ECX", "Extended Topology Enumeration", "Core::X86::Cpuid::ExtTopEnumEcx", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000B_ECX",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000B_EDX", "Extended Topology Enumeration", "Core::X86::Cpuid::ExtTopEnumEdx", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000B_EDX",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000D_EAX_x00", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEax00", R"(=====Read-only. Reset: 0000_0007h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EAX_x00",
         {{31, 0, R"=====(XFeatureSupportedMask[31:0]. Read-only. Reset: Fixed,0000_0007h.
ValidValues:
Bit       Name Description
[0]       X86  X87 Support.
[1]       SSE  128-bit SSE Support.
[2]       AVX 256-bit AVX support.
[31:3]         Reserved.)====="}}},
        {"CPUID_Fn0000000D_EBX_x00", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEbx00", R"(=====Read-only,Volatile. Reset: XXXX_XXXXh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EBX_x00",
         {{31, 0, R"=====(XFeatureEnabledSizeMax. Read-only,Volatile. Reset: XXXX_XXXXh.
ValidValues:
Value     Description
0000_0 Reserved.
23Fh-
0000_0
000h
0000_0 legacy header + FPU/SSE size; (XCR0[AVX]==0)
240h
0000_0 Reserved.
33Fh-
0000_0
241h
0000_0 legacy header + FPU/SSE + AVX size; (XCR0[AVX]==1)
340h
FFFF_F Reserved.
FFFh-
0000_0
341h)====="}}},
        {"CPUID_Fn0000000D_ECX_x00", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEcx00", R"(=====Read-only. Reset: XXXX_XXXXh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_ECX_x00",
         {{31, 0, R"=====(XFeatureSupportedSizeMax. Read-only. Reset: Fixed,XXXX_XXXXh. Size in bytes of XSAVE/XRSTOR area
for all features that the core supports. See
        Core::X86::Cpuid::ProcExtStateEnumEbx00[XFeatureEnabledSizeMax].
ValidValues:
Value      Description
0000_0 Reserved.
23Fh-
0000_0
000h
0000_0 legacy header + FPU/SSE size; (XCR0[AVX]==0)
240h
0000_0 Reserved.
33Fh-
0000_0
241h
0000_0 legacy header + FPU/SSE + AVX size; (XCR0[AVX]==1)
340h
FFFF_F Reserved.
FFFh-
0000_0
341h)====="}}},
        {"CPUID_Fn0000000D_EDX_x00", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEdx00", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EDX_x00",
         {{31, 0, R"=====(XFeatureSupportedMask[63:32]. Read-only. Reset: Fixed,0000_0000h. Upper portion of
        XFeatureSupportedMask.)====="}}},
        {"CPUID_Fn0000000D_EAX_x01", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEax01", R"(=====Read-only. Reset: 0000_000Fh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EAX_x01",
         {{31, 4, R"=====(Reserved)====="}, {3, 3, R"=====(XSAVES. Read-only. Reset: Fixed,1. XSAVES,XRSTORS, and IA32_XSS supported)====="}, {2, 2, R"=====(XGETBV. Read-only. Reset: Fixed,1. XGETBV with ECX=1 supported)====="}, {1, 1, R"=====(XSAVEC. Read-only. Reset: Fixed,1. XSAVEC and compact XRSTOR supported)====="}, {0, 0, R"=====(XSAVEOPT: XSAVEOPT is available. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn0000000D_EBX_x01", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEbx01", R"(=====Read-only,Volatile. Reset: XXXX_XXXXh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EBX_x01",
         {{31, 0, R"=====(XFeatureEnabledSizeMax. Read-only,Volatile. Reset: XXXX_XXXXh. Reset is 512 + ((XCR0[AVX]) ? 256 :
0).
ValidValues:
Value      Description
0000_0     Reserved.
23Fh-
0000_0
000h
0000_0     legacy header + FPU/SSE size; (XCR0[AVX]==0)
240h
0000_0     Reserved.
33Fh-
0000_0
241h
0000_0     legacy header + FPU/SSE + AVX size; (XCR0[AVX]==1)
340h
FFFF_F     Reserved.
FFFh-
0000_0
341h)====="}}},
        {"CPUID_Fn0000000D_ECX_x01", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEcx01", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_ECX_x01",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000D_EDX_x01", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEdx01", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EDX_x01",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000D_EAX_x02", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEax02", R"(=====Read-only. Reset: 0000_0100h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EAX_x02",
         {{31, 0, R"=====(YmmSaveStateSize: YMM save state byte size. Read-only. Reset: Fixed,0000_0100h.)====="}}},
        {"CPUID_Fn0000000D_EBX_x02", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEbx02", R"(=====Read-only. Reset: 0000_0240h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EBX_x02",
         {{31, 0, R"=====(YmmSaveStateOffset: YMM save state byte offset. Read-only. Reset: Fixed,0000_0240h.)====="}}},
        {"CPUID_Fn0000000D_ECX_x02", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEcx02", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_ECX_x02",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn0000000D_EDX_x02", "Processor Extended State Enumeration", "Core::X86::Cpuid::ProcExtStateEnumEdx02", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn0000000D_EDX_x02",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn80000000_EAX", "Largest Extended Function Number", "Core::X86::Cpuid::LargExtFuncNum", R"(=====Read-only. Reset: 8000_001Fh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000000_EAX",
         {{31, 0, R"=====(LFuncExt: largest extended function. Read-only. Reset: Fixed,8000_001Fh. The largest CPUID extended
function input value supported by the processor implementation.)====="}}},
        {"CPUID_Fn80000000_EBX", "Processor Vendor (ASCII Bytes3:0])", "Core::X86::Cpuid::ProcVendExtEbx", R"(=====Core::X86::Cpuid::ProcVendEbx and Core::X86::Cpuid::ProcVendExtEbx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000000_EBX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,6874_7541h. ASCII Bytes [3:0] (“h t u A") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000000_ECX", "Processor Vendor (ASCII Bytes11:8])", "Core::X86::Cpuid::ProcVendExtEcx", R"(=====Core::X86::Cpuid::ProcVendEcx and Core::X86::Cpuid::ProcVendExtEcx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000000_ECX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,444D_4163h. ASCII Bytes [11:8] (“D M A c") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000000_EDX", "Processor Vendor (ASCII Bytes7:4])", "Core::X86::Cpuid::ProcVendExtEdx", R"(=====Core::X86::Cpuid::ProcVendEdx and Core::X86::Cpuid::ProcVendExtEdx return the same value.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000000_EDX",
         {{31, 0, R"=====(Vendor. Read-only. Reset: Fixed,6974_6E65h. ASCII Bytes [7:4] (“i t n e") of the string “AuthenticAMD".)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000001_EAX", "Family, Model, Stepping Identifiers", "Core::X86::Cpuid::FamModStepExt", R"(=====        Core::X86::Cpuid::FamModStep.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000001_EAX",
         {{31, 28, R"=====(Reserved)====="}, {27, 20, R"=====(ExtFamily: extended family. Read-only. Reset: 08h)====="}, {19, 16, R"=====(ExtModel: extended model. Read-only. Reset: 0h)====="}, {15, 12, R"=====(Reserved)====="}, {11, 8, R"=====(BaseFamily. Read-only. Reset: Fh)====="}, {7, 4, R"=====(BaseModel. Read-only. Reset: Xh. Model numbers vary with product)====="}, {3, 0, R"=====(Stepping. Read-only. Reset: 2h.)====="}}},
        {"CPUID_Fn80000001_EBX", "BrandId Identifier", "Core::X86::Cpuid::BrandId", R"(=====Read-only. Reset: X000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000001_EBX",
         {{31, 28, R"=====(PkgType: package type. Read-only. Reset: Xh. Specifies the package type.
ValidValues:
Value     Description
1h-0h     Reserved.
2h        AM4
        Fh-3h     Reserved)====="},
          {27, 0, R"=====(Reserved.)====="}}},
        {"CPUID_Fn80000001_ECX", "Feature Identifiers", "Core::X86::Cpuid::FeatureExtIdEcx", R"(=====These values can be over-written by Core::X86::Msr::CPUID_ExtFeatures.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000001_ECX",
         {{31, 30, R"=====(Reserved)====="}, {29, 29, R"=====(MwaitExtended. Read-only. Reset: !Core::X86::Msr::HWCR[MonMwaitDis]. 1=MWAITX and MONITORX
capability is supported)====="},
          {28, 28, R"=====(PerfCtrExtLLC: Last Level Cache performance counter extensions. Read-only. Reset: Fixed,1. 1=Indicates
        support for Core::X86::Msr::ChL3PmcCfg and Core::X86::Msr::ChL3Pmc L3 performance counter extensions.
L3 performance counter extensions support. See 2.1.11.4 [L3 Cache Performance Monitor Counters] and 2.1.11
[Performance Monitor Counters])====="},
          {27, 27, R"=====(PerfTsc: performance time-stamp counter supported. Read-only. Reset: Fixed,0)====="},
          {26, 26, R"=====(DataBreakpointExtension. Read-only. Reset: Fixed,1. 1=Indicates data breakpoint support for
Core::X86::Msr::DR0_ADDR_MASK, Core::X86::Msr::DR1_ADDR_MASK,
Core::X86::Msr::DR2_ADDR_MASK and Core::X86::Msr::DR3_ADDR_MASK)====="},
          {25, 24, R"=====(Reserved)====="},
          {23, 23, R"=====(PerfCtrExtCore: core performance counter extensions support. Read-only. Reset: Fixed,1. 1=Indicates
        support for Core::X86::Msr::PERF_CTL and Core::X86::Msr::PERF_CTR. See See 2.1.11.3 [Core Performance
Monitor Counters] and 2.1.11 [Performance Monitor Counters])====="},
          {22, 22, R"=====(TopologyExtensions: topology extensions support. Read-only. Reset: Fixed,1. 1=Indicates support for
Core::X86::Cpuid::CachePropEax0 and Core::X86::Cpuid::ExtApicId)====="},
          {21, 18, R"=====(Reserved)====="},
          {17, 17, R"=====(TCE: translation cache extension. Read-only. Reset: Fixed,1)====="},
          {16, 16, R"=====(FMA4: 4-operand FMA instruction support. Read-only. Reset: Fixed,0)====="},
          {15, 15, R"=====(LWP: lightweight profiling support. Read-only. Reset: Fixed,0)====="},
          {14, 14, R"=====(Reserved)====="},
          {13, 13, R"=====(WDT: watchdog timer support. Read-only. Reset: Fixed,1)====="},
          {12, 12, R"=====(SKINIT: SKINIT and STGI support. Read-only. Reset: Fixed,1)====="},
          {11, 11, R"=====(XOP: extended operation support. Read-only. Reset: Fixed,0)====="},
          {10, 10, R"=====(IBS: Instruction Based Sampling. Read-only. Reset: 0)====="},
          {9, 9, R"=====(OSVW: OS Visible Work-around support. Read-only. Reset: Fixed,1)====="},
          {8, 8, R"=====(ThreeDNowPrefetch: Prefetch and PrefetchW instructions. Read-only. Reset: Fixed,1)====="},
          {7, 7, R"=====(MisAlignSse: Misaligned SSE Mode. Read-only. Reset: Fixed,1)====="},
          {6, 6, R"=====(SSE4A: EXTRQ, INSERTQ, MOVNTSS, and MOVNTSD instruction support. Read-only. Reset: Fixed,1)====="},
          {5, 5, R"=====(ABM: advanced bit manipulation. Read-only. Reset: Fixed,1. LZCNT instruction support)====="},
          {4, 4, R"=====(AltMovCr8: LOCK MOV CR0 means MOV CR8. Read-only. Reset: Fixed,1)====="},
          {3, 3, R"=====(ExtApicSpace: extended APIC register space. Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(SVM: Secure Virtual Mode feature. Read-only. Reset: Fixed,1. Indicates support for: VMRUN, VMLOAD,
VMSAVE, CLGI, VMMCALL, and INVLPGA)====="},
          {1, 1, R"=====(CmpLegacy: core multi-processing legacy mode. Read-only. Reset: Fixed,(Core::X86::Cpuid::SizeId[NC] > 0).
0=Single core product (Core::X86::Cpuid::SizeId[NC] == 0). 1=Multi core product
(Core::X86::Cpuid::SizeId[NC] !=0 ))====="},
          {0, 0, R"=====(LahfSahf: LAHF/SAHF instructions. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn80000001_EDX", "Feature Identifiers", "Core::X86::Cpuid::FeatureExtIdEdx", R"(=====These values can be over-written by Core::X86::Msr::CPUID_ExtFeatures.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000001_EDX",
         {{31, 31, R"=====(ThreeDNow: 3DNow!™ instructions. Read-only. Reset: Fixed,0)====="}, {30, 30, R"=====(ThreeDNowExt: AMD extensions to 3DNow!™ instructions. Read-only. Reset: Fixed,0)====="}, {29, 29, R"=====(LM: long mode. Read-only. Reset: Fixed,1)====="}, {28, 28, R"=====(Reserved)====="}, {27, 27, R"=====(RDTSCP: RDTSCP instruction. Read-only. Reset: Fixed,1)====="}, {26, 26, R"=====(Page1GB: one GB large page support. Read-only. Reset: Fixed,1)====="}, {25, 25, R"=====(FFXSR: FXSAVE and FXRSTOR instruction optimizations. Read-only. Reset: Fixed,1)====="}, {24, 24, R"=====(FXSR: FXSAVE and FXRSTOR instructions. Read-only. Reset: Fixed,1)====="}, {23, 23, R"=====(MMX: MMX™ instructions. Read-only. Reset: Fixed,1)====="}, {22, 22, R"=====(MmxExt: AMD extensions to MMX™ instructions. Read-only. Reset: Fixed,1)====="}, {21, 21, R"=====(Reserved)====="}, {20, 20, R"=====(NX: no-execute page protection. Read-only. Reset: Fixed,1)====="}, {19, 18, R"=====(Reserved)====="}, {17, 17, R"=====(PSE36: page-size extensions. Read-only. Reset: Fixed,1)====="}, {16, 16, R"=====(PAT: page attribute table. Read-only. Reset: Fixed,1)====="}, {15, 15, R"=====(CMOV: conditional move instructions, CMOV, FCOMI, FCMOV. Read-only. Reset: Fixed,1)====="}, {14, 14, R"=====(MCA: machine check architecture, MCG_CAP. Read-only. Reset: Fixed,1)====="}, {13, 13, R"=====(PGE: page global extension, CR4.PGE. Read-only. Reset: Fixed,1)====="}, {12, 12, R"=====(MTRR: memory-type range registers. Read-only. Reset: Fixed,1)====="}, {11, 11, R"=====(SysCallSysRet: SYSCALL and SYSRET instructions. Read-only. Reset: Fixed,1)====="}, {10, 10, R"=====(Reserved)====="}, {9, 9, R"=====(APIC: advanced programmable interrupt controller (APIC) exists and is enabled. Read-only. Reset: X.
Reset is Core::X86::Msr::APIC_BAR[ApicEn])====="},
          {8, 8, R"=====(CMPXCHG8B: CMPXCHG8B instruction. Read-only. Reset: Fixed,1)====="},
          {7, 7, R"=====(MCE: machine check exception, CR4.MCE. Read-only. Reset: Fixed,1)====="},
          {6, 6, R"=====(PAE: physical-address extensions (PAE). Read-only. Reset: Fixed,1)====="},
          {5, 5, R"=====(MSR: model-specific registers (MSRs), with RDMSR and WRMSR instructions. Read-only. Reset: Fixed,1)====="},
          {4, 4, R"=====(TSC: time stamp counter, RDTSC/RDTSCP instructions, CR4.TSD. Read-only. Reset: Fixed,1)====="},
          {3, 3, R"=====(PSE: page-size extensions (4 MB pages). Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(DE: debugging extensions, IO breakpoints, CR4.DE. Read-only. Reset: Fixed,1)====="},
          {1, 1, R"=====(VME: virtual-mode enhancements. Read-only. Reset: Fixed,1)====="},
          {0, 0, R"=====(FPU: x87 floating point unit on-chip. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn80000002_EAX", "Processor Name String Identifier (Bytes3:0])", "Core::X86::Cpuid::ProcNameStr0Eax", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000002_EAX",
         {{31, 24, R"=====(ProcNameByte3: processor name, byte3. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte2: processor name, byte2. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte1: processor name, byte1. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte0: processor name, byte0. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000002_EBX", "Processor Name String Identifier (Bytes7:4])", "Core::X86::Cpuid::ProcNameStr0Ebx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000002_EBX",
         {{31, 24, R"=====(ProcNameByte7: processor name, byte 7. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte6: processor name, byte 6. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte5: processor name, byte 5. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte4: processor name, byte 4. Read-only. Reset:
Core::X86::Msr::ProcNameString_n0[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000002_ECX", "Processor Name String Identifier (Bytes11:8])", "Core::X86::Cpuid::ProcNameStr0Ecx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n1.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000002_ECX",
         {{31, 24, R"=====(ProcNameByte11: processor name, byte 11. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte10: processor name, byte 10. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte9: processor name, byte 9. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte8: processor name, byte 8. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000002_EDX", "Processor Name String Identifier (Bytes15:12])", "Core::X86::Cpuid::ProcNameStr0Edx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n1.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000002_EDX",
         {{31, 24, R"=====(ProcNameByte15: processor name, byte 15. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte14: processor name, byte 14. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte13: processor name, byte 13. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte12: processor name, byte 12. Read-only. Reset:
Core::X86::Msr::ProcNameString_n1[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000003_EAX", "Processor Name String Identifier (Bytes19:16])", "Core::X86::Cpuid::ProcNameStr1Eax", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n2.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000003_EAX",
         {{31, 24, R"=====(ProcNameByte19: processor name, byte 19. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte18: processor name, byte 18. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte17: processor name, byte 17. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte16: processor name, byte 16. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000003_EBX", "Processor Name String Identifier (Bytes23:20])", "Core::X86::Cpuid::ProcNameStr1Ebx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n2.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000003_EBX",
         {{31, 24, R"=====(ProcNameByte23: processor name, byte 23. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte22: processor name, byte 22. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte21: processor name, byte 21. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte20: processor name, byte 20. Read-only. Reset:
Core::X86::Msr::ProcNameString_n2[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000003_ECX", "Processor Name String Identifier (Bytes27:24])", "Core::X86::Cpuid::ProcNameStr1Ecx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n3.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000003_ECX",
         {{31, 24, R"=====(ProcNameByte27: processor name, byte 27. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte26: processor name, byte 26. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte25: processor name, byte 25. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte24: processor name, byte 24. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000003_EDX", "Processor Name String Identifier (Bytes31:28])", "Core::X86::Cpuid::ProcNameStr1Edx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n3.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000003_EDX",
         {{31, 24, R"=====(ProcNameByte31: processor name, byte 31. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte30: processor name, byte 30. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte29: processor name, byte 29. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte28: processor name, byte 28. Read-only. Reset:
Core::X86::Msr::ProcNameString_n3[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000004_EAX", "Processor Name String Identifier (Bytes35:32])", "Core::X86::Cpuid::ProcNameStr2Eax", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n4.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000004_EAX",
         {{31, 24, R"=====(ProcNameByte35: processor name, byte 35. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte34: processor name, byte 34. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte33: processor name, byte 33. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte32: processor name, byte 32. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000004_EBX", "Processor Name String Identifier (Bytes39:36])", "Core::X86::Cpuid::ProcNameStr2Ebx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n4.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000004_EBX",
         {{31, 24, R"=====(ProcNameByte39: processor name, byte 39. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte38: processor name, byte 38. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte37: processor name, byte 37. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte36: processor name, byte 36. Read-only. Reset:
Core::X86::Msr::ProcNameString_n4[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000004_ECX", "Processor Name String Identifier (Bytes43:40])", "Core::X86::Cpuid::ProcNameStr2Ecx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n5.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000004_ECX",
         {{31, 24, R"=====(ProcNameByte43: processor name, byte 43. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString3])====="},
          {23, 16, R"=====(ProcNameByte42: processor name, byte 42. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString2])====="},
          {15, 8, R"=====(ProcNameByte41: processor name, byte 41. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString1])====="},
          {7, 0, R"=====(ProcNameByte40: processor name, byte 40. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString0].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000004_EDX", "Processor Name String Identifier (Bytes47:44])", "Core::X86::Cpuid::ProcNameStr2Edx", R"(=====Is an alias of Core::X86::Msr::ProcNameString_n5.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000004_EDX",
         {{31, 24, R"=====(ProcNameByte47: processor name, byte 47. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString7])====="},
          {23, 16, R"=====(ProcNameByte46: processor name, byte 46. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString6])====="},
          {15, 8, R"=====(ProcNameByte45: processor name, byte 45. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString5])====="},
          {7, 0, R"=====(ProcNameByte44: processor name, byte 44. Read-only. Reset:
Core::X86::Msr::ProcNameString_n5[CpuNameString4].)====="}}, Type::STRING_REGISTER},
        {"CPUID_Fn80000005_EAX", "L1 TLB 2M/4M Identifiers", "Core::X86::Cpuid::L1Tlb2M4M", R"(=====This function provides the processor's first level cache and TLB characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000005_EAX",
         {{31, 24, R"=====(L1DTlb2and4MAssoc: data TLB associativity for 2 MB and 4 MB pages. Read-only. Reset: Fixed,FFh. See
        Core::X86::Cpuid::L1DcId[L1DcAssoc])====="},
          {23, 16, R"=====(L1DTlb2and4MSize: data TLB number of entries for 2 MB and 4 MB pages. Read-only. Reset: Fixed,64.
The value returned is for the number of entries available for the 2 MB page size; 4 MB pages require two 2 MB
        entries, so the number of entries available for the 4 MB page size is one-half the returned value)====="},
          {15, 8, R"=====(L1ITlb2and4MAssoc: instruction TLB associativity for 2 MB and 4 MB pages. Read-only. Reset: Fixed,FFh.
See Core::X86::Cpuid::L1DcId[L1DcAssoc])====="},
          {7, 0, R"=====(L1ITlb2and4MSize: instruction TLB number of entries for 2 MB and 4 MB pages. Read-only. Reset:
Fixed,64. The value returned is for the number of entries available for the 2 MB page size; 4 MB pages require
        two 2 MB entries, so the number of entries available for the 4 MB page size is one-half the returned value.)====="}}},
        {"CPUID_Fn80000005_EBX", "L1 TLB 4K Identifiers", "Core::X86::Cpuid::L1Tlb4K", R"(=====See Core::X86::Cpuid::L1Tlb2M4M.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000005_EBX",
         {{31, 24, R"=====(L1DTlb4KAssoc: data TLB associativity for 4 KB pages. Read-only. Reset: Fixed,FFh. See
        Core::X86::Cpuid::L1DcId[L1DcAssoc])====="},
          {23, 16, R"=====(L1DTlb4KSize: data TLB number of entries for 4 KB pages. Read-only. Reset: Fixed,64)====="},
          {15, 8, R"=====(L1ITlb4KAssoc: instruction TLB associativity for 4 KB pages. Read-only. Reset: Fixed,FFh. See
        Core::X86::Cpuid::L1DcId[L1DcAssoc])====="},
          {7, 0, R"=====(L1ITlb4KSize: instruction TLB number of entries for 4 KB pages. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000005_ECX", "L1 Data Cache Identifiers", "Core::X86::Cpuid::L1DcId", R"(=====This function provides first level cache characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000005_ECX",
         {{31, 24, R"=====(L1DcSize: L1 data cache size in KB. Read-only. Reset: Fixed,32)====="}, {23, 16, R"=====(L1DcAssoc: L1 data cache associativity. Read-only. Reset: Fixed,8.
ValidValues:
Value   Description
00h     Reserved
01h     1 way (direct mapped)
02h     2 way
03h     3 way
        FEh-    <Value> way
04h
FFh     Fully associativ)====="},
          {15, 8, R"=====(L1DcLinesPerTag: L1 data cache lines per tag. Read-only. Reset: Fixed,01h)====="},
          {7, 0, R"=====(L1DcLineSize: L1 data cache line size in bytes. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000005_EDX", "L1 Instruction Cache Identifiers", "Core::X86::Cpuid::L1IcId", R"(=====This function provides first level cache characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000005_EDX",
         {{31, 24, R"=====(L1IcSize: L1 instruction cache size KB. Read-only. Reset: Fixed,64)====="}, {23, 16, R"=====(L1IcAssoc: L1 instruction cache associativity. Read-only. Reset: Fixed,4.
ValidValues:
Value     Description
00h       Reserved
01h       1 way (direct mapped)
02h       2 way
03h       3 way
04h       4 way
        FEh-      <Value> way
05h
FFh       Fully associativ)====="},
          {15, 8, R"=====(L1IcLinesPerTag: L1 instruction cache lines per tag. Read-only. Reset: Fixed,01h)====="},
          {7, 0, R"=====(L1IcLineSize: L1 instruction cache line size in bytes. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000006_EAX", "L2 TLB 2M/4M Identifiers", "Core::X86::Cpuid::L2Tlb2M4M", R"(=====This function provides the processor's second level cache and TLB characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000006_EAX",
         {{31, 28, R"=====(L2DTlb2and4MAssoc: L2 data TLB associativity for 2 MB and 4 MB pages. Read-only. Reset: Xh.
ValidValues:
Value    Description
1h-0h    Reserved.
2h       2 ways
3h       3 ways
        Fh-4h    Reserved)====="},
          {27, 16, R"=====(L2DTlb2and4MSize: L2 data TLB number of entries for 2 MB and 4 MB pages. Read-only. Reset:
Fixed,1536. The value returned is for the number of entries available for the 2 MB page size; 4 MB pages require
        two 2 MB entries, so the number of entries available for the 4 MB page size is one-half the returned value)====="},
          {15, 12, R"=====(L2ITlb2and4MAssoc: L2 instruction TLB associativity for 2 MB and 4 MB pages. Read-only. Reset: Fixed,6.
ValidValues:
Value    Description
5h-0h    Reserved.
6h      8 ways
        Fh-7h   Reserved)====="},
          {11, 0, R"=====(L2ITlb2and4MSize: L2 instruction TLB number of entries for 2 MB and 4 MB pages. Read-only. Reset:
Fixed,1024. The value returned is for the number of entries available for the 2 MB page size; 4 MB pages require
        two 2 MB entries, so the number of entries available for the 4 MB page size is one-half the returned value.)====="}}},
        {"CPUID_Fn80000006_EBX", "L2 TLB 4K Identifiers", "Core::X86::Cpuid::L2Tlb4K", R"(=====This function provides the processor's second level cache and TLB characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000006_EBX",
         {{31, 28, R"=====(L2DTlb4KAssoc: L2 data TLB associativity for 4 KB pages. Read-only. Reset: Xh.
ValidValues:
Value     Description
4h-0h     Reserved.
5h        6 ways
6h        8 ways
        Fh-7h     Reserved)====="},
          {27, 16, R"=====(L2DTlb4KSize: L2 data TLB number of entries for 4 KB pages. Read-only. Reset: Fixed,1536)====="},
          {15, 12, R"=====(L2ITlb4KAssoc: L2 instruction TLB associativity for 4 KB pages. Read-only. Reset: Fixed,6.
ValidValues:
Value     Description
5h-0h     Reserved.
6h        8 ways
        Fh-7h     Reserved)====="},
          {11, 0, R"=====(L2ITlb4KSize: L2 instruction TLB number of entries for 4 KB pages. Read-only. Reset: Fixed,1024.)====="}}},
        {"CPUID_Fn80000006_ECX", "L2 Cache Identifiers", "Core::X86::Cpuid::L2CacheId", R"(=====This function provides second level cache characteristics for each core.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000006_ECX",
         {{31, 16, R"=====(L2Size: L2 cache size in KB. Read-only. Reset: Fixed,0200h.
ValidValues:
Value     Description
00FFh- Reserved.
0000h
0100h 256 KB
01FFh- Reserved.
0101h
0200h 512 KB
03FFh- Reserved.
0201h
0400h 1 MB
07FFh- Reserved.
0401h
0800h 2 MB
        FFFFh- Reserved.
0801)====="},
          {15, 12, R"=====(L2Assoc: L2 cache associativity. Read-only. Reset: Fixed,6.
ValidValues:
Value    Description
0h       Disabled.
1h       1 way (direct mapped)
2h       2 ways
3h       Reserved.
4h       4 ways
5h       Reserved.
6h       8 ways
7h       Reserved.
8h       16 ways
9h       Reserved.
Ah       32 ways
        Bh       48 ways
        Ch       64 ways
        Dh       96 ways
        Eh       128 ways
        Fh       Fully associativ)====="},
          {11, 8, R"=====(L2LinesPerTag: L2 cache lines per tag. Read-only. Reset: Fixed,1h)====="},
          {7, 0, R"=====(L2LineSize: L2 cache line size in bytes. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000006_EDX", "L3 Cache Identifiers", "Core::X86::Cpuid::L3CacheId", R"(=====This function provides third level cache characteristics shared by all cores of a processor.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000006_EDX",
         {{31, 18, R"=====(L3Size: L3 cache size. Read-only. Reset: XXXXh. The L3 cache size in 512 KB units.
ValidValues:
Value     Description
0000h Disabled.
3FFFh- (<Value> *0.5) MB
0001)====="},
          {17, 16, R"=====(Reserved)====="},
          {15, 12, R"=====(L3Assoc: L3 cache associativity. Read-only. Reset: Xh.
ValidValues:
Value     Description
7h-0h     Reserved.
8h        16 Ways
9h        Reserved.
Ah        32 Ways
        Bh        48 Ways
        Ch        64 Ways
        Dh        96 Ways
        Eh        128 Ways
        Fh        Reserved)====="},
          {11, 8, R"=====(L3LinesPerTag: L3 cache lines per tag. Read-only. Reset: Fixed,1h)====="},
          {7, 0, R"=====(L3LineSize: L3 cache line size in bytes. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000007_EAX", "Reserved", "Core::X86::Cpuid::ProcFeedbackCap", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000007_EAX",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn80000007_EBX", "RAS Capabilities", "Core::X86::Cpuid::RasCap", R"(=====Read-only. Reset: 0000_000Xh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000007_EBX",
         {{31, 4, R"=====(Reserved)====="}, {3, 3, R"=====(ScalableMca. Read-only. Reset: Fixed,1. 0=Scalable MCA is not supported. 1=Scalable MCA is supported. See
3.2 [Machine Check Architecture Extensions] and MCA_CONFIG[McaX] for the respective bank)====="},
          {2, 2, R"=====(HWA: hardware assert supported. Read-only. Reset: Fixed,0)====="},
          {1, 1, R"=====(SUCCOR: Software uncorrectable error containment and recovery capability. Read-only. Reset: X. The
        processor supports software containment of uncorrectable errors through context synchronizing data poisoning
        and deferred error interrupts; MSR Core::X86::Msr::McaIntrCfg, MCA_STATUS[Deferred] and
MCA_STATUS[Poison] exist)====="},
          {0, 0, R"=====(McaOverflowRecov: MCA overflow recovery support. Read-only. Reset: Fixed,1. 0=MCA overflow
conditions require software to shut down the system. 1=MCA overflow conditions (MCi_STATUS[Overflow] ==
1) are not fatal; software may safely ignore such conditions. See 3 [Machine Check Architecture].)====="}}},
        {"CPUID_Fn80000007_ECX", "Advanced Power Management Information", "Core::X86::Cpuid::ApmInfoEcx", R"(=====Read-only. Reset: 0000_0000h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000007_ECX",
         {{31, 0, R"=====(CmpUnitPwrSampleTimeRatio. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn80000007_EDX", "Advanced Power Management Information", "Core::X86::Cpuid::ApmInfoEdx", R"(=====This function provides advanced power management feature identifiers.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000007_EDX",
         {{31, 15, R"=====(Reserved)====="}, {14, 14, R"=====(RAPL: running average power limit. Read-only. Reset: Fixed,1)====="}, {13, 13, R"=====(ConnectedStandby: Connected Standby. Read-only. Reset: Fixed,1)====="}, {12, 12, R"=====(ApmPwrReporting: APM power reporting. Read-only. Reset: Fixed,0)====="}, {11, 11, R"=====(ProcFeedbackInterface: processor feedback interface. Read-only. Reset: Fixed,0. 1=Indicates support for
processor feedback interface; Core::X86::Cpuid::ProcFeedbackCap)====="},
          {10, 10, R"=====(EffFreqRO: read-only effective frequency interface. Read-only. Reset: Fixed,1. Indicates presence of
        Core::X86::Msr::MPerfReadOnly and Core::X86::Msr::APerfReadOnly)====="},
          {9, 9, R"=====(CPB: core performance boost. Read-only. Reset: 1. 1=Indicates presence of Core::X86::Msr::HWCR[CpbDis]
and support for core performance boost)====="},
          {8, 8, R"=====(TscInvariant: TSC invariant. Read-only. Reset: Fixed,1. The TSC rate is invariant)====="},
          {7, 7, R"=====(HwPstate: hardware P-state control. Read-only. Reset: Fixed,1. Core::X86::Msr::PStateCurLim,
Core::X86::Msr::PStateCtl and Core::X86::Msr::PStateStat exist)====="},
          {6, 6, R"=====(OneHundredMHzSteps: 100 MHz multiplier Control. Read-only. Reset: Fixed,0)====="},
          {5, 4, R"=====(Reserved)====="},
          {3, 3, R"=====(TTP: THERMTRIP. Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(VID: Voltage ID control. Read-only. Reset: Fixed,0. Function replaced by HwPstate)====="},
          {1, 1, R"=====(FID: Frequency ID control. Read-only. Reset: Fixed,0. Function replaced by HwPstate)====="},
          {0, 0, R"=====(TS: Temperature sensor. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn80000008_EAX", "Long Mode Address Size Identifiers", "Core::X86::Cpuid::LongModeInfo", R"(=====This provides information about the maximum physical and linear address width supported by the processor.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000008_EAX",
         {{31, 24, R"=====(Reserved)====="}, {23, 16, R"=====(GuestPhysAddrSize: maximum guest physical byte address size in bits. Read-only. Reset: Fixed,00h.
ValidValues:
Value     Description
00h       The maximum guest physical address size defined by PhysAddrSize.
FFh-      The maximum guest physical address size defined by GuestPhysAddrSize.
01)====="},
          {15, 8, R"=====(LinAddrSize: Maximum linear byte address size in bits. Read-only. Reset: Fixed,30h)====="},
          {7, 0, R"=====(PhysAddrSize: Maximum physical byte address size in bits. Read-only. Reset: Fixed,30h.)====="}}},
        {"CPUID_Fn80000008_EBX", "Extended Feature Extensions ID EBX", "Core::X86::Cpuid::FeatureExtIdEbx", R"(=====Read-only. Reset: 0000_0007h.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000008_EBX",
         {{31, 3, R"=====(Reserved)====="}, {2, 2, R"=====(XSaveErPtr. Read-only. Reset: Fixed,1. 1=FXSAVE, XSAVE, FXSAVEOPT, XSAVEC, XSAVES always save
        error pointers and FXRSTOR, XRSTOR, XRSTORS always restore error pointers is supported)====="},
          {1, 1, R"=====(IRPerf: instructions retired count support. Read-only. Reset: Fixed,1. 1=Core::X86::Msr::IRPerfCount
        supported)====="},
          {0, 0, R"=====(CLZERO: Clear Zero Instruction. Read-only. Reset: Fixed,1. CLZERO instruction zero's out the 64 byte cache
line specified in RAX. Note: CLZERO instruction operations are cache-line aligned and RAX[5:0] is ignored.)====="}}},
        {"CPUID_Fn80000008_ECX", "Size Identifiers", "Core::X86::Cpuid::SizeId", R"(=====This provides information about the number of threads supported by the processor.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000008_ECX",
         {{31, 18, R"=====(Reserved)====="}, {17, 16, R"=====(PerfTscSize: performance time-stamp counter size. Read-only. Reset: Fixed,0h)====="}, {15, 12, R"=====(ApicIdSize: APIC ID size. Read-only. Reset: Xh. The number of bits in the initial
        Core::X86::Apic::ApicId[ApicId] value that indicate thread ID within a package)====="},
          {11, 8, R"=====(Reserved)====="},
          {7, 0, R"=====(NC: number of threads - 1. Read-only. Reset: XXh. The number of threads in the package is NC+1 (eg., if
NC=0, then there is one thread).)====="}}},
        {"CPUID_Fn8000000A_EAX", "SVM Revision and Feature Identification", "Core::X86::Cpuid::SvmRevFeatIdEax", R"(=====        Core::X86::Cpuid::SvmRevFeatIdEax is reserved.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000000A_EAX",
         {{31, 8, R"=====(Reserved)====="}, {7, 0, R"=====(SvmRev: SVM revision. Read-only. Reset: Fixed,01h.)====="}}},
        {"CPUID_Fn8000000A_EBX", "SVM Revision and Feature Identification", "Core::X86::Cpuid::SvmRevFeatIdEbx", R"(=====        Core::X86::Cpuid::SvmRevFeatIdEbx is reserved.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000000A_EBX",
         {{31, 0, R"=====(NASID: number of address space identifiers (ASID). Read-only,Volatile. Reset: 0000_8000h.)====="}}},
        {"CPUID_Fn8000000A_EDX", "SVM Revision and Feature Identification", "Core::X86::Cpuid::SvmRevFeatIdEdx", R"(=====        Core::X86::Cpuid::SvmRevFeatIdEdx is reserved.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000000A_EDX",
         {{31, 17, R"=====(Reserved)====="}, {16, 16, R"=====(vGIF: virtualized GIF. Read-only. Reset: Fixed,1)====="}, {15, 15, R"=====(V_VMSAVE_VMLOAD: virtualized VMLOAD and VMSAVE. Read-only. Reset: Fixed,1)====="}, {14, 14, R"=====(Reserved)====="}, {13, 13, R"=====(AVIC: AMD virtual interrupt controller. Read-only. Reset: Fixed,1. 1=Support indicated for SVM mode
virtualized interrupt controller; Indicates support for Core::X86::Msr::AvicDoorbell)====="},
          {12, 12, R"=====(PauseFilterThreshold: PAUSE filter threshold. Read-only. Reset: Fixed,1)====="},
          {11, 11, R"=====(Reserved)====="},
          {10, 10, R"=====(PauseFilter: pause intercept filter. Read-only. Reset: Fixed,1)====="},
          {9, 8, R"=====(Reserved)====="},
          {7, 7, R"=====(DecodeAssists: decode assists. Read-only. Reset: Fixed,1)====="},
          {6, 6, R"=====(FlushByAsid: flush by ASID. Read-only. Reset: Fixed,1)====="},
          {5, 5, R"=====(VmcbClean: VMCB clean bits. Read-only. Reset: Fixed,1)====="},
          {4, 4, R"=====(TscRateMsr: MSR based TSC rate control. Read-only. Reset: Fixed,1. 1=Indicates support for TSC ratio
Core::X86::Msr::TscRateMsr)====="},
          {3, 3, R"=====(NRIPS: NRIP Save. Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(SVML: SVM lock. Read-only. Reset: Fixed,1)====="},
          {1, 1, R"=====(LbrVirt: LBR virtualization. Read-only. Reset: Fixed,1)====="},
          {0, 0, R"=====(NP: nested paging. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn80000019_EAX", "L1 TLB 1G Identifiers", "Core::X86::Cpuid::L1Tlb1G", R"(=====This function provides first level TLB characteristics for 1GB pages.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000019_EAX",
         {{31, 28, R"=====(L1DTlb1GAssoc: L1 data TLB associativity for 1 GB pages. Read-only. Reset: Fixed,Fh. See
        Core::X86::Cpuid::L2CacheId[L2Assoc])====="},
          {27, 16, R"=====(L1DTlb1GSize: L1 data TLB number of entries for 1 GB pages. Read-only. Reset: Fixed,64)====="},
          {15, 12, R"=====(L1ITlb1GAssoc: L1 instruction TLB associativity for 1 GB pages. Read-only. Reset: Fixed,Fh. See
        Core::X86::Cpuid::L2CacheId[L2Assoc])====="},
          {11, 0, R"=====(L1ITlb1GSize: L1 instruction TLB number of entries for 1 GB pages. Read-only. Reset: Fixed,64.)====="}}},
        {"CPUID_Fn80000019_EBX", "L2 TLB 1G Identifiers", "Core::X86::Cpuid::L2Tlb1G", R"(=====Core::X86::Cpuid::L2Tlb4K, Core::X86::Cpuid::L2CacheId and Core::X86::Cpuid::L3CacheId.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn80000019_EBX",
         {{31, 28, R"=====(L2DTlb1GAssoc: L2 data TLB associativity for 1 GB pages. Read-only. Reset: Fixed,0h. See
        Core::X86::Cpuid::L2CacheId[L2Assoc])====="},
          {27, 16, R"=====(L2DTlb1GSize: L2 data TLB number of entries for 1 GB pages. Read-only. Reset: Fixed,000h)====="},
          {15, 12, R"=====(L2ITlb1GAssoc: L2 instruction TLB associativity for 1 GB pages. Read-only. Reset: Fixed,0h. See
        Core::X86::Cpuid::L2CacheId[L2Assoc])====="},
          {11, 0, R"=====(L2ITlb1GSize: L2 instruction TLB number of entries for 1 GB pages. Read-only. Reset: Fixed,000h.)====="}}},
        {"CPUID_Fn8000001A_EAX", "Performance Optimization Identifiers", "Core::X86::Cpuid::PerfOptId", R"(=====        see the optimization guide.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001A_EAX",
         {{31, 3, R"=====(Reserved)====="}, {2, 2, R"=====(FP256. Read-only. Reset: Fixed,0)====="}, {1, 1, R"=====(MOVU. Read-only. Reset: Fixed,1)====="}, {0, 0, R"=====(FP128. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn8000001B_EAX", "Instruction Based Sampling Identifiers", "Core::X86::Cpuid::IbsIdEax", R"(=====This function returns IBS feature information.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001B_EAX",
         {{31, 11, R"=====(Reserved)====="}, {10, 10, R"=====(IbsOpData4: IBS op data 4 MSR supported. Read-only. Reset: Fixed,0)====="}, {9, 9, R"=====(IbsFetchCtlExtd: IBS fetch control extended MSR supported. Read-only. Reset: Fixed,1. Indicates support for
Core::X86::Msr::IC_IBS_EXTD_CTL)====="},
          {8, 8, R"=====(OpBrnFuse: fused branch micro-op indication supported. Read-only. Reset: Fixed,1. Indicates support for
Core::X86::Msr::IBS_OP_DATA[IbsOpBrnFuse])====="},
          {7, 7, R"=====(RipInvalidChk: invalid RIP indication supported. Read-only. Reset: Fixed,1. Indicates support for
Core::X86::Msr::IBS_OP_DATA[IbsRipInvalid])====="},
          {6, 6, R"=====(OpCntExt: IbsOpCurCnt and IbsOpMaxCnt extend by 7 bits. Read-only. Reset: Fixed,1. Indicates support
for Core::X86::Msr::IBS_OP_CTL[IbsOpCurCnt[26:20],IbsOpMaxCnt[26:20]])====="},
          {5, 5, R"=====(BrnTrgt: branch target address reporting supported. Read-only. Reset: Fixed,1)====="},
          {4, 4, R"=====(OpCnt: op counting mode supported. Read-only. Reset: Fixed,1)====="},
          {3, 3, R"=====(RdWrOpCnt: read write of op counter supported. Read-only. Reset: Fixed,1)====="},
          {2, 2, R"=====(OpSam: IBS execution sampling supported. Read-only. Reset: Fixed,1)====="},
          {1, 1, R"=====(FetchSam: IBS fetch sampling supported. Read-only. Reset: Fixed,1)====="},
          {0, 0, R"=====(IBSFFV: IBS feature flags valid. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn8000001D_EAX_x00", "Cache Properties (DC)", "Core::X86::Cpuid::CachePropEax0", R"(=====reserved.
=====)",
         "_lthree[1:0]_core[3:0]",
         "CPUID_Fn8000001D_EAX_x00",
         {{31, 26, R"=====(Reserved)====="}, {25, 14, R"=====(NumSharingCache: number of logical processors sharing cache. Read-only. Reset: XXXh. The number of
        logical processors sharing this cache is NumSharingCache+1)====="},
          {13, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(FullyAssociative: fully associative cache. Read-only. Reset: Fixed,0. 1=Cache is fully associative)====="},
          {8, 8, R"=====(SelfInitialization: cache is self-initializing. Read-only. Reset: Fixed,1. 1=Cache is self initializing; cache does
not need software initialization)====="},
          {7, 5, R"=====(CacheLevel: cache level. Read-only. Reset: Fixed,1h. Identifies the cache level.
ValidValues:
Value    Description
0h       Reserved.
1h       Level 1
2h       Level 2
3h       Level 3
7h-4h    Reserved)====="},
          {4, 0, R"=====(CacheType: cache type. Read-only. Reset: Fixed,01h. Identifies the type of cache.
ValidValues:
Value    Description
00h      Null; no more caches.
01h      Data cache.
02h      Instruction cache.
03h      Unified cache.
1Fh-04h Reserved.)====="}}},
        {"CPUID_Fn8000001D_EAX_x01", "Cache Properties (IC)", "Core::X86::Cpuid::CachePropEax1", R"(=====Core::X86::Cpuid::CachePropEax1 reports topology information for the IC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EAX_x01",
         {{31, 26, R"=====(Reserved)====="}, {25, 14, R"=====(NumSharingCache: number of logical processors sharing cache. Read-only. Reset: XXXh. See
        Core::X86::Cpuid::CachePropEax0[NumSharingCache])====="},
          {13, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(FullyAssociative: fully associative cache. Read-only. Reset: Fixed,0. See
        Core::X86::Cpuid::CachePropEax0[FullyAssociative])====="},
          {8, 8, R"=====(SelfInitialization: cache is self-initializing. Read-only. Reset: Fixed,1. See
        Core::X86::Cpuid::CachePropEax0[SelfInitialization])====="},
          {7, 5, R"=====(CacheLevel: cache level. Read-only. Reset: Fixed,1h. Identifies the cache level. See
        Core::X86::Cpuid::CachePropEax0[CacheLevel])====="},
          {4, 0, R"=====(CacheType: cache type. Read-only. Reset: Fixed,02h. See Core::X86::Cpuid::CachePropEax0[CacheType].)====="}}},
        {"CPUID_Fn8000001D_EAX_x02", "Cache Properties (L2)", "Core::X86::Cpuid::CachePropEax2", R"(=====Core::X86::Cpuid::CachePropEax2 reports topology information for the L2. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EAX_x02",
         {{31, 26, R"=====(Reserved)====="}, {25, 14, R"=====(NumSharingCache: number of logical processors sharing cache. Read-only. Reset: XXXh.
Core::X86::Cpuid::CachePropEax0[NumSharingCache])====="},
          {13, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(FullyAssociative: fully associative cache. Read-only. Reset: Fixed,0.
Core::X86::Cpuid::CachePropEax0[FullyAssociative])====="},
          {8, 8, R"=====(SelfInitialization: cache is self-initializing. Read-only. Reset: Fixed,1.
Core::X86::Cpuid::CachePropEax0[SelfInitialization])====="},
          {7, 5, R"=====(CacheLevel: cache level. Read-only. Reset: Fixed,2h. Identifies the cache level.
Core::X86::Cpuid::CachePropEax0[CacheLevel])====="},
          {4, 0, R"=====(CacheType: cache type. Read-only. Reset: Fixed,03h. Core::X86::Cpuid::CachePropEax0[CacheType].)====="}}},
        {"CPUID_Fn8000001D_EAX_x03", "Cache Properties (L3)", "Core::X86::Cpuid::CachePropEax3", R"(=====Core::X86::Cpuid::CachePropEax3 reports topology information for the L3.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EAX_x03",
         {{31, 26, R"=====(Reserved)====="}, {25, 14, R"=====(NumSharingCache: number of logical processors sharing cache. Read-only. Reset: XXXh. The number of
        logical processors sharing this cache is NumSharingCache+1)====="},
          {13, 10, R"=====(Reserved)====="},
          {9, 9, R"=====(FullyAssociative: fully associative cache. Read-only. Reset: Fixed,0.
Core::X86::Cpuid::CachePropEax0[FullyAssociative])====="},
          {8, 8, R"=====(SelfInitialization: cache is self-initializing. Read-only. Reset: Fixed,1.
Core::X86::Cpuid::CachePropEax0[SelfInitialization])====="},
          {7, 5, R"=====(CacheLevel: cache level. Read-only. Reset: Fixed,3h. Identifies the cache level.
Core::X86::Cpuid::CachePropEax0[CacheLevel])====="},
          {4, 0, R"=====(CacheType: cache type. Read-only. Reset: Fixed,03h. Core::X86::Cpuid::CachePropEax0[CacheType].)====="}}},
        {"CPUID_Fn8000001D_EAX_x04", "Cache Properties Null", "Core::X86::Cpuid::CachePropEax4", R"(=====Core::X86::Cpuid::CachePropEax4 reports done/null. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EAX_x04",
         {{31, 5, R"=====(Reserved)====="}, {4, 0, R"=====(CacheType: cache type. Read-only. Reset: Fixed,00h. Core::X86::Cpuid::CachePropEax0[CacheType].)====="}}},
        {"CPUID_Fn8000001D_EBX_x00", "Cache Properties (DC)", "Core::X86::Cpuid::CachePropEbx0", R"(=====Core::X86::Cpuid::CachePropEbx0 reports topology information for the DC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EBX_x00",
         {{31, 22, R"=====(CacheNumWays: cache number of ways. Read-only. Reset: Fixed,007h. Cache number of ways is
        CacheNumWays + 1)====="},
          {21, 12, R"=====(CachePhysPartitions: cache physical line partitions. Read-only. Reset: Fixed,000h. Cache partitions is
        CachePhysPartitions + 1)====="},
          {11, 0, R"=====(CacheLineSize: cache line size in bytes. Read-only. Reset: Fixed,03Fh. Cache line size in bytes is
CacheLineSize + 1.)====="}}},
        {"CPUID_Fn8000001D_EBX_x01", "Cache Properties (IC)", "Core::X86::Cpuid::CachePropEbx1", R"(=====Core::X86::Cpuid::CachePropEbx1 reports topology information for the IC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EBX_x01",
         {{31, 22, R"=====(CacheNumWays: cache number of ways. Read-only. Reset: Fixed,003h.
Core::X86::Cpuid::CachePropEbx0[CacheNumWays])====="},
          {21, 12, R"=====(CachePhysPartitions: cache physical line partitions. Read-only. Reset: Fixed,000h.
Core::X86::Cpuid::CachePropEbx0[CachePhysPartitions])====="},
          {11, 0, R"=====(CacheLineSize: cache line size in bytes. Read-only. Reset: Fixed,03Fh.
Core::X86::Cpuid::CachePropEbx0[CacheLineSize].)====="}}},
        {"CPUID_Fn8000001D_EBX_x02", "Cache Properties (L2)", "Core::X86::Cpuid::CachePropEbx2", R"(=====Core::X86::Cpuid::CachePropEbx2 reports topology information for the L2. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EBX_x02",
         {{31, 22, R"=====(CacheNumWays: cache number of ways. Read-only. Reset: Fixed,007h. See
        Core::X86::Cpuid::CachePropEbx0[CacheNumWays])====="},
          {21, 12, R"=====(CachePhysPartitions: cache physical line partitions. Read-only. Reset: Fixed,000h. See
        Core::X86::Cpuid::CachePropEbx0[CachePhysPartitions])====="},
          {11, 0, R"=====(CacheLineSize: cache line size in bytes. Read-only. Reset: Fixed,03Fh. See
        Core::X86::Cpuid::CachePropEbx0[CacheLineSize].)====="}}},
        {"CPUID_Fn8000001D_EBX_x03", "Cache Properties (L3)", "Core::X86::Cpuid::CachePropEbx3", R"(=====Core::X86::Cpuid::CachePropEbx3 reports topology information for the L3. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EBX_x03",
         {{31, 22, R"=====(CacheNumWays: cache number of ways. Read-only. Reset: Fixed,00Fh. See
        Core::X86::Cpuid::CachePropEbx0[CacheNumWays])====="},
          {21, 12, R"=====(CachePhysPartitions: cache physical line partitions. Read-only. Reset: Fixed,000h. See
        Core::X86::Cpuid::CachePropEbx0[CachePhysPartitions])====="},
          {11, 0, R"=====(CacheLineSize: cache line size in bytes. Read-only. Reset: Fixed,03Fh. See
        Core::X86::Cpuid::CachePropEbx0[CacheLineSize].)====="}}},
        {"CPUID_Fn8000001D_EBX_x04", "Cache Properties Null", "Core::X86::Cpuid::CachePropEbx4", R"(=====Core::X86::Cpuid::CachePropEax4 reports done/null. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EBX_x04",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn8000001D_ECX_x00", "Cache Properties (DC)", "Core::X86::Cpuid::CachePropEcx0", R"(=====Core::X86::Cpuid::CachePropEcx0 reports topology information for the DC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_ECX_x00",
         {{31, 0, R"=====(CacheNumSets: cache number of sets. Read-only. Reset: Fixed,0000_003Fh. Cache number of sets is
        CacheNumSets+1.)====="}}},
        {"CPUID_Fn8000001D_ECX_x01", "Cache Properties (IC)", "Core::X86::Cpuid::CachePropEcx1", R"(=====Core::X86::Cpuid::CachePropEcx1 reports topology information for the IC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_ECX_x01",
         {{31, 0, R"=====(CacheNumSets: cache number of sets. Read-only. Reset: Fixed,0000_00FFh. See
        Core::X86::Cpuid::CachePropEcx0[CacheNumSets].)====="}}},
        {"CPUID_Fn8000001D_ECX_x02", "Cache Properties (L2)", "Core::X86::Cpuid::CachePropEcx2", R"(=====Core::X86::Cpuid::CachePropEcx2 reports topology information for the L2. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_ECX_x02",
         {{31, 0, R"=====(CacheNumSets: cache number of sets. Read-only. Reset: Fixed,0000_03FFh. See
        Core::X86::Cpuid::CachePropEcx0[CacheNumSets].)====="}}},
        {"CPUID_Fn8000001D_ECX_x03", "Cache Properties (L3)", "Core::X86::Cpuid::CachePropEcx3", R"(=====Core::X86::Cpuid::CachePropEcx3 reports topology information for the L3.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_ECX_x03",
         {{31, 0, R"=====(CacheNumSets: cache number of sets. Read-only. Reset: 0000_XXXXh. See
        Core::X86::Cpuid::CachePropEcx0[CacheNumSets].
ValidValues:
Value     Description
0000_0 Reserved.
FFEh-
0000_0
000h
0000_0 4096 L3 Cache Sets.
FFFh
0000_1 Reserved.
FFEh-
0000_1
000h
0000_1 8192 L3 Cache Sets.
FFFh
        FFFF_F Reserved.
FFFh-
0000_2
000h)====="}}},
        {"CPUID_Fn8000001D_ECX_x04", "Cache Properties Null", "Core::X86::Cpuid::CachePropEcx4", R"(=====Core::X86::Cpuid::CachePropEax3 reports done/null. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_ECX_x04",
         {{31, 0, R"=====(CacheNumSets: cache number of sets. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn8000001D_EDX_x00", "Cache Properties (DC)", "Core::X86::Cpuid::CachePropEdx0", R"(=====Core::X86::Cpuid::CachePropEdx0 reports topology information for the DC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EDX_x00",
         {{31, 2, R"=====(Reserved)====="}, {1, 1, R"=====(CacheInclusive: cache inclusive. Read-only. Reset: Fixed,0. 0=Cache is not inclusive of lower cache levels.
1=Cache is inclusive of lower cache levels)====="},
          {0, 0, R"=====(WBINVD: Write-Back Invalidate/Invalidate. Read-only. Reset: Fixed,0. 0=WBINVD/INVD invalidates all
        lower level caches of non-originating cores sharing this cache. 1=WBINVD/INVD not ensured to invalidate all
lower level caches of non-originating cores sharing this cache.)====="}}},
        {"CPUID_Fn8000001D_EDX_x01", "Cache Properties (IC)", "Core::X86::Cpuid::CachePropEdx1", R"(=====Core::X86::Cpuid::CachePropEdx1 reports topology information for the IC. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EDX_x01",
         {{31, 2, R"=====(Reserved)====="}, {1, 1, R"=====(CacheInclusive: cache inclusive. Read-only. Reset: Fixed,0. See
        Core::X86::Cpuid::CachePropEdx0[CacheInclusive])====="},
          {0, 0, R"=====(WBINVD: Write-Back Invalidate/Invalidate. Read-only. Reset: Fixed,0. 0=WBINVD/INVD invalidates all
        lower level caches of non-originating cores sharing this cache. 1=WBINVD/INVD not guaranteed to invalidate
        all lower level caches of non-originating cores sharing this cache. See
        Core::X86::Cpuid::CachePropEdx0[WBINVD].)====="}}},
        {"CPUID_Fn8000001D_EDX_x02", "Cache Properties (L2)", "Core::X86::Cpuid::CachePropEdx2", R"(=====Core::X86::Cpuid::CachePropEdx2 reports topology information for the L2. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EDX_x02",
         {{31, 2, R"=====(Reserved)====="}, {1, 1, R"=====(CacheInclusive: cache inclusive. Read-only. Reset: Fixed,1. See
        Core::X86::Cpuid::CachePropEdx0[CacheInclusive])====="},
          {0, 0, R"=====(WBINVD: Write-Back Invalidate/Invalidate. Read-only. Reset: Fixed,0. 0=WBINVD/INVD invalidates all
        lower level caches of non-originating cores sharing this cache. 1=WBINVD/INVD not guaranteed to invalidate
        all lower level caches of non-originating cores sharing this cache.)====="}}},
        {"CPUID_Fn8000001D_EDX_x03", "Cache Properties (L3)", "Core::X86::Cpuid::CachePropEdx3", R"(=====        Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EDX_x03",
         {{31, 2, R"=====(Reserved)====="}, {1, 1, R"=====(CacheInclusive: cache inclusive. Read-only. Reset: Fixed,0. See
        Core::X86::Cpuid::CachePropEdx0[CacheInclusive])====="},
          {0, 0, R"=====(WBINVD: Write-Back Invalidate/Invalidate. Read-only. Reset: Fixed,1. 0=WBINVD/INVD invalidates all
        lower level caches of non-originating cores sharing this cache. 1=WBINVD/INVD not guaranteed to invalidate
        all lower level caches of non-originating cores sharing this cache.)====="}}},
        {"CPUID_Fn8000001D_EDX_x04", "Cache Properties Null", "Core::X86::Cpuid::CachePropEdx4", R"(=====Core::X86::Cpuid::CachePropEax3 reports done/null. See Core::X86::Cpuid::CachePropEax0.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001D_EDX_x04",
         {{31, 0, R"=====(Reserved. Read-only. Reset: Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn8000001E_EAX", "Extended APIC ID", "Core::X86::Cpuid::ExtApicId", R"(=====If (Core::X86::Msr::APIC_BAR[ApicEn] == 0) then Core::X86::Cpuid::ExtApicId[ExtendedApicId] is reserved.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001E_EAX",
         {{31, 0, R"=====(ExtendedApicId: extended APIC ID. Read-only. See 2.1.8.2.1.3 [ApicId Enumeration Requirements].
Reset: Core::X86::Msr::APIC_BAR[ApicEn] ? Fixed,{00_0000h , Core::X86::Apic::ApicId[ApicId]} :
Fixed,0000_0000h.)====="}}},
        {"CPUID_Fn8000001E_EBX", "Core Identifiers", "Core::X86::Cpuid::CoreId", R"(=====See Core::X86::Cpuid::ExtApicId.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001E_EBX",
         {{31, 16, R"=====(Reserved)====="}, {15, 8, R"=====(ThreadsPerCore: threads per core. Read-only. Reset: XXh. The number of threads per core is
        ThreadsPerCore+1)====="},
          {7, 0, R"=====(CoreId: core ID. Read-only. Reset: Fixed,XXh.
Description: For Family 17, Model 1, Revision 1 and later:
CoreId = ({2'b0, DieId[1:0], LogicalComplexId[0], LogicalThreadId[2:0]} >> SMT).)====="}}},
        {"CPUID_Fn8000001E_ECX", "Node Identifiers", "Core::X86::Cpuid::NodeId", R"(=====Read-only. Reset: 0000_0XXXh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001E_ECX",
         {{31, 11, R"=====(Reserved)====="}, {10, 8, R"=====(NodesPerProcessor: Node per processor. Read-only. Reset: XXXb.
ValidValues:
Value     Description
0h        1 node per processor.
1h        2 nodes per processor.
2h        Reserved.
3h        4 nodes per processor.
7h-4h     Reserved)====="},
          {7, 0, R"=====(NodeId: Node ID. Read-only. Reset: Fixed,XXh.)====="}}},
        {"CPUID_Fn8000001F_EAX", "AMD Secure Encryption EAX", "Core::X86::Cpuid::SecureEncryptionEax", R"(=====Read-only. Reset: 0000_000Fh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001F_EAX",
         {{31, 4, R"=====(Reserved)====="}, {3, 3, R"=====(SevEs. Read-only. Reset: Fixed,1)====="}, {2, 2, R"=====(VmPgFlush: VM Page Flush MSR is supported. Read-only. Reset: Fixed,1. See
        Core::X86::Msr::VMPAGE_FLUSH)====="},
          {1, 1, R"=====(SEV: Secure Encrypted Virtualization supported. Read-only. Reset: Fixed,1)====="},
          {0, 0, R"=====(SME: Secure Memory Encryption supported. Read-only. Reset: Fixed,1.)====="}}},
        {"CPUID_Fn8000001F_EBX", "AMD Secure Encryption EBX", "Core::X86::Cpuid::SecureEncryptionEbx", R"(=====Read-only. Reset: 0000_016Fh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001F_EBX",
         {{31, 12, R"=====(Reserved)====="}, {11, 6, R"=====(MemEncryptPhysAddWidth. Read-only. Reset: 05h. Reduction of physical address space in bits when memory
        encryption is enabled (0 indicates no reduction).
ValidValues:
Value    Description
00h      Physical Address width is not reduced.
01h      Physical Address width is reduced by one.
02h      Physical Address width is reduced by two.
03h      Physical Address width is reduced by three.
04h      Physical Address width is reduced by four.
05h      Physical Address width is reduced by five.
3Fh-06h Reserved)====="},
          {5, 0, R"=====(CBit. Read-only. Reset: 2Fh. Page table bit number used to enable memory encryption.)====="}}},
        {"CPUID_Fn8000001F_ECX", "AMD Secure Encryption ECX", "Core::X86::Cpuid::SecureEncryptionEcx", R"(=====Read-only. Reset: 0000_000Fh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001F_ECX",
         {{31, 0, R"=====(NumEncryptedGuests. Read-only. Reset: 0000_000Fh. Number of encrypted guests supported simultaneously.)====="}}},
        {"CPUID_Fn8000001F_EDX", "Minimum ASID", "Core::X86::Cpuid::SecureEncryptionEdx", R"(=====Read-only. Reset: 0000_000Xh.
=====)",
         "_lthree[1:0]_core[3:0]_thread[1:0]",
         "CPUID_Fn8000001F_EDX",
         {{31, 0, R"=====(MinimumSEVASID: Minimum SEV enabled, SEV-ES disabled ASID. Read-only. Reset: 0000_000Xh.
Indicates the minimum ASID value that must be used for an SEV-enabled, SEV-ES-disabled guest.)====="}}}};
