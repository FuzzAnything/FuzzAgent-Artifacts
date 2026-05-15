// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static void cleanup_file(FILE *file) {
    if (file && file != stdin) {
        fclose(file);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file for offline functions
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Error buffer for functions that require it
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test pcap_open_offline_with_tstamp_precision
    // Use different timestamp precisions based on input data
    u_int precisions[] = {PCAP_TSTAMP_PRECISION_MICRO, PCAP_TSTAMP_PRECISION_NANO};
    for (size_t i = 0; i < sizeof(precisions)/sizeof(precisions[0]); i++) {
        pcap_t *pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", 
                                                               precisions[i], 
                                                               errbuf);
        if (pcap) {
            // Test pcap_file on the opened pcap
            FILE *pcap_fp = pcap_file(pcap);
            (void)pcap_fp; // Use result to avoid unused variable warning
            
            // Test pcap_minor_version
            int minor_ver = pcap_minor_version(pcap);
            (void)minor_ver; // Use result
            
            cleanup_pcap(pcap);
        }
    }
    
    // Test pcap_open_dead_with_tstamp_precision
    // Extract parameters from input data
    int linktype = Size > 0 ? Data[0] % 256 : DLT_EN10MB;
    int snaplen = Size > 1 ? Data[1] * 256 + (Size > 2 ? Data[2] : 0) : 65535;
    if (snaplen < 0) snaplen = -snaplen;
    
    for (size_t i = 0; i < sizeof(precisions)/sizeof(precisions[0]); i++) {
        pcap_t *dead_pcap = pcap_open_dead_with_tstamp_precision(linktype, 
                                                                snaplen, 
                                                                precisions[i]);
        if (dead_pcap) {
            FILE *dead_fp = pcap_file(dead_pcap);
            (void)dead_fp;
            
            int dead_minor = pcap_minor_version(dead_pcap);
            (void)dead_minor;
            
            cleanup_pcap(dead_pcap);
        }
    }
    
    // Test pcap_fopen_offline_with_tstamp_precision
    FILE *offline_file = fopen("./dummy_file", "rb");
    if (offline_file) {
        for (size_t i = 0; i < sizeof(precisions)/sizeof(precisions[0]); i++) {
            // Reset file pointer to beginning
            rewind(offline_file);
            
            pcap_t *offline_pcap = pcap_fopen_offline_with_tstamp_precision(offline_file,
                                                                           precisions[i],
                                                                           errbuf);
            if (offline_pcap) {
                FILE *offline_fp = pcap_file(offline_pcap);
                (void)offline_fp;
                
                int offline_minor = pcap_minor_version(offline_pcap);
                (void)offline_minor;
                
                cleanup_pcap(offline_pcap);
                // Note: pcap_close will close the FILE* for us
                offline_file = NULL; // Prevent double close
                break; // File is consumed, can't reuse
            }
        }
        if (offline_file) {
            fclose(offline_file);
        }
    }
    
    // Test pcap_fopen_offline
    FILE *simple_offline_file = fopen("./dummy_file", "rb");
    if (simple_offline_file) {
        pcap_t *simple_pcap = pcap_fopen_offline(simple_offline_file, errbuf);
        if (simple_pcap) {
            FILE *simple_fp = pcap_file(simple_pcap);
            (void)simple_fp;
            
            int simple_minor = pcap_minor_version(simple_pcap);
            (void)simple_minor;
            
            cleanup_pcap(simple_pcap);
            simple_offline_file = NULL; // Prevent double close
        }
        if (simple_offline_file) {
            fclose(simple_offline_file);
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}