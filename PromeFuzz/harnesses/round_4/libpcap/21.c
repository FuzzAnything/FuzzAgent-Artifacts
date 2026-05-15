// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void test_pcap_handle(pcap_t *pcap) {
    if (pcap == NULL) {
        return;
    }
    
    // Test version functions
    int major = pcap_major_version(pcap);
    int minor = pcap_minor_version(pcap);
    
    // Test timestamp precision function
    int prec = pcap_get_tstamp_precision(pcap);
    
    // Use the results to avoid compiler warnings about unused variables
    (void)major;
    (void)minor;
    (void)prec;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) {
        return 0;
    }
    
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test pcap_open_offline
    pcap_t *pcap1 = pcap_open_offline("./dummy_file", errbuf);
    test_pcap_handle(pcap1);
    if (pcap1 != NULL) {
        pcap_close(pcap1);
    }
    
    // Test pcap_open_offline_with_tstamp_precision with different precisions
    pcap_t *pcap2 = pcap_open_offline_with_tstamp_precision("./dummy_file", 
                                                            PCAP_TSTAMP_PRECISION_MICRO, 
                                                            errbuf);
    test_pcap_handle(pcap2);
    if (pcap2 != NULL) {
        pcap_close(pcap2);
    }
    
    pcap_t *pcap3 = pcap_open_offline_with_tstamp_precision("./dummy_file", 
                                                            PCAP_TSTAMP_PRECISION_NANO, 
                                                            errbuf);
    test_pcap_handle(pcap3);
    if (pcap3 != NULL) {
        pcap_close(pcap3);
    }
    
    // Test pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp != NULL) {
        pcap_t *pcap4 = pcap_fopen_offline_with_tstamp_precision(fp, 
                                                                PCAP_TSTAMP_PRECISION_MICRO, 
                                                                errbuf);
        test_pcap_handle(pcap4);
        if (pcap4 != NULL) {
            pcap_close(pcap4);
        } else {
            fclose(fp);
        }
        // Note: pcap_fopen_offline_with_tstamp_precision closes the FILE* on success
        // so we only close it on failure
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}