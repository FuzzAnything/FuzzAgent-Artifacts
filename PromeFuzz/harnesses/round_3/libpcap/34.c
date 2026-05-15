// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static void cleanup_file(FILE *fp) {
    if (fp && fp != stdin) {
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzzer input to dummy file
    FILE *dummy_fp = fopen("./dummy_file", "wb");
    if (!dummy_fp) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_fp);
    fclose(dummy_fp);

    // Use first byte to determine timestamp precision and link type
    uint8_t control_byte = Data[0];
    u_int timestamp_precision = (control_byte & 0x01) ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
    int linktype = (control_byte >> 1) & 0x7F;
    if (linktype == 0) {
        linktype = DLT_EN10MB; // Default to Ethernet if 0
    }

    char errbuf[PCAP_ERRBUF_SIZE];

    // Test pcap_open_offline_with_tstamp_precision
    pcap_t *pcap1 = pcap_open_offline_with_tstamp_precision("./dummy_file", timestamp_precision, errbuf);
    if (pcap1) {
        FILE *file1 = pcap_file(pcap1);
        int minor1 = pcap_minor_version(pcap1);
        cleanup_pcap(pcap1);
    }

    // Test pcap_fopen_offline
    FILE *fp1 = fopen("./dummy_file", "rb");
    if (fp1) {
        pcap_t *pcap2 = pcap_fopen_offline(fp1, errbuf);
        if (pcap2) {
            FILE *file2 = pcap_file(pcap2);
            int minor2 = pcap_minor_version(pcap2);
            cleanup_pcap(pcap2);
        } else {
            cleanup_file(fp1);
        }
    }

    // Test pcap_fopen_offline_with_tstamp_precision
    FILE *fp2 = fopen("./dummy_file", "rb");
    if (fp2) {
        pcap_t *pcap3 = pcap_fopen_offline_with_tstamp_precision(fp2, timestamp_precision, errbuf);
        if (pcap3) {
            FILE *file3 = pcap_file(pcap3);
            int minor3 = pcap_minor_version(pcap3);
            cleanup_pcap(pcap3);
        } else {
            cleanup_file(fp2);
        }
    }

    // Test pcap_open_dead_with_tstamp_precision
    pcap_t *pcap4 = pcap_open_dead_with_tstamp_precision(linktype, 65535, timestamp_precision);
    if (pcap4) {
        FILE *file4 = pcap_file(pcap4);
        int minor4 = pcap_minor_version(pcap4);
        cleanup_pcap(pcap4);
    }

    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}