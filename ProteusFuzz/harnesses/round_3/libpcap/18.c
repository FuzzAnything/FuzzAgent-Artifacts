// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>

static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    if (size == 0) {
        // Create an empty file if size is 0
        FILE *f = fopen(filename, "wb");
        if (!f) return -1;
        fclose(f);
        return 0;
    }
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Minimum size required to safely access Data[0] through Data[5]
    if (Size < 6) {
        return 0;
    }

    // Create a dummy file if we have enough data to write
    if (create_dummy_file("./dummy_file", Data, Size) != 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    // Try to open the dummy file as a savefile
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // If offline open fails, try opening a live interface
        handle = pcap_create("lo", errbuf);
        if (handle == NULL) {
            handle = pcap_create("eth0", errbuf);
        }
        if (handle == NULL) {
            return 0;
        }
    }

    // Extract parameters from fuzz input safely
    int snaplen = Data[0];
    int rfmon = (Data[1] > 128) ? 1 : 0;
    int tstamp_type = Data[2];
    int timeout = Data[3];
    int buffer_size = Data[4];
    int promisc = (Data[5] > 128) ? 1 : 0;

    // Diversify inputs slightly to test edge cases
    int snaplen_variants[] = {snaplen, 0, 65535, -1, 1500};
    int rfmon_variants[] = {rfmon, 0, 1, -1, 2};
    int tstamp_variants[] = {tstamp_type, 0, 1, 2, 3, -1};
    int timeout_variants[] = {timeout, 0, 100, -1, 60000};
    int buffer_variants[] = {buffer_size, 0, 1024, -1, 1000000};
    int promisc_variants[] = {promisc, 0, 1, -1, 2};

    // Test pcap_set_snaplen
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_snaplen(handle, snaplen_variants[i]);
        (void)ret;
    }

    // Test pcap_set_rfmon
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_rfmon(handle, rfmon_variants[i]);
        (void)ret;
    }

    // Test pcap_set_tstamp_type
    for (int i = 0; i < 6; i++) {
        ret = pcap_set_tstamp_type(handle, tstamp_variants[i]);
        (void)ret;
    }

    // Test pcap_set_timeout
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_timeout(handle, timeout_variants[i]);
        (void)ret;
    }

    // Test pcap_set_buffer_size
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_buffer_size(handle, buffer_variants[i]);
        (void)ret;
    }

    // Test pcap_set_promisc
    for (int i = 0; i < 5; i++) {
        ret = pcap_set_promisc(handle, promisc_variants[i]);
        (void)ret;
    }

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}