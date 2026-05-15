// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
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
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    FILE *fp = NULL;
    int minor_version;
    FILE *pcap_fp;
    
    // Test pcap_open_offline
    pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        minor_version = pcap_minor_version(pcap);
        pcap_fp = pcap_file(pcap);
        pcap_close(pcap);
    }
    
    // Test pcap_open_offline_with_tstamp_precision with microsecond precision
    pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", 
                                                   PCAP_TSTAMP_PRECISION_MICRO, 
                                                   errbuf);
    if (pcap) {
        minor_version = pcap_minor_version(pcap);
        pcap_fp = pcap_file(pcap);
        pcap_close(pcap);
    }
    
    // Test pcap_open_offline_with_tstamp_precision with nanosecond precision
    pcap = pcap_open_offline_with_tstamp_precision("./dummy_file", 
                                                   PCAP_TSTAMP_PRECISION_NANO, 
                                                   errbuf);
    if (pcap) {
        minor_version = pcap_minor_version(pcap);
        pcap_fp = pcap_file(pcap);
        pcap_close(pcap);
    }
    
    // Test pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        pcap = pcap_fopen_offline(fp, errbuf);
        if (pcap) {
            minor_version = pcap_minor_version(pcap);
            pcap_fp = pcap_file(pcap);
            pcap_close(pcap);
        } else {
            fclose(fp);
        }
    }
    
    // Test pcap_fopen_offline_with_tstamp_precision with microsecond precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        pcap = pcap_fopen_offline_with_tstamp_precision(fp, 
                                                        PCAP_TSTAMP_PRECISION_MICRO, 
                                                        errbuf);
        if (pcap) {
            minor_version = pcap_minor_version(pcap);
            pcap_fp = pcap_file(pcap);
            pcap_close(pcap);
        } else {
            fclose(fp);
        }
    }
    
    // Test pcap_fopen_offline_with_tstamp_precision with nanosecond precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        pcap = pcap_fopen_offline_with_tstamp_precision(fp, 
                                                        PCAP_TSTAMP_PRECISION_NANO, 
                                                        errbuf);
        if (pcap) {
            minor_version = pcap_minor_version(pcap);
            pcap_fp = pcap_file(pcap);
            pcap_close(pcap);
        } else {
            fclose(fp);
        }
    }
    
    remove_dummy_file();
    return 0;
}