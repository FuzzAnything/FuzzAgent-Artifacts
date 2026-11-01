// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;

    // Attempt to create a handle using pcap_create with a dummy device
    // Using "any" or a loopback device is common, but pcap_create is safer for fuzzing
    // as it doesn't immediately activate.
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        // If creation fails, try pcap_open_live on a loopback device as a fallback
        // This might fail on some systems but is a valid path to test.
        handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
    }

    if (handle == NULL) {
        // If we cannot create a handle, we cannot test the target functions effectively.
        // Return early to avoid crashing.
        return 0;
    }

    // Try to activate the handle to make it valid for the target functions.
    // We use a small buffer and timeout to avoid hanging.
    int ret = pcap_activate(handle);
    if (ret != 0) {
        // If activation fails, the handle is inactive.
        // The target functions should return PCAP_ERROR_NOT_ACTIVATED.
        // We still call them to test the error path.
        (void)pcap_snapshot(handle);
        (void)pcap_minor_version(handle);
        (void)pcap_bufsize(handle);
        (void)pcap_major_version(handle);
        (void)pcap_datalink_ext(handle);
        
        // pcap_inject on inactive handle should also fail gracefully
        if (Size > 0) {
            (void)pcap_inject(handle, Data, Size);
        }
        
        cleanup_pcap(handle);
        return 0;
    }

    // Handle is active. Test the target functions.
    
    // 1. pcap_snapshot
    (void)pcap_snapshot(handle);

    // 2. pcap_minor_version
    (void)pcap_minor_version(handle);

    // 3. pcap_bufsize
    (void)pcap_bufsize(handle);

    // 4. pcap_inject
    // Ensure Size is valid for injection (must be > 0 and <= INT_MAX)
    if (Size > 0 && Size <= 2147483647) {
        (void)pcap_inject(handle, Data, Size);
    }

    // 5. pcap_major_version
    (void)pcap_major_version(handle);

    // 6. pcap_datalink_ext
    (void)pcap_datalink_ext(handle);

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}