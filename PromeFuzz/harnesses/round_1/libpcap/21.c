// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    FILE *file = NULL;
    
    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);
    
    // Test pcap_open_offline
    pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        // Test pcap_file on the opened pcap
        FILE *pcap_fp = pcap_file(pcap);
        (void)pcap_fp; // Use result to avoid unused variable warning
        
        // Test pcap_minor_version
        int minor_ver = pcap_minor_version(pcap);
        (void)minor_ver;
        
        cleanup_pcap(pcap);
        pcap = NULL;
    }
    
    // Test pcap_open_offline_with_tstamp_precision
    // Use first byte modulo 2 to choose between microsecond and nanosecond precision
    u_int precision = (Data[0] % 2 == 0) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
    pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
    if (pcap) {
        cleanup_pcap(pcap);
        pcap = NULL;
    }
    
    // Test pcap_fopen_offline and pcap_fopen_offline_with_tstamp_precision
    file = fopen("./dummy_file", "rb");
    if (file) {
        // Test pcap_fopen_offline
        pcap = pcap_fopen_offline(file, errbuf);
        if (pcap) {
            cleanup_pcap(pcap);
            pcap = NULL;
        }
        
        // Reopen file since pcap_fopen_offline may close it
        fclose(file);
        file = fopen("./dummy_file", "rb");
        if (file) {
            // Test pcap_fopen_offline_with_tstamp_precision
            // Use second byte (if available) for precision, otherwise use first
            u_int file_precision = PCAP_TSTAMP_PRECISION_MICRO;
            if (Size > 1) {
                file_precision = (Data[1] % 2 == 0) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
            }
            pcap = pcap_fopen_offline_with_tstamp_precision(file, file_precision, errbuf);
            if (pcap) {
                cleanup_pcap(pcap);
                pcap = NULL;
            }
            fclose(file);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}