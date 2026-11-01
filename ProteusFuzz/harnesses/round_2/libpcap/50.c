// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_next at pcap.c:554:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    (void)header;
    (void)packet;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_pkthdr *header;
    const u_char *packet;
    int ret;

    // Create a dummy file for testing file-based capture
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write minimal pcap header (magic, version, etc.) to make it a valid pcap file
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
        if (Size > 8) {
            uint32_t ts_sec = 1234567890;
            uint32_t ts_usec = 123456;
            uint32_t incl_len = Size - 8;
            uint32_t orig_len = Size - 8;

            fwrite(&ts_sec, sizeof(ts_sec), 1, f);
            fwrite(&ts_usec, sizeof(ts_usec), 1, f);
            fwrite(&incl_len, sizeof(incl_len), 1, f);
            fwrite(&orig_len, sizeof(orig_len), 1, f);
            fwrite(Data + 8, 1, Size - 8, f);
        }
        fclose(f);
    }

    // Try to open the dummy file
    handle = pcap_open(dummy_file, 65535, 0, 0, NULL, errbuf);
    if (!handle) {
        // Try opening "any" or a non-existent file to test error paths
        handle = pcap_open("nonexistent_device", 65535, 0, 0, NULL, errbuf);
        if (handle) {
            pcap_close(handle);
            return 0;
        }
        return 0;
    }

    // Test pcap_next
    struct pcap_pkthdr hdr;
    const u_char *pkt = pcap_next(handle, &hdr);
    (void)pkt; // Suppress unused variable warning

    // Test pcap_next_ex
    ret = pcap_next_ex(handle, &header, &packet);
    (void)ret;
    (void)packet;

    // Test pcap_dispatch with a small count
    ret = pcap_dispatch(handle, 1, dummy_callback, NULL);
    (void)ret;

    // Test pcap_loop with a small count
    // Note: pcap_loop can block indefinitely on live captures, but should work on files
    ret = pcap_loop(handle, 1, dummy_callback, NULL);
    (void)ret;

    // Test pcap_dump
    FILE *dump_file = fopen("./dump_output.pcap", "wb");
    if (dump_file) {
        pcap_dumper_t *dump = pcap_dump_open(dump_file, errbuf);
        if (dump) {
            // Try to dump a packet if we have one
            if (pkt) {
                pcap_dump((u_char *)dump, &hdr, pkt);
            }
            pcap_dump_close(dump);
        }
        fclose(dump_file);
    }

    // Cleanup
    pcap_close(handle);

    // Remove dummy files
    unlink(dummy_file);
    unlink("./dump_output.pcap");

    return 0;
}