// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for pcap_open_offline
    write_dummy_file(Data, Size);
    
    // Initialize pcap handle
    pcap_t *pcap_handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Try to open the dummy file as a pcap capture file
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If that fails, create a dead pcap handle for testing
    if (pcap_handle == NULL) {
        // Use first byte of input as DLT type (modulo to keep it reasonable)
        int dlt_type = Size > 0 ? (Data[0] % 100) : DLT_EN10MB;
        pcap_handle = pcap_open_dead(dlt_type, 65535);
    }
    
    if (pcap_handle == NULL) {
        // If we still can't get a handle, return early
        return 0;
    }
    
    // First call to pcap_geterr (may be NULL if no error yet)
    char *error_msg1 = pcap_geterr(pcap_handle);
    (void)error_msg1; // Use result to avoid unused variable warning
    
    // Call pcap_list_datalinks
    int *dlt_list = NULL;
    int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
    
    // Call pcap_statustostr with various status codes from fuzzer input
    const char *status_str = NULL;
    if (Size > 0) {
        // Use first few bytes to generate status codes
        int status_code = 0;
        if (Size >= 4) {
            status_code = *(const int *)Data;
        } else if (Size >= 2) {
            status_code = *(const uint16_t *)Data;
        } else {
            status_code = Data[0];
        }
        status_str = pcap_statustostr(status_code);
    } else {
        status_str = pcap_statustostr(PCAP_ERROR);
    }
    (void)status_str; // Use result
    
    // Second call to pcap_geterr
    char *error_msg2 = pcap_geterr(pcap_handle);
    (void)error_msg2;
    
    // Call pcap_datalink_val_to_name with values from fuzzer input
    const char *dlt_name = NULL;
    if (Size > 0) {
        // Use different bytes for DLT value
        int dlt_value = 0;
        if (Size >= 8) {
            dlt_value = *(const int *)(Data + 4);
        } else if (Size >= 2) {
            dlt_value = Data[Size - 1];
        } else {
            dlt_value = Data[0];
        }
        dlt_name = pcap_datalink_val_to_name(dlt_value);
    } else {
        dlt_name = pcap_datalink_val_to_name(DLT_EN10MB);
    }
    (void)dlt_name;
    
    // Call pcap_free_datalinks if we got a list
    if (dlt_count > 0 && dlt_list != NULL) {
        pcap_free_datalinks(dlt_list);
    }
    
    // Cleanup
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}