// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int ret;
    
    if (Size < 1) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    // Test 1: Try to open the dummy file as a pcap savefile
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    if (pcap_handle != NULL) {
        // Test pcap_is_swapped on activated handle (from savefile)
        (void)pcap_is_swapped(pcap_handle);
        
        // Test pcap_bufsize on activated handle
        (void)pcap_bufsize(pcap_handle);
        
        // Test pcap_snapshot on activated handle
        (void)pcap_snapshot(pcap_handle);
        
        pcap_close(pcap_handle);
        pcap_handle = NULL;
    }
    
    // Test 2: Create a new handle for live capture testing
    // Use first byte to determine which device to try
    const char *device = (Data[0] & 1) ? "any" : "lo";
    pcap_handle = pcap_create(device, errbuf);
    
    if (pcap_handle != NULL) {
        // Use fuzzer data to determine parameters
        int snaplen = (Size > 1) ? (Data[1] % 65536) : 65535;
        int immediate_mode = (Size > 2) ? (Data[2] & 1) : 0;
        
        // Test pcap_set_snaplen before activation
        (void)pcap_set_snaplen(pcap_handle, snaplen);
        
        // Test pcap_set_immediate_mode before activation
        (void)pcap_set_immediate_mode(pcap_handle, immediate_mode);
        
        // Test pcap_activate
        ret = pcap_activate(pcap_handle);
        
        if (ret == 0 || ret == PCAP_WARNING) {
            // Handle activated successfully or with warnings
            // Test pcap_is_swapped on live capture (should return 0 or error)
            (void)pcap_is_swapped(pcap_handle);
            
            // Test pcap_bufsize on activated handle
            (void)pcap_bufsize(pcap_handle);
            
            // Test pcap_snapshot on activated handle
            (void)pcap_snapshot(pcap_handle);
        } else if (ret == PCAP_ERROR) {
            // Activation failed, try to close and create new handle
            pcap_close(pcap_handle);
            
            // Try with different parameters
            pcap_handle = pcap_create(device, errbuf);
            if (pcap_handle != NULL) {
                // Use different parameters from fuzzer data
                snaplen = (Size > 3) ? (Data[3] % 65536) : 1514;
                immediate_mode = (Size > 4) ? (Data[4] & 1) : 1;
                
                (void)pcap_set_snaplen(pcap_handle, snaplen);
                (void)pcap_set_immediate_mode(pcap_handle, immediate_mode);
                
                // Try activation again
                (void)pcap_activate(pcap_handle);
            }
        }
        
        if (pcap_handle != NULL) {
            pcap_close(pcap_handle);
        }
    }
    
    // Test 3: Create a dead handle for filter compilation
    // Use fuzzer data to determine link type and snaplen
    int linktype = (Size > 5) ? (Data[5] % 256) : DLT_EN10MB;
    int dead_snaplen = (Size > 6) ? (Data[6] % 65536) : 65535;
    
    pcap_handle = pcap_open_dead(linktype, dead_snaplen);
    if (pcap_handle != NULL) {
        // Dead handle is considered activated
        (void)pcap_is_swapped(pcap_handle);
        (void)pcap_bufsize(pcap_handle);
        (void)pcap_snapshot(pcap_handle);
        
        pcap_close(pcap_handle);
    }
    
    cleanup_dummy_file();
    
    return 0;
}