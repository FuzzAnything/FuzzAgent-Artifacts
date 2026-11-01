// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
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

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for savefile operations if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    // Try to open a savefile using the dummy file
    handle = pcap_open_offline(dummy_file, errbuf);

    // If opening the savefile fails, try opening a live capture on "lo" or "eth0"
    // Note: Live capture might fail in many environments, but we try to exercise the code path
    if (handle == NULL) {
        // Attempt to open a live interface; this often fails in fuzzing environments but is valid code
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            // Try to activate it
            ret = pcap_activate(handle);
            // Regardless of activation success, we can test the functions
            // If activation fails, the target functions should return PCAP_ERROR_NOT_ACTIVATED
        }
    }

    if (handle == NULL) {
        // If we cannot get a handle at all, return early
        // The functions require a pcap_t pointer, even if invalid
        return 0;
    }

    // Test pcap_major_version
    ret = pcap_major_version(handle);
    (void)ret; // Suppress unused warning

    // Test pcap_minor_version
    ret = pcap_minor_version(handle);
    (void)ret;

    // Test pcap_datalink_ext
    ret = pcap_datalink_ext(handle);
    (void)ret;

    // Test pcap_is_swapped
    ret = pcap_is_swapped(handle);
    (void)ret;

    // Test pcap_snapshot
    ret = pcap_snapshot(handle);
    (void)ret;

    // Test pcap_bufsize
    ret = pcap_bufsize(handle);
    (void)ret;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}