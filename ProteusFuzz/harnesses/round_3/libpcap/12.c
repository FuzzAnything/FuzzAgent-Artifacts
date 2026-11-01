// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const char *status_str;
    char *err_msg;
    int ret;

    if (Size < 2) {
        return 0;
    }

    // Initialize handle
    // We try "any" first, then "lo" if "any" fails.
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        handle = pcap_create("lo", errbuf);
    }
    
    if (handle == NULL) {
        // If we cannot create a handle, we cannot proceed with the API sequence.
        return 0;
    }

    // 1. pcap_set_immediate_mode
    // Use data from fuzz input to determine mode (0 or 1)
    int immediate_mode = (Data[0] % 2);
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    // We don't necessarily need to check ret here for the sequence, but we can log it if needed.

    // 2. pcap_statustostr
    // Use data to generate a status code.
    int status_code = (int)(Data[1]); 
    // Ensure we test some known values too
    if (Data[1] % 5 == 0) status_code = PCAP_ERROR;
    else if (Data[1] % 5 == 1) status_code = PCAP_WARNING;
    else if (Data[1] % 5 == 2) status_code = 0;
    else if (Data[1] % 5 == 3) status_code = -100;
    
    status_str = pcap_statustostr(status_code);
    (void)status_str; 

    // 3. pcap_set_timeout
    // Set timeout based on fuzz data. Must be called before activate.
    int timeout_val = (int)(Data[0] * 10 + Data[1]); 
    ret = pcap_set_timeout(handle, timeout_val);
    
    // 4. pcap_statustostr (again)
    // Test with another value
    status_code = (int)(Data[1] * -1);
    status_str = pcap_statustostr(status_code);
    (void)status_str;

    // 5. pcap_activate
    // This finalizes the setup.
    ret = pcap_activate(handle);
    // If activate fails, the handle might be in an error state, but we continue to test other functions.

    // 6. pcap_statustostr (again)
    // Test with a value derived from the activation result
    status_code = ret; 
    status_str = pcap_statustostr(status_code);
    (void)status_str;

    // 7. pcap_geterr
    // Retrieve the error message string.
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}