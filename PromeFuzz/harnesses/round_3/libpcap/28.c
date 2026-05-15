// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
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
#include <time.h>

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

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for offline functions
    FILE *dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    // Test pcap_open_offline_with_tstamp_precision
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *pcap = pcap_open_offline_with_tstamp_precision(
            "./dummy_file", 
            Size % 3,  // Vary precision: 0=micro, 1=nano, 2=invalid
            errbuf
        );
        if (pcap) {
            // Test pcap_file on opened offline capture
            FILE *fp = pcap_file(pcap);
            (void)fp;  // Use result to avoid unused variable warning
            
            // Test pcap_minor_version
            int minor = pcap_minor_version(pcap);
            (void)minor;
            
            cleanup_pcap(pcap);
        }
    }

    // Test pcap_fopen_offline_with_tstamp_precision
    {
        FILE *file = fopen("./dummy_file", "rb");
        if (file) {
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(
                file,
                Size % 3,  // Vary precision
                errbuf
            );
            if (pcap) {
                // Test pcap_file
                FILE *fp = pcap_file(pcap);
                (void)fp;
                
                // Test pcap_minor_version
                int minor = pcap_minor_version(pcap);
                (void)minor;
                
                cleanup_pcap(pcap);
            } else {
                // File not closed by pcap_close since pcap is NULL
                cleanup_file(file);
            }
        }
    }

    // Test pcap_fopen_offline
    {
        FILE *file = fopen("./dummy_file", "rb");
        if (file) {
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_t *pcap = pcap_fopen_offline(file, errbuf);
            if (pcap) {
                // Test pcap_file
                FILE *fp = pcap_file(pcap);
                (void)fp;
                
                // Test pcap_minor_version
                int minor = pcap_minor_version(pcap);
                (void)minor;
                
                cleanup_pcap(pcap);
            } else {
                // File not closed by pcap_close since pcap is NULL
                cleanup_file(file);
            }
        }
    }

    // Test pcap_open_dead_with_tstamp_precision with various parameters
    {
        // Vary link type and snapshot length based on input
        int linktype = (Size % 255) + 1;  // Common link types
        int snaplen = Size % 65536;       // Reasonable snapshot lengths
        u_int precision = Size % 3;       // Vary precision
        
        pcap_t *dead_pcap = pcap_open_dead_with_tstamp_precision(
            linktype,
            snaplen,
            precision
        );
        
        if (dead_pcap) {
            // Test pcap_file on dead capture (should return NULL)
            FILE *fp = pcap_file(dead_pcap);
            (void)fp;
            
            // Test pcap_minor_version on dead capture
            int minor = pcap_minor_version(dead_pcap);
            (void)minor;
            
            cleanup_pcap(dead_pcap);
        }
    }

    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}