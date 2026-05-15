// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void dummy_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy handler for callbacks - does nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Use first few bytes to seed parameters
    if (Size < 8) return 0;
    
    // Extract parameters from fuzzer input
    int snaplen = (Data[0] << 8) | Data[1];
    int buffer_size = (Data[2] << 8) | Data[3];
    int promisc = Data[4] & 1;
    int timeout_ms = (Data[5] << 8) | Data[6];
    int rfmon = Data[7] & 1;
    int direction_val = Data[7] & 3; // 0-2 for direction
    
    // Map direction value to enum
    pcap_direction_t direction;
    switch (direction_val) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
        default: direction = PCAP_D_INOUT; break;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Test 1: pcap_open_live with dummy device
    const char *device = "any"; // Common dummy device name
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout_ms, errbuf);
    
    if (pcap != NULL) {
        // Test pcap_setdirection on live handle
        pcap_setdirection(pcap, direction);
        
        // Try to set buffer size (might fail if already active)
        pcap_set_buffer_size(pcap, buffer_size);
        
        // Try to set snaplen (might fail if already active)
        pcap_set_snaplen(pcap, snaplen);
        
        pcap_close(pcap);
    }
    
    // Test 2: Create inactive handle and test pre-activation functions
    pcap = pcap_create(device, errbuf);
    if (pcap != NULL) {
        // Test pcap_set_rfmon on inactive handle
        pcap_set_rfmon(pcap, rfmon);
        
        // Test pcap_set_snaplen on inactive handle
        pcap_set_snaplen(pcap, snaplen);
        
        // Test pcap_set_buffer_size on inactive handle
        pcap_set_buffer_size(pcap, buffer_size);
        
        // Test pcap_activate
        int activate_ret = pcap_activate(pcap);
        
        if (activate_ret == 0) {
            // Test pcap_setdirection after activation
            pcap_setdirection(pcap, direction);
            
            // Try to set buffer size again (should fail)
            pcap_set_buffer_size(pcap, buffer_size);
            
            // Try to set snaplen again (should fail)
            pcap_set_snaplen(pcap, snaplen);
            
            // Try to set rfmon again (should fail)
            pcap_set_rfmon(pcap, rfmon);
            
            // Try a simple capture loop with dummy handler
            if (Size > 8) {
                pcap_loop(pcap, 1, dummy_handler, (u_char *)Data);
            }
        }
        
        pcap_close(pcap);
    }
    
    // Test 3: Create dead handle for offline processing
    pcap = pcap_open_dead(DLT_EN10MB, snaplen);
    if (pcap != NULL) {
        // Try various operations on dead handle
        pcap_setdirection(pcap, direction);
        pcap_set_buffer_size(pcap, buffer_size);
        pcap_set_snaplen(pcap, snaplen);
        pcap_close(pcap);
    }
    
    // Test 4: Create offline handle from dummy file
    if (Size > 20) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp != NULL) {
            // Write a minimal pcap file header (24 bytes) plus some packet data
            struct pcap_file_header hdr;
            hdr.magic = 0xa1b2c3d4;
            hdr.version_major = 2;
            hdr.version_minor = 4;
            hdr.thiszone = 0;
            hdr.sigfigs = 0;
            hdr.snaplen = snaplen;
            hdr.linktype = DLT_EN10MB;
            
            fwrite(&hdr, sizeof(hdr), 1, fp);
            fwrite(Data, Size > 1024 ? 1024 : Size, 1, fp);
            fclose(fp);
            
            pcap = pcap_open_offline("./dummy_file", errbuf);
            if (pcap != NULL) {
                pcap_setdirection(pcap, direction);
                pcap_loop(pcap, 1, dummy_handler, (u_char *)Data);
                pcap_close(pcap);
            }
            
            remove("./dummy_file");
        }
    }
    
    return 0;
}