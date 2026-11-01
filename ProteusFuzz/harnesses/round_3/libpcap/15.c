// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Use the fuzzer input as the device name, ensuring it is null-terminated
    // If Size is large, truncate to avoid overflow, but keep it valid for string operations
    size_t device_len = Size;
    if (device_len > 255) {
        device_len = 255;
    }
    char device_name[256];
    memcpy(device_name, Data, device_len);
    device_name[device_len] = '\0';

    // Step 1: pcap_create
    pcap_t *handle = pcap_create(device_name, errbuf);

    // Step 2: pcap_can_set_rfmon
    // Only call if handle is valid
    if (handle != NULL) {
        int can_rfmon = pcap_can_set_rfmon(handle);
        (void)can_rfmon; // Use the return value to avoid unused warnings
    }

    // Step 3: pcap_geterr
    // Call regardless of handle validity to test error retrieval logic
    if (handle != NULL) {
        const char *err_msg = pcap_geterr(handle);
        (void)err_msg;
    } else {
        // If handle is NULL, we can't call pcap_geterr on it, but we can test the error buffer
        // The error message is already in errbuf from pcap_create
        (void)errbuf;
    }

    // Step 4: pcap_statustostr
    // Test with various status codes derived from input or constants
    // Use input data to generate a status code, clamping to reasonable range
    int status_code = 0;
    if (Size >= 4) {
        status_code = *(const int32_t *)Data;
    } else if (Size > 0) {
        status_code = Data[0];
    }

    // Ensure status_code is within a safe range for testing, though the function handles any int
    // We test a few specific known codes and the fuzzed one
    const char *msg1 = pcap_statustostr(status_code);
    (void)msg1;

    const char *msg2 = pcap_statustostr(PCAP_ERROR);
    (void)msg2;

    const char *msg3 = pcap_statustostr(PCAP_ERROR_BREAK);
    (void)msg3;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}