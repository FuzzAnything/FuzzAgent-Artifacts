// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
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

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;

    // Use a dummy file for savefile-based capture to avoid needing live interfaces
    const char *dummy_file = "./dummy_file";
    
    // Write dummy data to the file if Size > 0 to ensure file exists and has content
    if (Size > 0) {
        FILE *f = fopen(dummy_file, "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
    }

    // Attempt to open the dummy file as a savefile
    // This creates a pcap_t handle without needing network interfaces
    handle = pcap_open_offline(dummy_file, errbuf);
    
    if (handle == NULL) {
        // If opening fails, we cannot test the target functions which require an active handle
        // However, we can still try to call them on NULL to test error handling if the API allows
        // But based on the description, they check for activation, not NULL pointer directly in a safe way usually.
        // Let's just return early if we can't get a handle.
        return 0;
    }

    // The target functions require the handle to be activated.
    // For savefiles opened with pcap_open_offline, the handle is typically considered active immediately.
    // However, to be safe and follow the "activated" requirement strictly, we can try pcap_activate.
    // Note: pcap_activate on an offline handle might fail or be a no-op depending on implementation.
    // If it fails, the handle remains inactive, and the target functions should return PCAP_ERROR_NOT_ACTIVATED.
    
    int activate_result = pcap_activate(handle);
    
    // We will call the target functions regardless of activation state to test both success and error paths.
    // This explores more program states as requested.

    // 1. pcap_snapshot
    int snap = pcap_snapshot(handle);
    (void)snap; // Use the return value to suppress warnings, logic is just to invoke

    // 2. pcap_minor_version
    int minor = pcap_minor_version(handle);
    (void)minor;

    // 3. pcap_major_version
    int major = pcap_major_version(handle);
    (void)major;

    // 4. pcap_datalink_ext
    int dlt_ext = pcap_datalink_ext(handle);
    (void)dlt_ext;

    // 5. pcap_bufsize
    int buf_size = pcap_bufsize(handle);
    (void)buf_size;

    // 6. pcap_is_swapped
    int swapped = pcap_is_swapped(handle);
    (void)swapped;

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}