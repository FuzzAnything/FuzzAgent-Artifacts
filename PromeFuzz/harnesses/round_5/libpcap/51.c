// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;
    
    // Create a dummy pcap handle for testing
    pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return 0;
    }
    
    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);
    
    // Test pcap_dump_open
    dumper = pcap_dump_open(pcap, "./dummy_file");
    if (dumper) {
        // Test pcap_dump_file
        fp = pcap_dump_file(dumper);
        (void)fp; // Use fp to avoid unused variable warning
        
        // Test pcap_dump_flush
        pcap_dump_flush(dumper);
        
        // Test pcap_dump_close
        pcap_dump_close(dumper);
        dumper = NULL;
    }
    
    // Test pcap_dump_open_append
    dumper = pcap_dump_open_append(pcap, "./dummy_file");
    if (dumper) {
        pcap_dump_flush(dumper);
        pcap_dump_close(dumper);
        dumper = NULL;
    }
    
    // Test pcap_dump_fopen
    fp = fopen("./dummy_file", "rb+");
    if (fp) {
        dumper = pcap_dump_fopen(pcap, fp);
        if (dumper) {
            pcap_dump_flush(dumper);
            pcap_dump_close(dumper);
            dumper = NULL;
        }
        // Note: Do NOT close fp here - pcap_dump_fopen takes ownership
    }
    
    // Cleanup
    if (pcap) {
        pcap_close(pcap);
    }
    cleanup_dummy_file();
    
    return 0;
}