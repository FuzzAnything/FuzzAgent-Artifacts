// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
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

static void setup_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal pcap header (24 bytes)
        // Magic number, version, timezone, sigfigs, snaplen, network
        uint8_t header[24] = {
            0xa1, 0xb2, 0xc3, 0xd4, // Magic number
            0x02, 0x00, 0x04, 0x00, // Version major/minor
            0x00, 0x00, 0x00, 0x00, // Timezone
            0x00, 0x00, 0x00, 0x00, // Sigfigs
            0x00, 0x00, 0x00, 0x00, // Snaplen
            0x01, 0x00, 0x00, 0x00  // Network type (Ethernet)
        };
        fwrite(header, 1, 24, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract values from input for fuzzing parameters
    int timeout_val = Data[0];
    int buffer_size_val = Data[1];
    int snaplen = Size > 2 ? Data[2] : 65535;
    int flags = Size > 3 ? Data[3] : 0;
    int read_timeout = Size > 4 ? Data[4] : 1000;
    int promisc = Size > 5 ? (Data[5] % 2) : 0;
    int live_timeout = Size > 6 ? Data[6] : 1000;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_t *live_handle = NULL;
    pcap_t *open_handle = NULL;

    // 1. pcap_statustostr
    const char *status_str = pcap_statustostr(PCAP_ERROR);
    (void)status_str; // Suppress unused warning

    // Create a handle to test set_timeout and set_buffer_size
    handle = pcap_create("any", errbuf);
    if (handle) {
        // 2. pcap_set_timeout
        int ret_timeout = pcap_set_timeout(handle, timeout_val);
        (void)ret_timeout;

        // 3. pcap_statustostr
        status_str = pcap_statustostr(ret_timeout);
        (void)status_str;

        // 4. pcap_set_buffer_size
        int ret_buffer = pcap_set_buffer_size(handle, buffer_size_val);
        (void)ret_buffer;

        // 5. pcap_statustostr
        status_str = pcap_statustostr(ret_buffer);
        (void)status_str;

        // 6. pcap_activate
        int ret_activate = pcap_activate(handle);
        (void)ret_activate;

        // 7. pcap_statustostr
        status_str = pcap_statustostr(ret_activate);
        (void)status_str;

        // 8. pcap_geterr
        char *err = pcap_geterr(handle);
        (void)err;

        // 9. pcap_statustostr
        status_str = pcap_statustostr(PCAP_ERROR);
        (void)status_str;

        // 10. pcap_geterr
        err = pcap_geterr(handle);
        (void)err;

        // Cleanup handle
        pcap_close(handle);
        handle = NULL;
    }

    // 11. pcap_open
    // Use a dummy file for file-based capture
    setup_dummy_file("./dummy_file");
    open_handle = pcap_open("./dummy_file", snaplen, flags, read_timeout, NULL, errbuf);
    if (open_handle) {
        pcap_close(open_handle);
        open_handle = NULL;
    }

    // 12. pcap_open_live
    live_handle = pcap_open_live("any", snaplen, promisc, live_timeout, errbuf);
    if (live_handle) {
        // 13. pcap_close
        pcap_close(live_handle);
        live_handle = NULL;
    }

    return 0;
}