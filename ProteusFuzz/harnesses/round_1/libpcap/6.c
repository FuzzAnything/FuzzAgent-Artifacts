// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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
    int dlt_val;
    int status_code;

    // Initialize errbuf
    memset(errbuf, 0, sizeof(errbuf));

    // Create a dead handle to test APIs that require a handle
    handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle == NULL) {
        // If handle creation fails, we cannot proceed with handle-dependent APIs safely.
        // We return early to avoid undefined behavior.
        return 0;
    }

    // Step 1: pcap_geterr
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Step 2: pcap_list_datalinks
    dlt_count = pcap_list_datalinks(handle, &datalinks);

    // Step 3: pcap_statustostr
    // Use fuzz input or the count as status code
    if (Size > 0) {
        status_code = (int)Data[0];
    } else {
        status_code = dlt_count;
    }
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str;

    // Step 4: pcap_geterr again
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Step 5: pcap_datalink_val_to_name
    // Try to map a DLT value from the list or from fuzz input
    if (datalinks != NULL && dlt_count > 0) {
        dlt_val = datalinks[0];
    } else if (Size > 0) {
        dlt_val = (int)Data[0];
    } else {
        dlt_val = DLT_EN10MB;
    }
    const char *dlt_name = pcap_datalink_val_to_name(dlt_val);
    (void)dlt_name;

    // Step 6: pcap_free_datalinks
    if (datalinks != NULL) {
        pcap_free_datalinks(datalinks);
        datalinks = NULL;
    }

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}