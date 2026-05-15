// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void cleanup_addrinfo(struct addrinfo *ai) {
    if (ai) {
        freeaddrinfo(ai);
    }
}

static void cleanup_devs(pcap_if_t *alldevs) {
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Prepare error buffers
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char source[PCAP_BUF_SIZE] = {0};
    char host[PCAP_BUF_SIZE] = {0};
    char port[PCAP_BUF_SIZE] = {0};
    char name[PCAP_BUF_SIZE] = {0};

    // Use first byte to select different code paths
    uint8_t selector = Data[0] % 6;

    switch (selector) {
        case 0: {
            // Test pcap_nametoaddrinfo
            struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
            cleanup_addrinfo(ai);
            break;
        }
        case 1: {
            // Test pcap_parsesrcstr
            int type = 0;
            pcap_parsesrcstr(input_str, &type, host, port, name, errbuf);
            break;
        }
        case 2: {
            // Test pcap_findalldevs_ex with NULL auth
            pcap_if_t *alldevs = NULL;
            pcap_findalldevs_ex(input_str, NULL, &alldevs, errbuf);
            cleanup_devs(alldevs);
            break;
        }
        case 3: {
            // Test pcap_createsrcstr with various types
            int types[] = {PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE, PCAP_SRC_FILE};
            int type_idx = Data[0] % 3;
            
            // Use parts of input for host/port/name if available
            size_t third = Size / 3;
            if (third > 0) {
                size_t host_len = (third < PCAP_BUF_SIZE - 1) ? third : PCAP_BUF_SIZE - 1;
                size_t port_len = (third < PCAP_BUF_SIZE - 1) ? third : PCAP_BUF_SIZE - 1;
                size_t name_len = (Size - 2*third < PCAP_BUF_SIZE - 1) ? Size - 2*third : PCAP_BUF_SIZE - 1;
                
                memcpy(host, Data, host_len);
                host[host_len] = '\0';
                memcpy(port, Data + third, port_len);
                port[port_len] = '\0';
                memcpy(name, Data + 2*third, name_len);
                name[name_len] = '\0';
            }
            
            pcap_createsrcstr(source, types[type_idx], host, port, name, errbuf);
            break;
        }
        case 4: {
            // Test pcap_open with various parameters
            int snaplen = (Data[0] << 8) | Data[1 % Size];
            int flags = (Data[2 % Size] << 8) | Data[3 % Size];
            int read_timeout = (Data[4 % Size] << 8) | Data[5 % Size];
            
            pcap_t *pcap = pcap_open(input_str, snaplen, flags, read_timeout, NULL, errbuf);
            cleanup_pcap(pcap);
            break;
        }
        case 5: {
            // Test pcap_findalldevs
            pcap_if_t *alldevs = NULL;
            pcap_findalldevs(&alldevs, errbuf);
            cleanup_devs(alldevs);
            break;
        }
    }

    // Additional test: Try pcap_parsesrcstr with the result from pcap_createsrcstr
    if (source[0] != '\0') {
        int type = 0;
        pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    }

    free(input_str);
    return 0;
}