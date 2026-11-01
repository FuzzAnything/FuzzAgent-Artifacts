// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
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
#include <errno.h>

// Fuzz driver for libpcap API functions
// Target functions: pcap_is_swapped, pcap_snapshot, pcap_datalink_ext,
//                   pcap_bufsize, pcap_major_version, pcap_minor_version

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret_val = 0;

    // Strategy: Try to open a savefile with the fuzz input, then activate and test APIs.
    // If that fails, try opening a live capture on a dummy interface (may fail gracefully).
    
    // Create a dummy file from input data
    const char *dummy_file = "./dummy_file";
    create_dummy_file(dummy_file, Data, Size);

    // Attempt to open as a savefile
    handle = pcap_open_offline(dummy_file, errbuf);
    
    if (handle == NULL) {
        // If offline open fails, try to open a live capture on a common loopback interface
        // This might fail on some systems but is worth trying to exercise the code path
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            // Try to activate it
            if (pcap_activate(handle) == 0) {
                // Successfully activated live capture
                // Proceed to test APIs
            } else {
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    if (handle != NULL) {
        // Ensure handle is activated before calling the target functions
        // If we opened offline, it's usually activated. If live, we activated above.
        // However, pcap_activate might be needed explicitly for some cases.
        if (pcap_activate(handle) != 0 && pcap_activate(handle) != 0) {
             // If activation fails twice (idempotent check), we might still have an active handle
             // or it might be inactive. Let's just proceed and let the API return error codes.
        }

        // Call target API functions
        // 1. pcap_is_swapped
        ret_val = pcap_is_swapped(handle);
        (void)ret_val; // Suppress unused variable warning

        // 2. pcap_snapshot
        ret_val = pcap_snapshot(handle);
        (void)ret_val;

        // 3. pcap_datalink_ext
        ret_val = pcap_datalink_ext(handle);
        (void)ret_val;

        // 4. pcap_bufsize
        ret_val = pcap_bufsize(handle);
        (void)ret_val;

        // 5. pcap_major_version
        ret_val = pcap_major_version(handle);
        (void)ret_val;

        // 6. pcap_minor_version
        ret_val = pcap_minor_version(handle);
        (void)ret_val;

        // Cleanup
        pcap_close(handle);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}