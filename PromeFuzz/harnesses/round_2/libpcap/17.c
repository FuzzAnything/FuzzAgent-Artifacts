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
#include "code/pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PCAP_ERRBUF_SIZE 256

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    struct pcap_rmtauth auth;
    bpf_u_int32 net, mask;
    
    // Initialize auth structure using fuzzer data
    if (Size >= sizeof(auth.type)) {
        auth.type = *(int*)Data;
        Data += sizeof(auth.type);
        Size -= sizeof(auth.type);
    } else {
        auth.type = 0;
    }
    
    // Use fuzzer data for username and password if available
    static char username[256];
    static char password[256];
    
    if (Size > 0) {
        size_t user_len = Size > 255 ? 255 : Size;
        memcpy(username, Data, user_len);
        username[user_len] = '\0';
        auth.username = username;
        
        if (Size > 1) {
            size_t pass_len = (Size - 1) > 255 ? 255 : (Size - 1);
            memcpy(password, Data + 1, pass_len);
            password[pass_len] = '\0';
            auth.password = password;
        } else {
            auth.password = NULL;
        }
    } else {
        auth.username = NULL;
        auth.password = NULL;
    }
    
    // First pcap_findalldevs_ex call
    pcap_findalldevs_ex("rpcap://", &auth, &alldevs, errbuf);
    
    // Second pcap_findalldevs_ex call with different source
    pcap_findalldevs_ex("any", NULL, &alldevs2, errbuf);
    
    // pcap_findalldevs call
    pcap_findalldevs(&alldevs3, errbuf);
    
    // pcap_lookupnet call - try with first device if available
    if (alldevs3 != NULL && alldevs3->name != NULL) {
        pcap_lookupnet(alldevs3->name, &net, &mask, errbuf);
    } else {
        // Try with common device names
        pcap_lookupnet("lo", &net, &mask, errbuf);
        pcap_lookupnet("eth0", &net, &mask, errbuf);
        pcap_lookupnet("any", &net, &mask, errbuf);
    }
    
    // pcap_freealldevs call - free all device lists
    pcap_freealldevs(alldevs);
    pcap_freealldevs(alldevs2);
    pcap_freealldevs(alldevs3);
    
    return 0;
}