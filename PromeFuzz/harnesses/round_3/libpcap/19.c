// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <string.h>

static void fuzz_lookupnet_on_devices(pcap_if_t *devices) {
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    
    for (pcap_if_t *dev = devices; dev != NULL; dev = dev->next) {
        pcap_lookupnet(dev->name, &net, &mask, errbuf);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs_ex1 = NULL;
    pcap_if_t *alldevs_ex2 = NULL;
    pcap_if_t *alldevs = NULL;
    
    struct pcap_rmtauth auth;
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // First pcap_findalldevs_ex call
    if (Size > 0) {
        char source[256];
        size_t source_len = Size < 255 ? Size : 255;
        memcpy(source, Data, source_len);
        source[source_len] = '\0';
        
        pcap_findalldevs_ex(source, &auth, &alldevs_ex1, errbuf);
    }
    
    // Second pcap_findalldevs_ex call
    if (Size > 1) {
        char source2[256];
        size_t source2_len = (Size/2) < 255 ? (Size/2) : 255;
        memcpy(source2, Data + (Size/2), source2_len);
        source2[source2_len] = '\0';
        
        pcap_findalldevs_ex(source2, &auth, &alldevs_ex2, errbuf);
    }
    
    // pcap_findalldevs call
    pcap_findalldevs(&alldevs, errbuf);
    
    // pcap_lookupnet calls on all found devices
    fuzz_lookupnet_on_devices(alldevs_ex1);
    fuzz_lookupnet_on_devices(alldevs_ex2);
    fuzz_lookupnet_on_devices(alldevs);
    
    // Cleanup
    pcap_freealldevs(alldevs_ex1);
    pcap_freealldevs(alldevs_ex2);
    pcap_freealldevs(alldevs);
    
    return 0;
}