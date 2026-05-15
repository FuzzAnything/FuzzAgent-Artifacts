// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static char* extract_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    size_t len = (*data)[0] % 256;
    if (len > *size - 1) len = *size - 1;
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data + 1, len);
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

static int extract_int(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        int val = (*size > 0) ? (int)(*data)[0] : 0;
        *data += (*size > 0) ? 1 : 0;
        *size -= (*size > 0) ? 1 : 0;
        return val;
    }
    
    int val;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    const uint8_t *cursor = Data;
    size_t remaining = Size;
    
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    // Test pcap_init
    if (remaining > 0) {
        unsigned int opts = extract_int(&cursor, &remaining);
        pcap_init(opts, errbuf);
    }
    
    // Test pcap_lookupnet
    if (remaining > 0) {
        char *device = extract_string(&cursor, &remaining);
        bpf_u_int32 net, mask;
        
        if (device) {
            pcap_lookupnet(device, &net, &mask, errbuf);
            free(device);
        } else {
            pcap_lookupnet("any", &net, &mask, errbuf);
        }
    }
    
    // Test pcap_open_live
    if (remaining > 0) {
        char *device = extract_string(&cursor, &remaining);
        int snaplen = extract_int(&cursor, &remaining);
        int promisc = extract_int(&cursor, &remaining);
        int timeout = extract_int(&cursor, &remaining);
        
        pcap_t *pcap = pcap_open_live(device ? device : "any", 
                                      snaplen, promisc, timeout, errbuf);
        if (pcap) {
            // Test pcap_set_snaplen on inactive handle
            int new_snaplen = extract_int(&cursor, &remaining);
            pcap_set_snaplen(pcap, new_snaplen);
            
            // Test pcap_set_timeout on inactive handle
            int new_timeout = extract_int(&cursor, &remaining);
            pcap_set_timeout(pcap, new_timeout);
            
            pcap_close(pcap);
        }
        
        if (device) free(device);
    }
    
    // Test pcap_open with file source
    write_dummy_file(cursor, remaining);
    
    if (remaining > 0) {
        char *source = extract_string(&cursor, &remaining);
        int snaplen = extract_int(&cursor, &remaining);
        int flags = extract_int(&cursor, &remaining);
        int read_timeout = extract_int(&cursor, &remaining);
        
        // Create auth structure if we have enough data
        struct pcap_rmtauth auth;
        if (remaining > 8) {
            auth.type = extract_int(&cursor, &remaining);
            auth.username = extract_string(&cursor, &remaining);
            auth.password = extract_string(&cursor, &remaining);
        } else {
            auth.type = 0;
            auth.username = NULL;
            auth.password = NULL;
        }
        
        pcap_t *pcap = pcap_open(source ? source : "file:./dummy_file",
                                 snaplen, flags, read_timeout, 
                                 (remaining > 0) ? &auth : NULL, errbuf);
        if (pcap) {
            pcap_close(pcap);
        }
        
        if (source) free(source);
        if (auth.username) free(auth.username);
        if (auth.password) free(auth.password);
    }
    
    // Test pcap_open with live source
    if (remaining > 0) {
        char *source = extract_string(&cursor, &remaining);
        int snaplen = extract_int(&cursor, &remaining);
        int flags = extract_int(&cursor, &remaining);
        int read_timeout = extract_int(&cursor, &remaining);
        
        pcap_t *pcap = pcap_open(source ? source : "any",
                                 snaplen, flags, read_timeout, NULL, errbuf);
        if (pcap) {
            pcap_close(pcap);
        }
        
        if (source) free(source);
    }
    
    cleanup_dummy_file();
    return 0;
}