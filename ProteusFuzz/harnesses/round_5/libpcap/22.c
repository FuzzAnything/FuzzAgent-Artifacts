// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal pcap file header (24 bytes)
        // Magic number, version, timezone, sigfigs, snaplen, network
        uint8_t header[] = {
            0xa1, 0xb2, 0xc3, 0xd4, // Magic number (little endian)
            0x02, 0x00,             // Major version
            0x04, 0x00,             // Minor version
            0x00, 0x00, 0x00, 0x00, // Timezone
            0x00, 0x00, 0x00, 0x00, // Sigfigs
            0xff, 0xff, 0x00, 0x00, // Snaplen (65535)
            0x01, 0x00, 0x00, 0x00  // Network (Ethernet)
        };
        fwrite(header, 1, sizeof(header), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Prepare environment
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    
    // Use the first byte to decide between live (dummy device) and savefile
    int use_savefile = Data[0] % 2;
    int buffer_size_val = (int)(Data[1]);
    int rfmon_val = (Data[0] & 0x01);
    int promisc_val = (Data[0] & 0x02) ? 1 : 0;
    int snaplen_val = (int)(Data[1]);
    int timeout_val = (int)(Data[1]);
    int immediate_val = (Data[0] & 0x04) ? 1 : 0;

    // Ensure values are within reasonable bounds for testing
    if (buffer_size_val < 0) buffer_size_val = 0;
    if (snaplen_val < 0) snaplen_val = 0;
    if (timeout_val < 0) timeout_val = 0;

    if (use_savefile) {
        // Ensure dummy file exists
        setup_dummy_file();
        handle = pcap_open_offline("./dummy_file", errbuf);
    } else {
        // Try to open a live handle on a dummy device (likely to fail, but tests the code path)
        // Using "lo" or "eth0" might work on some systems, but "dummy" is safer for fuzzing
        // if the system doesn't have it, it returns NULL, which we handle.
        handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
        if (!handle) {
            // Fallback to a non-existent device name to ensure we get an inactive handle or NULL
            // If pcap_open_live fails immediately, handle is NULL.
            // We need an inactive handle to test the setters.
            // If we can't get a live handle, we try to open a savefile just for the setters.
            setup_dummy_file();
            handle = pcap_open_offline("./dummy_file", errbuf);
        }
    }

    if (!handle) {
        return 0;
    }

    // The target functions require an inactive handle.
    // pcap_open_offline returns an inactive handle.
    // pcap_open_live returns an active handle immediately.
    // We need to ensure we are testing on an INACTIVE handle.
    // If we got a live handle, it is already active, so the setters will return PCAP_ERROR_ACTIVATED.
    // This is a valid test case (testing the error path).
    // However, to test the success path, we ideally need a handle that is not activated.
    // pcap_open_offline is the best candidate for this.
    
    // If we have a live handle (active), we can still call the functions to test the error return.
    // If we have a savefile handle (inactive), we can test the success path.

    // Test pcap_set_buffer_size
    ret = pcap_set_buffer_size(handle, buffer_size_val);
    (void)ret; // Suppress unused result warning, fuzzing checks state

    // Test pcap_set_rfmon
    ret = pcap_set_rfmon(handle, rfmon_val);
    (void)ret;

    // Test pcap_set_promisc
    ret = pcap_set_promisc(handle, promisc_val);
    (void)ret;

    // Test pcap_set_snaplen
    ret = pcap_set_snaplen(handle, snaplen_val);
    (void)ret;

    // Test pcap_set_timeout
    ret = pcap_set_timeout(handle, timeout_val);
    (void)ret;

    // Test pcap_set_immediate_mode
    ret = pcap_set_immediate_mode(handle, immediate_val);
    (void)ret;

    // Cleanup
    pcap_close(handle);
    
    return 0;
}