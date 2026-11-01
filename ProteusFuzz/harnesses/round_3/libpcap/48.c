// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a dummy file for offline capture testing
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write minimal pcap header (magic number, version, etc.)
        // Magic: 0xa1b2c3d4 (microseconds)
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
        // Write a dummy packet if data is available
        if (Size > 4) {
            uint32_t ts_sec = 0;
            uint32_t ts_usec = 0;
            uint32_t incl_len = Size - 4;
            uint32_t orig_len = Size - 4;
            fwrite(&ts_sec, sizeof(ts_sec), 1, f);
            fwrite(&ts_usec, sizeof(ts_usec), 1, f);
            fwrite(&incl_len, sizeof(incl_len), 1, f);
            fwrite(&orig_len, sizeof(orig_len), 1, f);
            fwrite(Data + 4, 1, Size - 4, f);
        }
        fclose(f);
    }

    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret;

    // Try to open the dummy file
    handle = pcap_open_offline(filename, errbuf);
    if (!handle) {
        // Fallback: try to open live on "lo" or "any" if offline fails (often fails in sandbox)
        handle = pcap_create("lo", errbuf);
        if (handle) {
            ret = pcap_activate(handle);
            if (ret != 0) {
                pcap_close(handle);
                handle = NULL;
            }
        }
        if (!handle) {
            handle = pcap_create("any", errbuf);
            if (handle) {
                ret = pcap_activate(handle);
                if (ret != 0) {
                    pcap_close(handle);
                    handle = NULL;
                }
            }
        }
    }

    if (!handle) {
        return 0;
    }

    // 1. pcap_set_datalink
    // Try setting to a few common DLTs based on input data
    int dlt = Data[0] % 100; 
    // Common DLTs: 1 (Ethernet), 101 (NULL), 228 (IPv4), 229 (IPv6)
    if (dlt == 0) dlt = 1;
    else if (dlt == 1) dlt = 101;
    else if (dlt == 2) dlt = 228;
    else if (dlt == 3) dlt = 229;
    else dlt = Data[1] % 500; // Randomish

    ret = pcap_set_datalink(handle, dlt);
    (void)ret; // Ignore return for fuzzing, just trigger state

    // 2. pcap_set_timeout
    // Only valid if not activated, but we might have activated it.
    // If activated, it returns error, which is fine for fuzzing.
    int timeout_val = (int)(Data[2] * 100 + Data[3]);
    if (timeout_val < 0) timeout_val = -timeout_val;
    ret = pcap_set_timeout(handle, timeout_val);
    (void)ret;

    // 3. pcap_stats
    struct pcap_stat stats;
    memset(&stats, 0, sizeof(stats));
    ret = pcap_stats(handle, &stats);
    (void)ret;

    // 4. pcap_dispatch
    // cnt = 1 to avoid blocking too long in live mode, or process 1 packet in file mode
    ret = pcap_dispatch(handle, 1, dummy_callback, NULL);
    (void)ret;

    // 5. pcap_loop
    // cnt = 1
    ret = pcap_loop(handle, 1, dummy_callback, NULL);
    (void)ret;

    // 6. pcap_sendpacket
    // Prepare a small dummy packet buffer
    u_char packet_buf[256];
    int pkt_len = 0;
    if (Size > 4) {
        pkt_len = Size - 4;
        if (pkt_len > 256) pkt_len = 256;
        memcpy(packet_buf, Data + 4, pkt_len);
    } else {
        // Create a minimal dummy packet if input is too small
        memset(packet_buf, 0, 14);
        pkt_len = 14;
    }
    
    if (pkt_len > 0) {
        ret = pcap_sendpacket(handle, packet_buf, pkt_len);
        (void)ret;
    }

    // Cleanup
    pcap_close(handle);
    
    // Remove dummy file
    unlink(filename);

    return 0;
}