// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int *dlt_list = NULL;
    int dlt_count = 0;
    
    // Write fuzzing data to dummy file
    write_dummy_file(Data, Size);
    
    // Create a pcap handle from dummy file
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    if (pcap_handle == NULL) {
        // If opening fails, create a dead pcap handle for testing
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap_handle == NULL) {
            // Clean up and return if we can't create any handle
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // First call to pcap_geterr (may return empty string or previous error)
    char *err_msg1 = pcap_geterr(pcap_handle);
    (void)err_msg1; // Use the result to avoid unused variable warning
    
    // Call pcap_list_datalinks - requires activated handle
    dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
    
    // Call pcap_statustostr with various status codes derived from fuzzing input
    int status_code = 0;
    if (Size > 0) {
        // Use first byte as status code, but mask to get valid range
        status_code = (int)Data[0];
        // Try both positive and negative status codes
        const char *status_str1 = pcap_statustostr(status_code);
        const char *status_str2 = pcap_statustostr(-status_code);
        const char *status_str3 = pcap_statustostr(PCAP_ERROR);
        const char *status_str4 = pcap_statustostr(PCAP_WARNING);
        (void)status_str1;
        (void)status_str2;
        (void)status_str3;
        (void)status_str4;
    }
    
    // Second call to pcap_geterr
    char *err_msg2 = pcap_geterr(pcap_handle);
    (void)err_msg2;
    
    // Call pcap_datalink_val_to_name with various DLT values
    if (Size > 1) {
        // Use second byte as DLT value
        int dlt_val = (int)Data[1];
        const char *dlt_name1 = pcap_datalink_val_to_name(dlt_val);
        const char *dlt_name2 = pcap_datalink_val_to_name(DLT_EN10MB);
        const char *dlt_name3 = pcap_datalink_val_to_name(DLT_NULL);
        const char *dlt_name4 = pcap_datalink_val_to_name(DLT_RAW);
        (void)dlt_name1;
        (void)dlt_name2;
        (void)dlt_name3;
        (void)dlt_name4;
    }
    
    // If dlt_list was allocated, free it
    if (dlt_list != NULL) {
        pcap_free_datalinks(dlt_list);
    }
    
    // Clean up
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}