// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
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

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Use the fuzzer input to construct a device name or use NULL
    // We limit the device name length to avoid buffer overflows
    char device_name[256];
    size_t dev_len = (Size < 255) ? Size : 255;
    memcpy(device_name, Data, dev_len);
    device_name[dev_len] = '\0';

    // Try with the generated device name or NULL
    const char *dev = (dev_len > 0 && device_name[0] != '\0') ? device_name : NULL;

    pcap_t *handle = pcap_create(dev, errbuf);

    int result_can_set_rfmon = 0;
    if (handle != NULL) {
        // Invoke pcap_can_set_rfmon
        result_can_set_rfmon = pcap_can_set_rfmon(handle);

        // Cleanup the handle
        cleanup_pcap(handle);
    }

    // Invoke pcap_geterr on a NULL handle (should be safe, returns empty or generic error)
    // Note: pcap_geterr expects a valid pcap_t*, passing NULL might cause issues depending on implementation.
    // However, the task asks to invoke it. Let's try with a valid handle first if we had one, 
    // but since we cleaned up, we will just skip or use a dummy if we could keep it.
    // To strictly follow "invoke", let's create a handle again just for geterr if needed, 
    // or simply acknowledge that after cleanup we can't call it safely on the same handle.
    // Re-creating for the sake of calling pcap_geterr if the first one failed or to show usage.
    
    // Let's re-create to demonstrate pcap_geterr usage after a potential failure or success
    memset(errbuf, 0, sizeof(errbuf));
    pcap_t *handle2 = pcap_create(dev, errbuf);
    if (handle2 != NULL) {
        // Even if successful, pcap_geterr returns the error buffer content (likely empty or last error)
        char *err_str = pcap_geterr(handle2);
        (void)err_str; // Use the return value to avoid unused warning
        cleanup_pcap(handle2);
    } else {
        // If pcap_create failed, we can't call pcap_geterr on a NULL handle safely in all versions
        // So we skip pcap_geterr in the failure path of create for safety, 
        // or assume the user handles NULL checks. 
        // The spec says "Use it after a pcap function fails". 
        // If pcap_create fails, handle is NULL, so we cannot call pcap_geterr(handle).
        // This is a limitation of the API flow requested. 
    }

    // Invoke pcap_statustostr with various status codes derived from input
    // We can try PCAP_ERROR, PCAP_WARNING, PCAP_ERROR_BREAK, etc.
    // Let's use the first byte of input to select a status code
    int status_code = Data[0];
    
    // Map some common codes or just pass the raw value to test robustness
    // Common codes: PCAP_ERROR (-1), PCAP_WARNING (1), PCAP_ERROR_BREAK (-2)
    // We will just pass the raw value to test the function's handling of arbitrary ints
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str; // Use the return value

    // Also test with specific known constants if possible, but raw input is good for fuzzing
    // Let's try a few specific ones to ensure coverage of known paths
    (void)pcap_statustostr(PCAP_ERROR);
    (void)pcap_statustostr(PCAP_WARNING);
    (void)pcap_statustostr(PCAP_ERROR_BREAK);
    (void)pcap_statustostr(0); // Unknown or no error

    return 0;
}