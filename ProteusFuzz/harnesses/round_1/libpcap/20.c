// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Create a dummy file for savefile mode
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write minimal pcap header
        uint32_t magic = 0xa1b2c3d4; // Standard pcap magic
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);

        // Write some dummy packet data if Size > 0
        if (Size > 0) {
            uint32_t ts_sec = 0;
            uint32_t ts_usec = 0;
            uint32_t incl_len = Size;
            uint32_t orig_len = Size;

            fwrite(&ts_sec, sizeof(ts_sec), 1, f);
            fwrite(&ts_usec, sizeof(ts_usec), 1, f);
            fwrite(&incl_len, sizeof(incl_len), 1, f);
            fwrite(&orig_len, sizeof(orig_len), 1, f);
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    // Attempt to open the dummy file
    handle = pcap_open_offline("./dummy_file", errbuf);

    if (handle == NULL) {
        // Fallback to pcap_create if open_offline fails (e.g., invalid header)
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            ret = pcap_activate(handle);
            // Proceed regardless of activation status to test error paths
        }
    }

    if (handle != NULL) {
        // Test pcap_is_swapped
        ret = pcap_is_swapped(handle);
        (void)ret;

        // Test pcap_snapshot
        ret = pcap_snapshot(handle);
        (void)ret;

        // Test pcap_datalink_ext
        ret = pcap_datalink_ext(handle);
        (void)ret;

        // Test pcap_bufsize
        ret = pcap_bufsize(handle);
        (void)ret;

        // Test pcap_major_version
        ret = pcap_major_version(handle);
        (void)ret;

        // Test pcap_minor_version
        ret = pcap_minor_version(handle);
        (void)ret;

        // Cleanup
        pcap_close(handle);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}