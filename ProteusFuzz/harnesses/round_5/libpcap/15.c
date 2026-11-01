// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    struct pcap_rmtauth auth;

    // Initialize auth structure
    memset(&auth, 0, sizeof(auth));
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;

    // Prepare source string from input data
    char source[256];
    size_t src_len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    if (src_len > 0) {
        memcpy(source, Data, src_len);
        source[src_len] = '\0';
    } else {
        strcpy(source, "test_source");
    }

    // 1. First call to pcap_findalldevs_ex
    pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);

    // 2. Second call to pcap_findalldevs_ex (with potentially different source or same)
    pcap_findalldevs_ex(source, NULL, &alldevs2, errbuf);

    // 3. Call to pcap_findalldevs
    pcap_findalldevs(&alldevs3, errbuf);

    // 4. Call to pcap_lookupnet
    // Try with "any" which is a valid pseudo-device
    pcap_lookupnet("any", &net, &mask, errbuf);
    
    // If we have devices from the previous calls, try to lookup one of them
    if (alldevs3 != NULL) {
        pcap_lookupnet(alldevs3->name, &net, &mask, errbuf);
    } else if (alldevs2 != NULL) {
        pcap_lookupnet(alldevs2->name, &net, &mask, errbuf);
    } else if (alldevs != NULL) {
        pcap_lookupnet(alldevs->name, &net, &mask, errbuf);
    }

    // 5. Cleanup: Free all device lists
    pcap_freealldevs(alldevs);
    pcap_freealldevs(alldevs2);
    pcap_freealldevs(alldevs3);

    return 0;
}