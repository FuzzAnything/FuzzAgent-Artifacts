// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *status_str;
    int ret;

    // Prepare environment: Create a pcap handle
    // We try to create from a file or live device. 
    // Using "lo" (loopback) is common for testing, but if it fails, we try the dummy file.
    handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        // Try creating from the dummy file if live creation fails
        write_dummy_file(Data, Size);
        handle = pcap_create("./dummy_file", errbuf);
    }

    if (handle == NULL) {
        // If we cannot create a handle, we can still test pcap_statustostr with error codes
        status_str = pcap_statustostr(PCAP_ERROR);
        (void)status_str; // Avoid unused warning
        status_str = pcap_statustostr(PCAP_ERROR_ACTIVATED);
        (void)status_str;
        return 0;
    }

    // 1. pcap_set_immediate_mode
    // Try both 0 and 1 based on input data to explore states
    int immediate_mode = (Size > 0 && Data[0] % 2 == 0) ? 1 : 0;
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    
    // 2. pcap_statustostr
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 3. pcap_set_timeout
    // Use input data to determine timeout, clamping to reasonable values
    int timeout = 0;
    if (Size > 1) {
        timeout = (Data[1] % 1000); // 0 to 999 ms
    }
    ret = pcap_set_timeout(handle, timeout);

    // 4. pcap_statustostr
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 5. pcap_activate
    ret = pcap_activate(handle);

    // 6. pcap_statustostr
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 7. pcap_geterr
    // Retrieve error message regardless of activation success to test the API
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}