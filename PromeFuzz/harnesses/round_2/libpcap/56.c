// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
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

static void fuzz_pcap_open_live(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char device[256];
    
    // Extract device name from fuzz data
    size_t device_len = size < sizeof(device) - 1 ? size : sizeof(device) - 1;
    if (device_len > 0) {
        memcpy(device, data, device_len);
        device[device_len] = '\0';
    } else {
        strcpy(device, "any");
    }
    
    // Extract parameters from fuzz data
    int snaplen = 65535;
    int promisc = size > 0 ? data[0] % 2 : 0;
    int timeout = 1000;
    
    pcap_t *handle = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void fuzz_pcap_set_snaplen(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    
    // Create a dummy handle first
    handle = pcap_open_live("any", 65535, 0, 1000, errbuf);
    if (handle != NULL) {
        // Extract snaplen from fuzz data
        int snaplen = 65535;
        if (size >= sizeof(int)) {
            memcpy(&snaplen, data, sizeof(int));
            // Ensure positive value
            if (snaplen < 0) snaplen = -snaplen;
            if (snaplen == 0) snaplen = 1;
        }
        
        pcap_set_snaplen(handle, snaplen);
        pcap_close(handle);
    }
}

static void fuzz_pcap_lookupnet(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char device[256];
    bpf_u_int32 net, mask;
    
    // Extract device name from fuzz data
    size_t device_len = size < sizeof(device) - 1 ? size : sizeof(device) - 1;
    if (device_len > 0) {
        memcpy(device, data, device_len);
        device[device_len] = '\0';
    } else {
        strcpy(device, "any");
    }
    
    pcap_lookupnet(device, &net, &mask, errbuf);
}

static void fuzz_pcap_init(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    unsigned int opts = 0;
    
    // Extract options from fuzz data
    if (size >= sizeof(unsigned int)) {
        memcpy(&opts, data, sizeof(unsigned int));
    }
    
    pcap_init(opts, errbuf);
}

static void fuzz_pcap_set_timeout(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    
    // Create a dummy handle first
    handle = pcap_open_live("any", 65535, 0, 1000, errbuf);
    if (handle != NULL) {
        // Extract timeout from fuzz data
        int timeout = 1000;
        if (size >= sizeof(int)) {
            memcpy(&timeout, data, sizeof(int));
            // Ensure reasonable value
            if (timeout < 0) timeout = -timeout;
            if (timeout > 1000000) timeout = 1000000;
        }
        
        pcap_set_timeout(handle, timeout);
        pcap_close(handle);
    }
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[512];
    
    // Extract source string from fuzz data
    size_t source_len = size < sizeof(source) - 1 ? size : sizeof(source) - 1;
    if (source_len > 0) {
        memcpy(source, data, source_len);
        source[source_len] = '\0';
    } else {
        strcpy(source, "any");
    }
    
    // Extract parameters from fuzz data
    int snaplen = 65535;
    int flags = 0;
    int read_timeout = 1000;
    
    // Try to open as file if source looks like a file path
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size > 1024 ? 1024 : size);
        }
        close(fd);
        
        // Try with file source
        pcap_t *handle = pcap_open("./dummy_file", snaplen, flags, read_timeout, NULL, errbuf);
        if (handle != NULL) {
            pcap_close(handle);
        }
    }
    
    // Try with original source
    pcap_t *handle = pcap_open(source, snaplen, flags, read_timeout, NULL, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to decide which function to fuzz
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector) {
        case 0:
            fuzz_pcap_open_live(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_pcap_set_snaplen(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_pcap_lookupnet(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_pcap_init(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_pcap_set_timeout(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_pcap_open(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}