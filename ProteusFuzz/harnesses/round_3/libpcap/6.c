// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
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
#include <pcap.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *datalinks = NULL;
    int dlt_count;
    int i;
    const char *name;
    const char *status_str;
    char *err_msg;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Step 1: Prepare environment by attempting to open a dummy file
    // We use "./dummy_file" as requested.
    handle = pcap_open_offline("./dummy_file", errbuf);

    // Invoke pcap_geterr (First call in sequence)
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Invoke pcap_list_datalinks
    // Only call if handle is valid to avoid segmentation faults on NULL handle
    if (handle != NULL) {
        dlt_count = pcap_list_datalinks(handle, &datalinks);
    } else {
        dlt_count = -1;
    }

    // Invoke pcap_statustostr
    status_str = pcap_statustostr(dlt_count);
    (void)status_str;

    // Invoke pcap_geterr (Second call in sequence)
    if (handle != NULL) {
        err_msg = pcap_geterr(handle);
        (void)err_msg;
    }

    // Invoke pcap_datalink_val_to_name
    int dlt_val = 0;
    if (Size > 0) {
        dlt_val = Data[0];
    } else if (dlt_count > 0) {
        dlt_val = dlt_count;
    }
    
    name = pcap_datalink_val_to_name(dlt_val);
    (void)name;

    // Explore more states by iterating through the datalinks list if available
    if (datalinks != NULL && dlt_count > 0) {
        for (i = 0; i < dlt_count; i++) {
            name = pcap_datalink_val_to_name(datalinks[i]);
            (void)name;
        }
    }

    // Invoke pcap_free_datalinks
    if (datalinks != NULL) {
        pcap_free_datalinks(datalinks);
        datalinks = NULL;
    }

    // Cleanup: Close the pcap handle if it was opened
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}