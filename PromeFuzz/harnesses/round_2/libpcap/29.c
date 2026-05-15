// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    FILE *fp = NULL;
    pcap_t *pcap = NULL;
    FILE *result_file = NULL;
    int minor_ver;
    
    // Write fuzzer input to dummy file
    fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test 1: pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        pcap = pcap_fopen_offline(fp, errbuf);
        if (pcap) {
            // Test pcap_file on the opened pcap
            result_file = pcap_file(pcap);
            // Test pcap_minor_version
            minor_ver = pcap_minor_version(pcap);
            cleanup_pcap(pcap);
            pcap = NULL;
        } else {
            fclose(fp);
        }
    }
    
    // Test 2: pcap_open_offline_with_tstamp_precision
    // Use different timestamp precisions
    for (u_int precision = 0; precision <= 1; precision++) {
        pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
        if (pcap) {
            result_file = pcap_file(pcap);
            minor_ver = pcap_minor_version(pcap);
            cleanup_pcap(pcap);
            pcap = NULL;
        }
    }
    
    // Test 3: pcap_open_dead_with_tstamp_precision
    // Use fuzzer data to generate parameters
    if (Size >= 3) {
        int linktype = Data[0] % 256;
        int snaplen = (Data[1] << 8) | Data[2];
        u_int precision = Data[0] % 2; // 0 or 1
        
        pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
        if (pcap) {
            result_file = pcap_file(pcap);
            minor_ver = pcap_minor_version(pcap);
            cleanup_pcap(pcap);
            pcap = NULL;
        }
    }
    
    // Test 4: pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        for (u_int precision = 0; precision <= 1; precision++) {
            rewind(fp);
            pcap = pcap_fopen_offline_with_tstamp_precision(fp, precision, errbuf);
            if (pcap) {
                result_file = pcap_file(pcap);
                minor_ver = pcap_minor_version(pcap);
                cleanup_pcap(pcap);
                pcap = NULL;
            }
        }
        fclose(fp);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}