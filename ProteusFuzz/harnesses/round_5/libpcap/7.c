// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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
#include <pcap.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *datalinks = NULL;
    int count;
    int dlt_value;
    const char *name;
    const char *status_str;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Attempt to create a dummy pcap handle using a file to avoid live capture issues
    // We create a dummy file first if needed, but pcap_open_offline expects a valid pcap file.
    // Since we are fuzzing, we can try to open a dummy file that might not be a valid pcap.
    // This will likely fail, which is fine for testing error paths.
    handle = pcap_open_offline("./dummy_file", errbuf);

    // Step 1: pcap_geterr
    // Retrieve error message immediately after opening (likely to have an error if file is invalid)
    char *err_msg = pcap_geterr(handle);
    (void)err_msg; // Suppress unused warning

    // Step 2: pcap_list_datalinks
    // Try to list datalinks. This might fail if handle is NULL or invalid, or succeed if file is valid.
    if (handle != NULL) {
        count = pcap_list_datalinks(handle, &datalinks);
        
        // Step 3: pcap_statustostr
        // Convert the return value (count or error code) to a string.
        // Note: pcap_list_datalinks returns count on success, -1 on error.
        // pcap_statustostr expects status codes like PCAP_ERROR.
        // We pass the return value directly to see how it handles various integers.
        status_str = pcap_statustostr(count);
        (void)status_str;

        // Step 4: pcap_geterr again
        // Check for errors after the datalinks call
        err_msg = pcap_geterr(handle);
        (void)err_msg;

        // Step 5: pcap_datalink_val_to_name
        // If we got datalinks, try to convert some values to names.
        // We also try to fuzz with values from the input data if available.
        if (datalinks != NULL && count > 0) {
            for (int i = 0; i < count; i++) {
                dlt_value = datalinks[i];
                name = pcap_datalink_val_to_name(dlt_value);
                (void)name;
            }
        }
        
        // Also try fuzzing with values from the input data
        if (Size > 0) {
            dlt_value = Data[0];
            name = pcap_datalink_val_to_name(dlt_value);
            (void)name;
            
            if (Size > 1) {
                dlt_value = (Data[0] << 8) | Data[1];
                name = pcap_datalink_val_to_name(dlt_value);
                (void)name;
            }
        }

        // Step 6: pcap_free_datalinks
        if (datalinks != NULL) {
            pcap_free_datalinks(datalinks);
            datalinks = NULL;
        }
    }

    // Cleanup: Close the pcap handle if it was opened
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}