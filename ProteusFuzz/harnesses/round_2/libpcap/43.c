// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write a minimal pcap global header (24 bytes) to make it a valid pcap file
        // Magic number (little endian), version major/minor, thiszone, sigfigs, snaplen, network
        uint8_t header[24] = {
            0xa1, 0xb2, 0xc2, 0xd3, // Magic number (little endian)
            0x02, 0x00, 0x04, 0x00, // Version major (2), minor (4)
            0x00, 0x00, 0x00, 0x00, // thiszone
            0x00, 0x00, 0x00, 0x00, // sigfigs
            0x00, 0x00, 0x00, 0x00, // snaplen (0)
            0x01, 0x00, 0x00, 0x00  // network (Ethernet)
        };
        fwrite(header, 1, 24, fp);
        
        // Write the fuzzing data as packet content (if any)
        if (Size > 0) {
            // Write a minimal packet header (16 bytes)
            uint8_t pkt_header[16];
            memset(pkt_header, 0, sizeof(pkt_header));
            // Set caplen and len to a safe value (min of Size and 65535)
            uint32_t len = (Size < 65535) ? Size : 65535;
            pkt_header[12] = len & 0xFF;
            pkt_header[13] = (len >> 8) & 0xFF;
            pkt_header[14] = (len >> 16) & 0xFF;
            pkt_header[15] = (len >> 24) & 0xFF;
            fwrite(pkt_header, 1, 16, fp);
            
            // Write the actual data
            fwrite(Data, 1, len, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *fp = NULL;
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Create a dummy pcap file with the fuzzing data
    create_dummy_file(Data, Size);

    // Test pcap_open_offline
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        // Test pcap_file
        FILE *stream = pcap_file(handle);
        (void)stream; // Use the return value to avoid warnings

        // Test pcap_next_ex
        ret = pcap_next_ex(handle, &header, &packet);
        // ret can be 1 (success), -1 (error), or -2 (EOF/timeout)
        (void)ret;
        (void)header;
        (void)packet;

        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_open_offline_with_tstamp_precision
    handle = pcap_open_offline_with_tstamp_precision("./dummy_file", PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (handle) {
        pcap_file(handle);
        pcap_next_ex(handle, &header, &packet);
        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline(fp, errbuf);
        if (handle) {
            pcap_file(handle);
            pcap_next_ex(handle, &header, &packet);
            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // Test pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline_with_tstamp_precision(fp, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
        if (handle) {
            pcap_file(handle);
            pcap_next_ex(handle, &header, &packet);
            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // Cleanup: remove dummy file
    remove("./dummy_file");

    return 0;
}