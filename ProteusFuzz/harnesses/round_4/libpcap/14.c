// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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
#include <stdio.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a device name string from input data
    // Ensure it is null-terminated and has a reasonable length
    size_t max_dev_len = 256;
    if (Size > max_dev_len) {
        Size = max_dev_len;
    }

    char *device_name = (char *)malloc(Size + 1);
    if (device_name == NULL) {
        return 0;
    }
    memcpy(device_name, Data, Size);
    device_name[Size] = '\0';

    // Error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // 1. Invoke pcap_create
    pcap_t *handle = pcap_create(device_name, errbuf);

    int result = 0;

    if (handle != NULL) {
        // 2. Invoke pcap_can_set_rfmon
        // Returns 1 if supported, 0 otherwise
        result = pcap_can_set_rfmon(handle);

        // 3. Invoke pcap_geterr
        // Retrieve error message (safe to call even if no error occurred)
        const char *err_msg = pcap_geterr(handle);
        (void)err_msg; // Use the value to avoid unused warning, logic handled by return

        // Cleanup: Close the handle
        pcap_close(handle);
    } else {
        // If pcap_create failed, we still want to exercise pcap_geterr on the NULL handle?
        // No, pcap_geterr requires a valid pcap_t*. So we skip it or handle gracefully.
        // However, the task says invoke in order. If handle is NULL, we cannot invoke pcap_geterr safely.
        // We can just proceed to the next function that doesn't require a handle.
    }

    // 4. Invoke pcap_statustostr
    // Try different status codes to explore states
    int status_codes[] = {PCAP_ERROR, PCAP_ERROR_BREAK, PCAP_ERROR_NO_SUCH_DEVICE, PCAP_ERROR_PERM_DENIED, 0, -1, 100};
    for (int i = 0; i < 7; i++) {
        const char *str = pcap_statustostr(status_codes[i]);
        (void)str; // Use the value
    }

    // Cleanup allocated memory
    free(device_name);

    return 0;
}