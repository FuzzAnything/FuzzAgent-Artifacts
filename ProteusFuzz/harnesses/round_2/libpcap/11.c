// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pcap.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    struct pcap_stat stats;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));
    memset(&fp, 0, sizeof(fp));
    memset(&stats, 0, sizeof(stats));

    // Create a dummy file for pcap_open_offline if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write minimal pcap header to make it a valid savefile
        // Magic number, version, timezone, sigfigs, snaplen, network type
        uint32_t header[6] = {
            0xa1b2c3d4, // magic number
            2,          // major version
            4,          // minor version
            0,          // timezone
            0,          // sigfigs
            65535,      // snaplen
            1           // linktype (Ethernet)
        };
        fwrite(header, sizeof(header), 1, f);
        // Write some dummy packet data if size allows
        if (Size > 0) {
            size_t write_size = (Size < 1000) ? Size : 1000;
            fwrite(Data, 1, write_size, f);
        }
        fclose(f);
    }

    // Attempt to open the dummy file
    handle = pcap_open_offline(dummy_file, errbuf);

    if (handle == NULL) {
        // If open fails, we cannot safely call pcap_geterr on a NULL handle.
        // Clean up and return.
        if (f) {
            fclose(f);
        }
        unlink(dummy_file);
        return 0;
    }

    // 1. pcap_geterr
    char *err1 = pcap_geterr(handle);
    (void)err1;

    // 2. pcap_setnonblock
    // Try both 0 and 1 based on input data to explore states
    int nonblock_mode = (Size > 0 && Data[0] % 2 == 0) ? 1 : 0;
    int ret_nonblock = pcap_setnonblock(handle, nonblock_mode, errbuf);
    (void)ret_nonblock;

    // 3. pcap_dispatch
    // Use input data to determine count (clamped to avoid huge numbers)
    int count = (Size > 0) ? (int)(Data[0] % 100 + 1) : 1;
    int ret_dispatch = pcap_dispatch(handle, count, dummy_callback, NULL);
    (void)ret_dispatch;

    // 4. pcap_stats
    int ret_stats = pcap_stats(handle, &stats);
    (void)ret_stats;

    // 5. pcap_geterr
    char *err2 = pcap_geterr(handle);
    (void)err2;

    // 6. pcap_geterr (again)
    char *err3 = pcap_geterr(handle);
    (void)err3;

    // 7. pcap_close
    pcap_close(handle);
    handle = NULL;

    // 8. pcap_freecode
    // We didn't compile a filter, so fp is zeroed. This is safe.
    pcap_freecode(&fp);

    // Cleanup dummy file
    unlink(dummy_file);

    return 0;
}