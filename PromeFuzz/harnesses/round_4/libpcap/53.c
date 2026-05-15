// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Prepare string from fuzz data (null-terminated)
    char *fuzz_str = (char *)malloc(Size + 1);
    if (!fuzz_str) return 0;
    memcpy(fuzz_str, Data, Size);
    fuzz_str[Size] = '\0';

    // 1. Test pcap_findalldevs_ex
    {
        pcap_if_t *alldevs = NULL;
        struct pcap_rmtauth auth;
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
        
        // Use fuzz_str as source
        pcap_findalldevs_ex(fuzz_str, &auth, &alldevs, errbuf);
        if (alldevs) {
            pcap_freealldevs(alldevs);
        }
    }

    // 2. Test pcap_nametoaddrinfo
    {
        struct addrinfo *addr = pcap_nametoaddrinfo(fuzz_str);
        if (addr) {
            freeaddrinfo(addr);
        }
    }

    // 3. Test pcap_parsesrcstr
    {
        int type;
        char host[256], port[256], name[256];
        memset(host, 0, sizeof(host));
        memset(port, 0, sizeof(port));
        memset(name, 0, sizeof(name));
        
        pcap_parsesrcstr(fuzz_str, &type, host, port, name, errbuf);
    }

    // 4. Test pcap_open
    {
        // Use fuzz_str as source
        pcap_t *pcap = pcap_open(fuzz_str, 65535, 0, 1000, NULL, errbuf);
        if (pcap) {
            pcap_close(pcap);
        }
    }

    // 5. Test pcap_findalldevs
    {
        pcap_if_t *alldevs = NULL;
        pcap_findalldevs(&alldevs, errbuf);
        if (alldevs) {
            pcap_freealldevs(alldevs);
        }
    }

    // 6. Test pcap_createsrcstr
    {
        char source[256];
        memset(source, 0, sizeof(source));
        
        // Use fuzz_str for host, port, and name parameters
        int type = (Data[0] % 3) + 1; // Random type between 1-3
        
        pcap_createsrcstr(source, type, fuzz_str, fuzz_str, fuzz_str, errbuf);
    }

    free(fuzz_str);
    return 0;
}