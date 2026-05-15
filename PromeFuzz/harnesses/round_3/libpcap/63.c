// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
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

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static pcap_t *create_dummy_handle(void) {
    // Create a dummy handle using pcap_open_dead since it doesn't require actual devices
    return pcap_open_dead(DLT_EN10MB, 65535);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    
    // Use first byte to determine which API to test
    if (Size == 0) return 0;
    uint8_t api_selector = Data[0] % 7; // 6 APIs + 1 for combined test
    
    // Extract parameters from fuzzer input
    int int_param = 0;
    pcap_direction_t direction = PCAP_D_IN;
    
    if (Size > 1) {
        int_param = (int)Data[1];
        if (Size > 2) {
            uint8_t dir_selector = Data[2] % 3;
            if (dir_selector == 0) direction = PCAP_D_IN;
            else if (dir_selector == 1) direction = PCAP_D_OUT;
            else direction = PCAP_D_INOUT;
        }
    }
    
    // Test pcap_open_live
    if (api_selector == 0) {
        // Use "./dummy_file" as device name for fuzzing
        const char *device = "./dummy_file";
        
        // Create dummy file with fuzzer data
        int fd = open(device, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            if (Size > 0) {
                write(fd, Data, Size > 1024 ? 1024 : Size);
            }
            close(fd);
        }
        
        handle = pcap_open_live(device, int_param, 1, 1000, errbuf);
        cleanup_pcap(handle);
        
        // Remove dummy file
        unlink(device);
    }
    // Test pcap_setdirection
    else if (api_selector == 1) {
        handle = create_dummy_handle();
        if (handle) {
            pcap_setdirection(handle, direction);
            cleanup_pcap(handle);
        }
    }
    // Test pcap_set_rfmon
    else if (api_selector == 2) {
        handle = pcap_create("any", errbuf);
        if (handle) {
            pcap_set_rfmon(handle, int_param & 1);
            // Try to activate to test error paths
            pcap_activate(handle);
            cleanup_pcap(handle);
        }
    }
    // Test pcap_set_snaplen
    else if (api_selector == 3) {
        handle = pcap_create("any", errbuf);
        if (handle) {
            pcap_set_snaplen(handle, int_param);
            // Try to activate
            pcap_activate(handle);
            cleanup_pcap(handle);
        }
    }
    // Test pcap_set_promisc
    else if (api_selector == 4) {
        handle = pcap_create("any", errbuf);
        if (handle) {
            pcap_set_promisc(handle, int_param & 1);
            // Try to activate
            pcap_activate(handle);
            cleanup_pcap(handle);
        }
    }
    // Test pcap_set_buffer_size
    else if (api_selector == 5) {
        handle = pcap_create("any", errbuf);
        if (handle) {
            pcap_set_buffer_size(handle, int_param);
            // Try to activate
            pcap_activate(handle);
            cleanup_pcap(handle);
        }
    }
    // Combined test: create handle and apply multiple settings
    else if (api_selector == 6) {
        handle = pcap_create("any", errbuf);
        if (handle) {
            // Apply various settings in different orders
            pcap_set_snaplen(handle, int_param);
            pcap_set_promisc(handle, (int_param >> 1) & 1);
            pcap_set_buffer_size(handle, (int_param >> 2) & 0xFF);
            
            // Try to activate
            if (pcap_activate(handle) == 0) {
                pcap_setdirection(handle, direction);
            }
            cleanup_pcap(handle);
        }
    }
    
    return 0;
}