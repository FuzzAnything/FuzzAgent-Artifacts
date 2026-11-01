// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
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

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *pkt) {
    (void)user;
    (void)header;
    (void)pkt;
    return;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_stat stats;
    int ret;

    if (Size < 1) {
        return 0;
    }

    // Use a dummy file for offline capture to avoid live interface dependencies
    // Write some dummy data to the file to ensure it's not empty
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        return 0;
    }

    // Attempt to open the file
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (!handle) {
        return 0;
    }

    // Fuzz pcap_dispatch
    // cnt can be derived from input data to vary behavior
    int cnt = (Size > 0) ? (Data[0] % 100) + 1 : 1;
    ret = pcap_dispatch(handle, cnt, dummy_callback, NULL);
    (void)ret; // Ignore return value for fuzzing stability

    // Fuzz pcap_loop
    // Loop for a small number of packets or -1 for infinite (handled by break_loop)
    int loop_cnt = (Size > 1) ? (Data[1] % 50) + 1 : 1;
    ret = pcap_loop(handle, loop_cnt, dummy_callback, NULL);
    (void)ret;

    // Fuzz pcap_set_datalink
    // Try setting various DLT types based on input data
    if (Size > 2) {
        int dlt = Data[2];
        ret = pcap_set_datalink(handle, dlt);
        (void)ret;
    }

    // Fuzz pcap_set_timeout
    // Note: pcap_set_timeout on an active handle (offline is considered active)
    // typically returns PCAP_ERROR_ACTIVATED. We still call it to test error handling.
    if (Size > 3) {
        int timeout = Data[3];
        ret = pcap_set_timeout(handle, timeout);
        (void)ret;
    }

    // Fuzz pcap_sendpacket
    // This usually fails on offline handles, but we test the path.
    if (Size > 4) {
        int pkt_len = Data[4] % (Size - 4);
        if (pkt_len > 0) {
            ret = pcap_sendpacket(handle, Data + 4, pkt_len);
            (void)ret;
        }
    }

    // Fuzz pcap_stats
    ret = pcap_stats(handle, &stats);
    (void)ret;

    // Cleanup
    pcap_close(handle);

    return 0;
}