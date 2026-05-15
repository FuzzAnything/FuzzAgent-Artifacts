// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdlib.h>
#include <string.h>

static void free_auth(struct pcap_rmtauth *auth) {
    if (auth) {
        free(auth->username);
        free(auth->password);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs1 = NULL, *alldevs2 = NULL, *alldevs3 = NULL;
    struct pcap_rmtauth auth;
    bpf_u_int32 netp, maskp;
    
    // Initialize error buffers
    memset(errbuf, 0, sizeof(errbuf));
    
    // First pcap_findalldevs_ex call with NULL auth
    {
        // Create source string from fuzz data (null-terminated)
        char *source = NULL;
        if (Size > 0) {
            size_t source_len = Size > 255 ? 255 : Size;
            source = (char *)malloc(source_len + 1);
            if (source) {
                memcpy(source, Data, source_len);
                source[source_len] = '\0';
                pcap_findalldevs_ex(source, NULL, &alldevs1, errbuf);
                free(source);
            }
        } else {
            // Try with empty string
            pcap_findalldevs_ex("", NULL, &alldevs1, errbuf);
        }
    }
    
    // Second pcap_findalldevs_ex call with auth structure
    {
        // Initialize auth structure from fuzz data
        memset(&auth, 0, sizeof(auth));
        
        // Set auth type (use first 4 bytes if available)
        if (Size >= sizeof(int)) {
            auth.type = *(const int *)Data;
        }
        
        // Create username from fuzz data
        if (Size > 4) {
            size_t username_len = (Size - 4) > 127 ? 127 : (Size - 4);
            auth.username = (char *)malloc(username_len + 1);
            if (auth.username) {
                memcpy(auth.username, Data + 4, username_len);
                auth.username[username_len] = '\0';
            }
        }
        
        // Create password from remaining fuzz data
        if (Size > 132) {
            size_t password_len = (Size - 132) > 127 ? 127 : (Size - 132);
            auth.password = (char *)malloc(password_len + 1);
            if (auth.password) {
                memcpy(auth.password, Data + 132, password_len);
                auth.password[password_len] = '\0';
            }
        }
        
        // Try different source types
        const char *sources[] = {"rpcap://", "file://./dummy_file", "any", NULL};
        for (int i = 0; sources[i] != NULL; i++) {
            pcap_findalldevs_ex(sources[i], &auth, &alldevs2, errbuf);
        }
        
        // Clean up auth strings
        free_auth(&auth);
    }
    
    // pcap_findalldevs call
    pcap_findalldevs(&alldevs3, errbuf);
    
    // pcap_lookupnet calls on found devices
    pcap_if_t *dev_lists[] = {alldevs1, alldevs2, alldevs3};
    for (int i = 0; i < 3; i++) {
        for (pcap_if_t *d = dev_lists[i]; d != NULL; d = d->next) {
            if (d->name) {
                pcap_lookupnet(d->name, &netp, &maskp, errbuf);
                // Try with NULL device name
                pcap_lookupnet(NULL, &netp, &maskp, errbuf);
                // Try with "any"
                pcap_lookupnet("any", &netp, &maskp, errbuf);
            }
        }
    }
    
    // pcap_freealldevs calls
    pcap_freealldevs(alldevs1);
    pcap_freealldevs(alldevs2);
    pcap_freealldevs(alldevs3);
    
    return 0;
}