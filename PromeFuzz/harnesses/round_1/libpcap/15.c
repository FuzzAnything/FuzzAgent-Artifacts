// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_rmtauth(struct pcap_rmtauth *auth, const uint8_t *data, size_t size) {
    if (size == 0) {
        auth->type = 0;
        auth->username = NULL;
        auth->password = NULL;
        return;
    }
    
    auth->type = data[0] % 3;
    
    size_t username_len = (size > 1) ? (data[1] % (size - 1)) : 0;
    if (username_len > 0 && username_len <= size - 2) {
        auth->username = (char *)malloc(username_len + 1);
        if (auth->username) {
            memcpy(auth->username, data + 2, username_len);
            auth->username[username_len] = '\0';
        }
    } else {
        auth->username = NULL;
    }
    
    size_t password_start = 2 + username_len;
    if (password_start < size) {
        size_t remaining = size - password_start;
        size_t password_len = (remaining > 0) ? (data[password_start % size] % remaining) : 0;
        
        if (password_len > 0 && password_len <= remaining) {
            auth->password = (char *)malloc(password_len + 1);
            if (auth->password) {
                memcpy(auth->password, data + password_start, password_len);
                auth->password[password_len] = '\0';
            }
        } else {
            auth->password = NULL;
        }
    } else {
        auth->password = NULL;
    }
}

static void cleanup_rmtauth(struct pcap_rmtauth *auth) {
    if (auth->username) {
        free(auth->username);
        auth->username = NULL;
    }
    if (auth->password) {
        free(auth->password);
        auth->password = NULL;
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs_ex1 = NULL;
    pcap_if_t *alldevs_ex2 = NULL;
    pcap_if_t *alldevs = NULL;
    
    struct pcap_rmtauth auth;
    
    if (Size == 0) {
        return 0;
    }
    
    init_rmtauth(&auth, Data, Size);
    
    size_t source_len = (Size > 0) ? (Data[0] % Size) : 0;
    char *source = (char *)malloc(source_len + 1);
    if (source && source_len > 0 && source_len <= Size) {
        memcpy(source, Data, source_len);
        source[source_len] = '\0';
    } else if (source) {
        source[0] = '\0';
    }
    
    pcap_findalldevs_ex(source, &auth, &alldevs_ex1, errbuf);
    
    if (source) {
        free(source);
    }
    
    size_t source2_len = (Size > 1) ? (Data[1] % Size) : 0;
    char *source2 = (char *)malloc(source2_len + 1);
    if (source2 && source2_len > 0) {
        size_t offset = (source_len % Size);
        if (offset + source2_len <= Size) {
            memcpy(source2, Data + offset, source2_len);
            source2[source2_len] = '\0';
        } else {
            size_t safe_len = (Size > offset) ? (Size - offset) : 0;
            if (safe_len > 0) {
                memcpy(source2, Data + offset, safe_len);
                source2[safe_len] = '\0';
            } else {
                source2[0] = '\0';
            }
        }
    } else if (source2) {
        source2[0] = '\0';
    }
    
    pcap_findalldevs_ex(source2, &auth, &alldevs_ex2, errbuf);
    
    if (source2) {
        free(source2);
    }
    
    pcap_findalldevs(&alldevs, errbuf);
    
    bpf_u_int32 netp, maskp;
    const char *device_name = "any";
    
    if (alldevs_ex1) {
        device_name = alldevs_ex1->name;
    } else if (alldevs_ex2) {
        device_name = alldevs_ex2->name;
    } else if (alldevs) {
        device_name = alldevs->name;
    }
    
    pcap_lookupnet(device_name, &netp, &maskp, errbuf);
    
    if (alldevs_ex1) {
        pcap_freealldevs(alldevs_ex1);
    }
    
    if (alldevs_ex2) {
        pcap_freealldevs(alldevs_ex2);
    }
    
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    cleanup_rmtauth(&auth);
    
    return 0;
}