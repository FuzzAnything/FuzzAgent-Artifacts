// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to select different code paths
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Prepare error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Prepare authentication structure
    struct pcap_rmtauth auth;
    char username[100] = {0};
    char password[100] = {0};
    
    // Extract username and password from fuzzer input if available
    if (offset + 2 < Size) {
        size_t user_len = Data[offset] % 50;
        offset++;
        if (offset + user_len < Size) {
            memcpy(username, Data + offset, user_len);
            username[user_len] = '\0';
            offset += user_len;
        }
        
        size_t pass_len = Data[offset] % 50;
        offset++;
        if (offset + pass_len < Size) {
            memcpy(password, Data + offset, pass_len);
            password[pass_len] = '\0';
            offset += pass_len;
        }
    }
    
    auth.type = (selector & 0x1) ? RPCAP_RMTAUTH_NULL : RPCAP_RMTAUTH_PWD;
    auth.username = (selector & 0x1) ? NULL : username;
    auth.password = (selector & 0x1) ? NULL : password;
    
    // Extract parameters from fuzzer input
    int snaplen = 65535;
    int flags = 0;
    int read_timeout = 1000;
    int buffer_size = 1024 * 1024;
    int timeout = 1000;
    
    if (offset + 4 < Size) {
        snaplen = (Data[offset] << 8) | Data[offset + 1];
        flags = Data[offset + 2];
        read_timeout = (Data[offset + 3] << 8) | Data[offset + 4];
        offset += 5;
    }
    
    if (offset + 2 < Size) {
        buffer_size = (Data[offset] << 8) | Data[offset + 1];
        timeout = (Data[offset + 2] << 8) | Data[offset + 3];
        offset += 4;
    }
    
    // Create source string from fuzzer input
    char source[256] = {0};
    size_t source_len = (Size - offset) % 255;
    if (source_len > 0) {
        memcpy(source, Data + offset, source_len);
        source[source_len] = '\0';
    } else {
        strcpy(source, "any");
    }
    
    // Test pcap_open with file source
    if (selector & 0x2) {
        write_dummy_file(Data, Size);
        pcap_t *pcap = pcap_open("./dummy_file", snaplen, flags, read_timeout, &auth, errbuf);
        if (pcap) {
            // Test functions on the opened pcap handle
            pcap_setsampling(pcap);
            pcap_set_buffer_size(pcap, buffer_size);
            pcap_set_timeout(pcap, timeout);
            pcap_set_snaplen(pcap, snaplen);
            cleanup_pcap(pcap);
        }
        remove("./dummy_file");
    }
    
    // Test pcap_open_live
    if (selector & 0x4) {
        pcap_t *pcap = pcap_open_live(source, snaplen, flags, timeout, errbuf);
        if (pcap) {
            // Test functions on the live capture handle
            pcap_setsampling(pcap);
            pcap_set_buffer_size(pcap, buffer_size);
            pcap_set_timeout(pcap, timeout);
            pcap_set_snaplen(pcap, snaplen);
            cleanup_pcap(pcap);
        }
    }
    
    // Test pcap_open with network source
    if (selector & 0x8) {
        pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
        if (pcap) {
            // Test functions on the opened pcap handle
            pcap_setsampling(pcap);
            pcap_set_buffer_size(pcap, buffer_size);
            pcap_set_timeout(pcap, timeout);
            pcap_set_snaplen(pcap, snaplen);
            cleanup_pcap(pcap);
        }
    }
    
    // Test with pcap_create for unactivated handle
    if (selector & 0x10) {
        pcap_t *pcap = pcap_create(source, errbuf);
        if (pcap) {
            // Test functions that should work on unactivated handle
            pcap_set_buffer_size(pcap, buffer_size);
            pcap_set_timeout(pcap, timeout);
            pcap_set_snaplen(pcap, snaplen);
            
            // Try to activate
            if (pcap_activate(pcap) == 0) {
                // Test functions that might work on activated handle
                pcap_setsampling(pcap);
                cleanup_pcap(pcap);
            } else {
                pcap_close(pcap);
            }
        }
    }
    
    // Test with invalid parameters
    if (selector & 0x20) {
        pcap_t *pcap = pcap_open_live(NULL, -1, -1, -1, errbuf);
        if (pcap) {
            pcap_setsampling(pcap);
            pcap_set_buffer_size(pcap, -1);
            pcap_set_timeout(pcap, -1);
            pcap_set_snaplen(pcap, -1);
            cleanup_pcap(pcap);
        }
    }
    
    return 0;
}