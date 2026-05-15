// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for file-based APIs
    write_dummy_file(Data, Size);
    
    // Test pcap_open_dead_with_tstamp_precision
    {
        int linktype = (Size > 0) ? Data[0] : 1;  // DLT_EN10MB by default
        int snaplen = (Size > 1) ? Data[1] : 65535;
        u_int precision = (Size > 2) ? Data[2] : 0;
        
        pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
        if (pcap) {
            // Test pcap_file on the dead pcap (should return NULL for dead captures)
            FILE *fp = pcap_file(pcap);
            (void)fp; // Unused, just testing the call
            
            // Test pcap_minor_version (should return PCAP_ERROR_NOT_ACTIVATED for dead captures)
            int minor = pcap_minor_version(pcap);
            (void)minor; // Unused
            
            pcap_close(pcap);
        }
    }
    
    // Test pcap_open_offline_with_tstamp_precision
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        u_int precision = (Size > 0) ? Data[0] : 0;
        
        pcap_t *pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", precision, errbuf);
        if (pcap) {
            // Test pcap_file on the opened pcap
            FILE *fp = pcap_file(pcap);
            if (fp) {
                // Test pcap_minor_version
                int minor = pcap_minor_version(pcap);
                (void)minor; // Unused
            }
            pcap_close(pcap);
        }
    }
    
    // Test pcap_fopen_offline and pcap_fopen_offline_with_tstamp_precision
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        FILE *fp = fopen("./dummy_file", "rb");
        if (fp) {
            // Test pcap_fopen_offline
            pcap_t *pcap1 = pcap_fopen_offline(fp, errbuf);
            if (pcap1) {
                // Test pcap_file
                FILE *fp2 = pcap_file(pcap1);
                (void)fp2; // Unused
                
                // Test pcap_minor_version
                int minor = pcap_minor_version(pcap1);
                (void)minor; // Unused
                
                pcap_close(pcap1);
            } else {
                // If pcap_fopen_offline failed, rewind and try with precision version
                rewind(fp);
                
                u_int precision = (Size > 0) ? Data[0] : 0;
                pcap_t *pcap2 = pcap_fopen_offline_with_tstamp_precision(fp, precision, errbuf);
                if (pcap2) {
                    // Test pcap_file
                    FILE *fp3 = pcap_file(pcap2);
                    (void)fp3; // Unused
                    
                    // Test pcap_minor_version
                    int minor = pcap_minor_version(pcap2);
                    (void)minor; // Unused
                    
                    pcap_close(pcap2);
                }
            }
            fclose(fp);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}