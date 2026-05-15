// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
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

static void dummy_pcap_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy handler for pcap_loop - do nothing
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write minimal valid pcap file header if size is 0
            static const uint8_t minimal_pcap[] = {
                0xd4, 0xc3, 0xb2, 0xa1,  // magic number
                0x02, 0x00,              // major version
                0x04, 0x00,              // minor version
                0x00, 0x00, 0x00, 0x00,  // timezone
                0x00, 0x00, 0x00, 0x00,  // sigfigs
                0x00, 0x00, 0x00, 0x00,  // snaplen
                0x00, 0x00, 0x00, 0x00   // linktype
            };
            write(fd, minimal_pcap, sizeof(minimal_pcap));
        }
        close(fd);
    }

    // Create null-terminated string from fuzzer input for name functions
    char *name_buf = NULL;
    if (Size > 0) {
        name_buf = (char *)malloc(Size + 1);
        if (name_buf) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
        }
    }

    // Test pcap_tstamp_type_name_to_val
    if (name_buf) {
        int tstamp_val = pcap_tstamp_type_name_to_val(name_buf);
        (void)tstamp_val; // Use result to avoid unused variable warning
    }

    // Test pcap_tstamp_type_val_to_name and pcap_tstamp_type_val_to_description
    if (Size >= sizeof(int)) {
        int val;
        memcpy(&val, Data, sizeof(int));
        
        const char *name = pcap_tstamp_type_val_to_name(val);
        (void)name;
        
        const char *desc = pcap_tstamp_type_val_to_description(val);
        (void)desc;
    }

    // Create pcap handle for functions that require it
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = NULL;
    
    // Try different ways to create pcap handle
    if (Size > 0) {
        // Try pcap_open_dead
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    } else {
        // Try pcap_create with dummy device
        pcap_handle = pcap_create("lo", errbuf);
        if (pcap_handle) {
            pcap_activate(pcap_handle);
        }
    }
    
    // Also try opening dummy file
    pcap_t *file_handle = pcap_open_offline("./dummy_file", errbuf);

    // Test pcap_list_tstamp_types and pcap_free_tstamp_types
    if (pcap_handle) {
        int *tstamp_types = NULL;
        int count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
        if (count > 0 && tstamp_types) {
            pcap_free_tstamp_types(tstamp_types);
        }
    }
    
    if (file_handle) {
        int *tstamp_types = NULL;
        int count = pcap_list_tstamp_types(file_handle, &tstamp_types);
        if (count > 0 && tstamp_types) {
            pcap_free_tstamp_types(tstamp_types);
        }
    }

    // Test pcap_set_tstamp_type
    if (pcap_handle) {
        if (Size >= sizeof(int)) {
            int val;
            memcpy(&val, Data, sizeof(int));
            pcap_set_tstamp_type(pcap_handle, val);
        }
        
        // Try some known timestamp type values
        pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_HOST);
        pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_HOST_LOWPREC);
        pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_HOST_HIPREC);
        pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_ADAPTER);
        pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_ADAPTER_UNSYNCED);
    }

    // Clean up
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    if (file_handle) {
        pcap_close(file_handle);
    }
    free(name_buf);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}