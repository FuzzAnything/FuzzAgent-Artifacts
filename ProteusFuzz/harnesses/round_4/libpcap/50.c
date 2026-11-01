// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_stats at pcap.c:3848:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_sendpacket at pcap.c:4137:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

static void create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal libpcap file header (magic, version, snaplen, linktype, zone, sigfigs)
        // Magic number: 0xa1b2c3d4 (standard pcap)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        int32_t sigfigs = 0;
        int32_t snaplen = 65535;
        int32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);

        // Write some dummy packet data if available
        if (Size > 0) {
            // Write a packet header
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.caplen = (Size < 1500) ? Size : 1500;
            hdr.len = hdr.caplen;
            fwrite(&hdr, sizeof(hdr), 1, f);
            fwrite(Data, 1, hdr.caplen, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_stat stats;
    struct pcap_pkthdr *header;
    const u_char *packet;
    int ret;

    if (Size < 1) {
        return 0;
    }

    // Create a dummy file to use for pcap_open
    create_dummy_file(Data, Size);

    // Attempt to open the file source
    // Using "file:./dummy_file" to explicitly open the file we created
    handle = pcap_open("file:./dummy_file", 65535, 0, 0, NULL, errbuf);

    if (handle != NULL) {
        // 1. Test pcap_major_version
        // Requires activated handle. pcap_open on a file usually activates it.
        ret = pcap_major_version(handle);
        (void)ret;

        // 2. Test pcap_is_swapped
        ret = pcap_is_swapped(handle);
        (void)ret;

        // 3. Test pcap_stats
        memset(&stats, 0, sizeof(stats));
        ret = pcap_stats(handle, &stats);
        (void)ret;

        // 4. Test pcap_next_ex
        // Loop to try to read packets until EOF or error
        while ((ret = pcap_next_ex(handle, &header, &packet)) == 1) {
            // Packet read successfully, do nothing or minimal processing
            // header and packet are valid here
            (void)header;
            (void)packet;
        }
        // ret == -1 (error) or -2 (EOF/timeout) is expected exit condition

        // 5. Test pcap_sendpacket
        // This usually fails on file handles or non-injection capable handles,
        // but we call it to exercise the error path.
        if (Size > 0) {
            ret = pcap_sendpacket(handle, Data, (int)Size);
            (void)ret;
        }

        // Cleanup
        pcap_close(handle);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}