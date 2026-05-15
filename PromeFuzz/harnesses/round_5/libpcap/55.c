// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
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

static int get_dlt_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return DLT_EN10MB;
    return (int)(*data) % 256;
}

static const char* get_dlt_name_from_data(const uint8_t *data, size_t size) {
    static const char* known_names[] = {
        "EN10MB", "NULL", "RAW", "IEEE802_11", "PPP", "LINUX_SLL"
    };
    if (size == 0) return known_names[0];
    return known_names[(*data) % (sizeof(known_names)/sizeof(known_names[0]))];
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    pcap_t *dead_handle = NULL;
    int *dlt_list = NULL;
    int dlt_count;
    
    // Create dummy file for potential pcap_open_offline
    write_dummy_file(Data, Size);
    
    // Try to open dummy file as pcap file
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If that fails, create a dead capture handle
    if (pcap_handle == NULL) {
        dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
        pcap_handle = dead_handle;
    }
    
    // Also try to create a live capture handle (may fail without device)
    pcap_t *live_handle = pcap_create("lo", errbuf);
    if (live_handle != NULL) {
        // Try to activate it
        if (pcap_activate(live_handle) == 0) {
            // Test pcap_datalink on activated handle
            (void)pcap_datalink(live_handle);
            
            // Test pcap_datalink_ext on activated handle
            (void)pcap_datalink_ext(live_handle);
            
            // Test pcap_list_datalinks on activated handle
            dlt_count = pcap_list_datalinks(live_handle, &dlt_list);
            if (dlt_count > 0 && dlt_list != NULL) {
                free(dlt_list);
                dlt_list = NULL;
            }
        }
        
        // Test pcap_set_datalink on non-activated handle
        // First create another handle for this test
        pcap_t *set_handle = pcap_create("lo", errbuf);
        if (set_handle != NULL) {
            int test_dlt = get_dlt_from_data(Data, Size);
            (void)pcap_set_datalink(set_handle, test_dlt);
            pcap_close(set_handle);
        }
        
        pcap_close(live_handle);
    }
    
    // Test functions that don't require a handle
    int test_dlt = get_dlt_from_data(Data, Size);
    const char *name = pcap_datalink_val_to_name(test_dlt);
    
    const char *test_name = get_dlt_name_from_data(Data, Size);
    int dlt_val = pcap_datalink_name_to_val(test_name);
    
    // Test functions with the pcap handle we have
    if (pcap_handle != NULL) {
        // Test pcap_datalink if handle is activated
        if (pcap_handle != dead_handle || pcap_is_swapped(pcap_handle) >= 0) {
            (void)pcap_datalink(pcap_handle);
            (void)pcap_datalink_ext(pcap_handle);
            
            dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
            if (dlt_count > 0 && dlt_list != NULL) {
                free(dlt_list);
                dlt_list = NULL;
            }
        }
        
        // Try pcap_set_datalink on the handle (may fail if activated)
        (void)pcap_set_datalink(pcap_handle, test_dlt);
        
        pcap_close(pcap_handle);
    }
    
    // Clean up
    if (dead_handle != NULL && dead_handle != pcap_handle) {
        pcap_close(dead_handle);
    }
    
    unlink("./dummy_file");
    
    return 0;
}