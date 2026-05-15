// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"

static void dummy_cleanup(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *dlt_list = NULL;
    int dlt_count = 0;
    
    // Create a dummy pcap handle
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        // If "any" device doesn't exist, try creating a dead capture handle
        handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (handle == NULL) {
            return 0;
        }
    }
    
    // Use fuzzer data to determine which functions to call and in what order
    // We'll use the first byte to decide the sequence
    if (Size > 0) {
        uint8_t sequence = Data[0] % 6;  // 6 different sequences
        
        switch (sequence) {
            case 0:
                // Sequence: create -> set_datalink -> activate -> datalink_ext -> snapshot -> bufsize
                if (Size > 1) {
                    int dlt_type = Data[1] % 256;
                    pcap_set_datalink(handle, dlt_type);
                }
                pcap_activate(handle);
                pcap_datalink_ext(handle);
                pcap_snapshot(handle);
                pcap_bufsize(handle);
                break;
                
            case 1:
                // Sequence: create -> activate -> list_datalinks -> free
                pcap_activate(handle);
                dlt_count = pcap_list_datalinks(handle, &dlt_list);
                if (dlt_count > 0 && dlt_list != NULL) {
                    pcap_free_datalinks(dlt_list);
                }
                break;
                
            case 2:
                // Sequence: create -> set_datalink (multiple times) -> activate
                if (Size > 2) {
                    for (size_t i = 1; i < Size && i < 10; i++) {
                        int dlt_type = Data[i] % 256;
                        pcap_set_datalink(handle, dlt_type);
                    }
                }
                pcap_activate(handle);
                break;
                
            case 3:
                // Sequence: create -> activate -> call all query functions
                pcap_activate(handle);
                pcap_datalink_ext(handle);
                pcap_snapshot(handle);
                pcap_bufsize(handle);
                dlt_count = pcap_list_datalinks(handle, &dlt_list);
                if (dlt_count > 0 && dlt_list != NULL) {
                    pcap_free_datalinks(dlt_list);
                }
                break;
                
            case 4:
                // Sequence: create -> set_datalink -> activate -> deactivate (close) -> create new
                if (Size > 1) {
                    int dlt_type = Data[1] % 256;
                    pcap_set_datalink(handle, dlt_type);
                }
                pcap_activate(handle);
                pcap_close(handle);
                
                // Create a new handle
                handle = pcap_create("any", errbuf);
                if (handle == NULL) {
                    handle = pcap_open_dead(DLT_EN10MB, 65535);
                }
                if (handle != NULL) {
                    pcap_activate(handle);
                    pcap_snapshot(handle);
                }
                break;
                
            case 5:
                // Sequence: create -> try all functions without activation
                if (Size > 1) {
                    int dlt_type = Data[1] % 256;
                    pcap_set_datalink(handle, dlt_type);
                }
                pcap_datalink_ext(handle);  // Should fail
                pcap_snapshot(handle);      // Should fail
                pcap_bufsize(handle);       // Should fail
                dlt_count = pcap_list_datalinks(handle, &dlt_list);  // Should fail
                pcap_activate(handle);  // Finally activate
                break;
        }
    } else {
        // Minimal test with default sequence
        pcap_activate(handle);
        pcap_datalink_ext(handle);
        pcap_snapshot(handle);
    }
    
    // Cleanup
    dummy_cleanup(handle);
    
    return 0;
}