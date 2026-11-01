// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret;
    int immediate_mode;
    int timeout;
    int status_code;

    if (Size < 3) {
        return 0;
    }

    // Extract parameters from fuzz input
    immediate_mode = Data[0] % 2; // 0 or 1
    timeout = (int)Data[1];
    status_code = (int)Data[2];

    // Create a pcap handle for an inactive capture (required before set_immediate_mode, set_timeout, activate)
    // Using "any" as a generic device name.
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        // If create fails, we cannot proceed with the specific API sequence.
        // We still call pcap_statustostr to exercise that path.
        pcap_statustostr(status_code);
        return 0;
    }

    // 1. pcap_set_immediate_mode
    ret = pcap_set_immediate_mode(handle, immediate_mode);
    (void)ret;

    // 2. pcap_statustostr
    pcap_statustostr(status_code);

    // 3. pcap_set_timeout
    ret = pcap_set_timeout(handle, timeout);
    (void)ret;

    // 4. pcap_statustostr
    pcap_statustostr(status_code);

    // 5. pcap_activate
    ret = pcap_activate(handle);
    (void)ret;

    // 6. pcap_statustostr
    pcap_statustostr(status_code);

    // 7. pcap_geterr
    // This retrieves the error message from the handle's internal buffer.
    const char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}