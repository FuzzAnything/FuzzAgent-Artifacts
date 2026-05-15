// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/namedb.h"
#include "pcap/pcap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzz data for functions that need strings
    // Limit size to avoid overly large allocations
    size_t str_size = Size > 1024 ? 1024 : Size;
    char *str = (char *)malloc(str_size + 1);
    if (!str) {
        return 0;
    }
    memcpy(str, Data, str_size);
    str[str_size] = '\0';

    // Buffer for error messages
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // 1. Test pcap_nametoaddrinfo
    struct addrinfo *addr_info = pcap_nametoaddrinfo(str);
    if (addr_info) {
        freeaddrinfo(addr_info);
    }

    // 2. Test pcap_findalldevs
    pcap_if_t *alldevs = NULL;
    int ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }

    // 3. Test pcap_parsesrcstr
    int type;
    char host[256];
    char port[256];
    char name[256];
    pcap_parsesrcstr(str, &type, host, port, name, errbuf);

    // 4. Test pcap_createsrcstr
    char source[256];
    // Use some reasonable values for the parameters
    const char *test_host = "example.com";
    const char *test_port = "80";
    const char *test_name = "eth0";
    
    // Test with different types
    int types[] = {PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE, PCAP_SRC_FILE, PCAP_SRC_IFREMOTE};
    for (int i = 0; i < 4; i++) {
        pcap_createsrcstr(source, types[i], test_host, test_port, test_name, errbuf);
    }

    // 5. Test pcap_open
    // Create a dummy file for file-based capture
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy_file);
        fclose(dummy_file);
        
        // Try to open as file
        pcap_t *pcap_handle = pcap_open("./dummy_file", 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
        if (pcap_handle) {
            pcap_close(pcap_handle);
        }
        
        // Try with "any" device
        pcap_handle = pcap_open("any", 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
        if (pcap_handle) {
            pcap_close(pcap_handle);
        }
        
        // Try with the fuzz string as source
        pcap_handle = pcap_open(str, 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
        if (pcap_handle) {
            pcap_close(pcap_handle);
        }
        
        remove("./dummy_file");
    }

    // 6. Test pcap_findalldevs_ex
    pcap_if_t *alldevs_ex = NULL;
    struct pcap_rmtauth auth;
    auth.type = RPCAP_RMTAUTH_NULL;
    auth.username = NULL;
    auth.password = NULL;
    
    // Test with different source types
    const char *sources[] = {"rpcap://", "file://./dummy_file", str};
    for (int i = 0; i < 3; i++) {
        ret = pcap_findalldevs_ex(sources[i], &auth, &alldevs_ex, errbuf);
        if (ret == 0 && alldevs_ex) {
            pcap_freealldevs(alldevs_ex);
            alldevs_ex = NULL;
        }
    }

    free(str);
    return 0;
}