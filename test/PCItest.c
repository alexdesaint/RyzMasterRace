#include "stdio.h"
#include "pci/pci.h"
#include "zconf.h"
#include "cpuid.h"

unsigned int ReadDword(struct pci_dev* dev, unsigned int value)
{
    pci_write_word(dev, 0xB8, value);
    //Thread.Sleep(SMUDelay);
    usleep(100);
    return pci_read_word(dev, 0xBC);
}

int main() {
    struct pci_access* pciaccess;
    struct pci_dev* dev;
    char namebuf[1024];

    pciaccess = pci_alloc();
    pci_init(pciaccess);
    dev = pci_get_dev(pciaccess, 0, 0, 0, 0);

    pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);
    pci_lookup_name(pciaccess, namebuf, sizeof(namebuf), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);
    printf("%-100s %2u %2u %2u %2u\n", namebuf, dev->domain, dev->bus, dev->dev, dev->func);

    uint64_t eax = 0, ebx = 0, ecx = 0, edx = 0;

    __cpuid_count(0x80000001, 0x01, eax, ebx, ecx, edx);

    uint CPUFMS = eax & 0xFFFF00;

    if (CPUFMS != 0x00800F00 && CPUFMS != 0x00810F00 && CPUFMS != 0x00870F00)
    {
        printf("The software only supports RV & ZP based Ryzen CPUs");
        pci_cleanup(pciaccess);
        return 1;
    }

    uint SMUORG = pci_read_word(dev, 0xB8);
    printf("%u\n", SMUORG);
    usleep(100);
    uint someOffset = ReadDword(dev, 0x50200) == 0x300 ? 0x100000u : 0x0u;
    printf("%u\n", someOffset);
    // Read data
    uint BGS = ReadDword(dev, 0x00050058 + someOffset);
    uint BGSA = ReadDword(dev, 0x000500D0 + someOffset);

    uint DramConfiguration = ReadDword(dev, 0x00050200 + someOffset);

    uint DramTiming1 = ReadDword(dev, 0x00050204 + someOffset);
    uint DramTiming2 = ReadDword(dev, 0x00050208 + someOffset);
    uint DramTiming3 = ReadDword(dev, 0x0005020C + someOffset);
    uint DramTiming4 = ReadDword(dev, 0x00050210 + someOffset);
    uint DramTiming5 = ReadDword(dev, 0x00050214 + someOffset);
    uint DramTiming6 = ReadDword(dev, 0x00050218 + someOffset);
    uint DramTiming7 = ReadDword(dev, 0x0005021C + someOffset);
    uint DramTiming8 = ReadDword(dev, 0x00050220 + someOffset);
    uint DramTiming9 = ReadDword(dev, 0x00050224 + someOffset);
    uint DramTiming10 = ReadDword(dev, 0x00050228 + someOffset);
    // 11?
    uint DramTiming12 = ReadDword(dev, 0x00050230 + someOffset);
    uint DramTiming13 = ReadDword(dev, 0x00050234 + someOffset);
    uint DramTiming20 = ReadDword(dev, 0x00050250 + someOffset);
    uint DramTiming21 = ReadDword(dev, 0x00050254 + someOffset);
    uint DramTiming22 = ReadDword(dev, 0x00050258 + someOffset);

    uint tRFCTiming0 = ReadDword(dev, 0x00050260 + someOffset);
    uint tRFCTiming1 = ReadDword(dev, 0x00050264 + someOffset);

    uint tSTAGTiming0 = ReadDword(dev, 0x00050270 + someOffset);
    uint tSTAGTiming1 = ReadDword(dev, 0x00050274 + someOffset);

    uint DramTiming35 = ReadDword(dev, 0x0005028C + someOffset);

    printf("%X", BGS);
    pci_cleanup(pciaccess);
    return 0;
}