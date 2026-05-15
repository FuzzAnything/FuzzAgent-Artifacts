// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_addrinfo(struct addrinfo *ai) {
    if (ai) {
        freeaddrinfo(ai);
    }
}

static void cleanup_devs(pcap_if_t *devs) {
    if (devs) {
        pcap_freealldevs(devs);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Buffer for error messages
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // 1. Test pcap_nametoaddrinfo
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(ai);

    // 2. Test pcap_parsesrcstr
    int type;
    char host[256];
    char port[256];
    char name[256];
    pcap_parsesrcstr(input_str, &type, host, port, name, errbuf);

    // 3. Test pcap_findalldevs_ex
    pcap_if_t *alldevs_ex = NULL;
    pcap_findalldevs_ex(input_str, NULL, &alldevs_ex, errbuf);
    cleanup_devs(alldevs_ex);

    // 4. Test pcap_createsrcstr
    char source[1024];
    // Use parts of input for different parameters
    size_t len = strlen(input_str);
    size_t third = len / 3;
    char *host_part = input_str;
    char *port_part = (third > 0 && len > third) ? input_str + third : "";
    char *name_part = (third > 0 && len > 2*third) ? input_str + 2*third : "";
    
    // Try different source types
    int types_to_try[] = {PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE, PCAP_SRC_FILE};
    for (size_t i = 0; i < sizeof(types_to_try)/sizeof(types_to_try[0]); i++) {
        pcap_createsrcstr(source, types_to_try[i], host_part, port_part, name_part, errbuf);
    }

    // 5. Test pcap_open
    // Try with different combinations of parameters
    int snaplen_values[] = {68, 96, 1514, 65535};
    int flags_values[] = {0, PCAP_OPENFLAG_PROMISCUOUS, PCAP_OPENFLAG_MAX_RESPONSIVENESS};
    int timeout_values[] = {0, 1000, -1};
    
    for (size_t i = 0; i < sizeof(snaplen_values)/sizeof(snaplen_values[0]); i++) {
        for (size_t j = 0; j < sizeof(flags_values)/sizeof(flags_values[0]); j++) {
            for (size_t k = 0; k < sizeof(timeout_values)/sizeof(timeout_values[0]); k++) {
                pcap_t *pcap = pcap_open(input_str, snaplen_values[i], flags_values[j], 
                                        timeout_values[k], NULL, errbuf);
                cleanup_pcap(pcap);
            }
        }
    }

    // 6. Test pcap_findalldevs
    pcap_if_t *alldevs = NULL;
    pcap_findalldevs(&alldevs, errbuf);
    cleanup_devs(alldevs);

    // Cleanup
    free(input_str);
    
    return 0;
}