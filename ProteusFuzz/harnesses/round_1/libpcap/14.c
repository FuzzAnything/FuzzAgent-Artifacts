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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs1 = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    
    // Prepare source string from input data
    char source[256];
    size_t source_len = (Size > 255) ? 255 : Size;
    memcpy(source, Data, source_len);
    source[source_len] = '\0';

    // Prepare auth structure
    struct pcap_rmtauth auth;
    memset(&auth, 0, sizeof(auth));
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    // 1. First call to pcap_findalldevs_ex
    if (pcap_findalldevs_ex(source, &auth, &alldevs1, errbuf) == 0) {
        // Successfully populated alldevs1
    } else {
        // Error occurred, alldevs1 remains NULL
    }

    // 2. Second call to pcap_findalldevs_ex with potentially modified source
    // Use a different slice of input or modify slightly
    char source2[256];
    size_t source2_len = (Size > 255) ? 255 : Size;
    memcpy(source2, Data, source2_len);
    source2[source2_len] = '\0';
    // Flip first bit to create variation
    if (source2_len > 0) {
        source2[0] ^= 1;
    }

    if (pcap_findalldevs_ex(source2, &auth, &alldevs2, errbuf) == 0) {
        // Successfully populated alldevs2
    }

    // 3. Call to pcap_findalldevs
    if (pcap_findalldevs(&alldevs3, errbuf) == 0) {
        // Successfully populated alldevs3
    }

    // 4. Call to pcap_lookupnet
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    
    // Try to lookup net for the first device in each list if available
    if (alldevs1 && alldevs1->name) {
        pcap_lookupnet(alldevs1->name, &net, &mask, errbuf);
    }
    if (alldevs2 && alldevs2->name) {
        pcap_lookupnet(alldevs2->name, &net, &mask, errbuf);
    }
    if (alldevs3 && alldevs3->name) {
        pcap_lookupnet(alldevs3->name, &net, &mask, errbuf);
    }
    
    // Also try with a device name derived from input if it looks like a valid name
    char dev_name[64];
    size_t dev_name_len = (Size > 63) ? 63 : Size;
    memcpy(dev_name, Data, dev_name_len);
    dev_name[dev_name_len] = '\0';
    pcap_lookupnet(dev_name, &net, &mask, errbuf);

    // 5. Cleanup: Free all device lists
    pcap_freealldevs(alldevs1);
    pcap_freealldevs(alldevs2);
    pcap_freealldevs(alldevs3);

    return 0;
}