// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Use a dummy file path for savefile mode to avoid needing a live network interface
    const char *device = "./dummy_file";

    // Write the fuzzer input to the dummy file so pcap_open_offline can read it
    FILE *f = fopen(device, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    } else {
        return 0;
    }

    // Open the handle in offline mode (reading from file)
    handle = pcap_open_offline(device, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Activate the handle as required by the target functions
    ret = pcap_activate(handle);
    if (ret != 0) {
        pcap_close(handle);
        return 0;
    }

    // Fuzz target functions
    // 1. pcap_is_swapped
    ret = pcap_is_swapped(handle);
    (void)ret; // Use return value to prevent unused warnings

    // 2. pcap_major_version
    ret = pcap_major_version(handle);
    (void)ret;

    // 3. pcap_snapshot
    ret = pcap_snapshot(handle);
    (void)ret;

    // 4. pcap_minor_version
    ret = pcap_minor_version(handle);
    (void)ret;

    // 5. pcap_bufsize
    ret = pcap_bufsize(handle);
    (void)ret;

    // 6. pcap_datalink_ext
    ret = pcap_datalink_ext(handle);
    (void)ret;

    // Cleanup
    pcap_close(handle);

    return 0;
}