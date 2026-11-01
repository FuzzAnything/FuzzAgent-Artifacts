// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
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

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy savefile to avoid needing a live network interface
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f == NULL) {
        return 0;
    }

    // Write a minimal pcap header (magic number, version, etc.)
    // Magic number: 0xa1b2c3d4 (standard pcap)
    uint32_t magic = 0xa1b2c3d4;
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

    // Write some dummy packet data if available
    // Ensure we have enough data before reading
    if (Size >= 4) {
        // Write a minimal packet header
        uint32_t ts_sec = Data[0];
        uint32_t ts_usec = Data[1] * 256;
        uint32_t incl_len = Data[2];
        uint32_t orig_len = Data[3];

        fwrite(&ts_sec, sizeof(ts_sec), 1, f);
        fwrite(&ts_usec, sizeof(ts_usec), 1, f);
        fwrite(&incl_len, sizeof(incl_len), 1, f);
        fwrite(&orig_len, sizeof(orig_len), 1, f);

        // Write packet data
        size_t data_len = Size - 4;
        if (data_len > 0) {
            fwrite(Data + 4, 1, data_len, f);
        }
    } else {
        // If not enough data for a full packet, write a minimal valid packet
        uint32_t ts_sec = 0;
        uint32_t ts_usec = 0;
        uint32_t incl_len = 0;
        uint32_t orig_len = 0;
        fwrite(&ts_sec, sizeof(ts_sec), 1, f);
        fwrite(&ts_usec, sizeof(ts_usec), 1, f);
        fwrite(&incl_len, sizeof(incl_len), 1, f);
        fwrite(&orig_len, sizeof(orig_len), 1, f);
    }

    fclose(f);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(dummy_file, errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Activate the handle
    if (pcap_activate(handle) != 0) {
        cleanup_pcap(handle);
        return 0;
    }

    // Fuzz the target API functions
    int major_ver = pcap_major_version(handle);
    int minor_ver = pcap_minor_version(handle);
    int datalink_ext = pcap_datalink_ext(handle);
    int is_swapped = pcap_is_swapped(handle);
    int snapshot = pcap_snapshot(handle);
    int bufsize = pcap_bufsize(handle);

    // Use the return values to ensure they are compiled and not optimized away
    volatile int unused = major_ver + minor_ver + datalink_ext + is_swapped + snapshot + bufsize;
    (void)unused;

    cleanup_pcap(handle);
    return 0;
}