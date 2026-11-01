// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
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

static int fuzzer_run(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret = 0;

    // Attempt to open a dummy file for fuzzing
    // pcap_open_offline is suitable as it doesn't require live hardware
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // If the file doesn't exist or is invalid, we can't test the target functions
        // which require an active handle. We return 0 to indicate no crash,
        // but no testing occurred for this input.
        return 0;
    }

    // The target functions require the handle to be activated.
    // pcap_open_offline implicitly activates the handle upon successful open.
    // However, to be safe and explicit per the requirements, we ensure activation.
    // Note: pcap_activate might fail if already activated or if it's a savefile
    // that doesn't support re-activation in some versions, but typically open_offline
    // leaves it active. If it returns an error here, the handle is not active.
    if (pcap_activate(handle) != 0) {
        pcap_close(handle);
        return 0;
    }

    // Fuzz the target API functions
    // 1. pcap_snapshot
    int snapshot_val = pcap_snapshot(handle);
    (void)snapshot_val; // Use value to prevent unused variable warnings

    // 2. pcap_minor_version
    int minor_ver = pcap_minor_version(handle);
    (void)minor_ver;

    // 3. pcap_bufsize
    int buf_size = pcap_bufsize(handle);
    (void)buf_size;

    // 4. pcap_is_swapped
    int is_swapped = pcap_is_swapped(handle);
    (void)is_swapped;

    // 5. pcap_major_version
    int major_ver = pcap_major_version(handle);
    (void)major_ver;

    // 6. pcap_datalink_ext
    int datalink_ext = pcap_datalink_ext(handle);
    (void)datalink_ext;

    // Cleanup
    pcap_close(handle);

    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Create a dummy file to satisfy pcap_open_offline requirements
    // We write some data to ensure the file exists and has content
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        // Write a minimal pcap header or just raw data to make the file valid-ish
        // A minimal pcap global header is 24 bytes.
        // Magic number (little endian): 0xa1b2c3d4
        // Version major: 2, minor: 4
        // This is a simplified attempt to create a valid-ish pcap file structure
        // to prevent immediate parse errors, though the content is arbitrary.
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        int32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, fp);
        fwrite(&ver_major, sizeof(ver_major), 1, fp);
        fwrite(&ver_minor, sizeof(ver_minor), 1, fp);
        fwrite(&thiszone, sizeof(thiszone), 1, fp);
        fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
        fwrite(&snaplen, sizeof(snaplen), 1, fp);
        fwrite(&network, sizeof(network), 1, fp);

        // Write some packet data based on fuzz input if available
        if (Size > 0) {
            // Write a minimal packet header
            struct {
                uint32_t ts_sec;
                uint32_t ts_usec;
                uint32_t incl_len;
                uint32_t orig_len;
            } pkt_header;
            pkt_header.ts_sec = 0;
            pkt_header.ts_usec = 0;
            pkt_header.incl_len = Size;
            pkt_header.orig_len = Size;
            fwrite(&pkt_header, sizeof(pkt_header), 1, fp);
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Run the fuzzing logic
    int result = fuzzer_run(Data, Size);

    // Cleanup the dummy file
    remove("./dummy_file");

    return result;
}