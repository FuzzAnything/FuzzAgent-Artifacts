// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 40) return 0;
    
    // Create dummy file for potential file-based captures
    create_dummy_file(Data, Size);
    
    // Extract parameters from fuzz data safely
    int timeout, buffer_size, snaplen, flags, read_timeout, promisc;
    
    // Copy data to local variables to avoid direct pointer arithmetic
    memcpy(&timeout, Data, sizeof(int));
    memcpy(&buffer_size, Data + 4, sizeof(int));
    memcpy(&snaplen, Data + 8, sizeof(int));
    memcpy(&flags, Data + 12, sizeof(int));
    memcpy(&read_timeout, Data + 16, sizeof(int));
    memcpy(&promisc, Data + 20, sizeof(int));
    
    // Apply bounds
    timeout = timeout % 10000;
    buffer_size = buffer_size % 65536;
    snaplen = snaplen % 65536;
    flags = flags % 256;
    read_timeout = read_timeout % 10000;
    promisc = promisc % 2;
    
    // Create device name from fuzz data safely
    char device[256];
    size_t device_len = Size > 256 ? 256 : Size;
    if (device_len > 0) {
        memcpy(device, Data, device_len);
        device[device_len - 1] = '\0';
    } else {
        device[0] = '\0';
    }
    
    // Create source string - try both device and file
    char source[512];
    if (Size > 100) {
        snprintf(source, sizeof(source), "file:./dummy_file");
    } else {
        snprintf(source, sizeof(source), "%s", device);
    }
    
    // Create error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Create auth structure (simplified)
    struct pcap_rmtauth auth;
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // First pcap_statustostr call
    int status1;
    memcpy(&status1, Data, sizeof(int));
    const char *str1 = pcap_statustostr(status1);
    (void)str1;
    
    // Create a pcap handle for configuration
    pcap_t *pcap = pcap_create(device, errbuf);
    if (pcap) {
        // pcap_set_timeout
        pcap_set_timeout(pcap, timeout);
        
        // Second pcap_statustostr call
        int status2;
        memcpy(&status2, Data + 24, sizeof(int));
        const char *str2 = pcap_statustostr(status2);
        (void)str2;
        
        // pcap_set_buffer_size
        pcap_set_buffer_size(pcap, buffer_size);
        
        // Third pcap_statustostr call
        int status3;
        memcpy(&status3, Data + 28, sizeof(int));
        const char *str3 = pcap_statustostr(status3);
        (void)str3;
        
        // pcap_activate
        int activate_result = pcap_activate(pcap);
        
        // Fourth pcap_statustostr call
        int status4;
        memcpy(&status4, Data + 32, sizeof(int));
        const char *str4 = pcap_statustostr(status4);
        (void)str4;
        
        // First pcap_geterr call
        char *err1 = pcap_geterr(pcap);
        (void)err1;
        
        // Fifth pcap_statustostr call
        int status5;
        memcpy(&status5, Data + 36, sizeof(int));
        const char *str5 = pcap_statustostr(status5);
        (void)str5;
        
        // Second pcap_geterr call
        char *err2 = pcap_geterr(pcap);
        (void)err2;
        
        // Close the first handle
        pcap_close(pcap);
    }
    
    // pcap_open
    pcap_t *pcap2 = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    
    // pcap_open_live
    pcap_t *pcap3 = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    
    // pcap_close
    if (pcap3) {
        pcap_close(pcap3);
    }
    if (pcap2) {
        pcap_close(pcap2);
    }
    
    // Cleanup dummy file
    cleanup_dummy_file();
    
    return 0;
}