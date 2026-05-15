// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp, maskp;
    int timeout, snaplen;
    unsigned int opts;
    
    // Use first 4 bytes for integer parameters
    timeout = *(const int*)Data;
    snaplen = *(const int*)Data;
    opts = *(const unsigned int*)Data;
    
    // Prepare source string (device/file name)
    size_t str_len = Size > 256 ? 256 : Size;
    char *source = malloc(str_len + 1);
    if (!source) return 0;
    
    memcpy(source, Data, str_len);
    source[str_len] = '\0';
    
    // 1. Test pcap_init
    if (pcap_init(opts, errbuf) != 0) {
        // Handle error silently
    }
    
    // 2. Test pcap_lookupnet
    if (pcap_lookupnet(source, &netp, &maskp, errbuf) != 0) {
        // Handle error silently
    }
    
    // 3. Test pcap_open_live
    pcap_t *pcap_live = pcap_open_live(source, snaplen, 1, timeout, errbuf);
    if (pcap_live) {
        // 4. Test pcap_set_timeout on live handle
        if (pcap_set_timeout(pcap_live, timeout) != 0) {
            // Handle error silently
        }
        
        // 5. Test pcap_set_snaplen on live handle
        if (pcap_set_snaplen(pcap_live, snaplen) != 0) {
            // Handle error silently
        }
        
        pcap_close(pcap_live);
    }
    
    // 6. Test pcap_open with file source
    write_dummy_file(Data, Size);
    
    // Create pcap_rmtauth structure if we have enough data
    struct pcap_rmtauth auth;
    if (Size >= sizeof(auth)) {
        memcpy(&auth, Data, sizeof(auth));
    } else {
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
    }
    
    // Try pcap_open with file source
    char file_source[256];
    snprintf(file_source, sizeof(file_source), "file:./dummy_file");
    pcap_t *pcap_file = pcap_open(file_source, snaplen, 0, timeout, &auth, errbuf);
    if (pcap_file) {
        pcap_close(pcap_file);
    }
    
    // Try pcap_open with "any" device
    pcap_t *pcap_any = pcap_open("any", snaplen, 0, timeout, NULL, errbuf);
    if (pcap_any) {
        pcap_close(pcap_any);
    }
    
    // Try pcap_open with original source string
    pcap_t *pcap_src = pcap_open(source, snaplen, 0, timeout, NULL, errbuf);
    if (pcap_src) {
        pcap_close(pcap_src);
    }
    
    cleanup_dummy_file();
    free(source);
    
    return 0;
}