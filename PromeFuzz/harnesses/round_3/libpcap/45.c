// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
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
#include <errno.h>

static void free_addrinfo_safe(struct addrinfo *ai) {
    if (ai) {
        freeaddrinfo(ai);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Buffer for error messages
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Function selector based on first byte
    uint8_t selector = Data[0] % 6;

    switch (selector) {
        case 0: {
            // pcap_nametoaddrinfo
            struct addrinfo *result = pcap_nametoaddrinfo(input_str);
            free_addrinfo_safe(result);
            break;
        }

        case 1: {
            // pcap_findalldevs
            pcap_if_t *alldevs = NULL;
            int ret = pcap_findalldevs(&alldevs, errbuf);
            if (ret == 0 && alldevs) {
                pcap_freealldevs(alldevs);
            }
            break;
        }

        case 2: {
            // pcap_parsesrcstr
            int type;
            char host[256], port[256], name[256];
            memset(host, 0, sizeof(host));
            memset(port, 0, sizeof(port));
            memset(name, 0, sizeof(name));
            
            pcap_parsesrcstr(input_str, &type, host, port, name, errbuf);
            break;
        }

        case 3: {
            // pcap_createsrcstr
            char source[256];
            memset(source, 0, sizeof(source));
            
            // Split input into three parts for host, port, name
            char *host = input_str;
            char *port = strchr(host, '|');
            char *name = NULL;
            
            if (port) {
                *port = '\0';
                port++;
                name = strchr(port, '|');
                if (name) {
                    *name = '\0';
                    name++;
                }
            }
            
            int type = PCAP_SRC_IFREMOTE;
            if (Size > 1) {
                type = Data[1] % 4; // Various source types
            }
            
            pcap_createsrcstr(source, type, host, port, name, errbuf);
            break;
        }

        case 4: {
            // pcap_open
            int snaplen = 65535;
            int flags = PCAP_OPENFLAG_PROMISCUOUS;
            int read_timeout = 1000;
            
            // Create dummy auth structure
            struct pcap_rmtauth auth;
            auth.type = RPCAP_RMTAUTH_NULL;
            auth.username = NULL;
            auth.password = NULL;
            
            pcap_t *pcap = pcap_open(input_str, snaplen, flags, read_timeout, &auth, errbuf);
            if (pcap) {
                pcap_close(pcap);
            }
            break;
        }

        case 5: {
            // pcap_findalldevs_ex
            pcap_if_t *alldevs = NULL;
            
            // Create dummy auth structure
            struct pcap_rmtauth auth;
            auth.type = RPCAP_RMTAUTH_NULL;
            auth.username = NULL;
            auth.password = NULL;
            
            int ret = pcap_findalldevs_ex(input_str, &auth, &alldevs, errbuf);
            if (ret == 0 && alldevs) {
                pcap_freealldevs(alldevs);
            }
            break;
        }
    }

    free(input_str);
    return 0;
}