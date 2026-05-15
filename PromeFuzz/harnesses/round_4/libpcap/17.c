// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write minimal pcap header if empty
        uint8_t empty_header[] = {0xd4, 0xc3, 0xb2, 0xa1, 0x02, 0x00, 0x04, 0x00};
        fwrite(empty_header, 1, sizeof(empty_header), f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    // Create dummy pcap_t handle for offline use
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return 0;
    }
    
    // Test pcap_dump_open
    write_dummy_file(Data, Size);
    pcap_dumper_t *dumper1 = pcap_dump_open(pcap, "./dummy_file");
    if (dumper1) {
        // Test pcap_dump_file
        FILE *dump_file = pcap_dump_file(dumper1);
        if (dump_file) {
            // Test pcap_dump_flush
            pcap_dump_flush(dumper1);
        }
        pcap_dump_close(dumper1);
    }
    
    // Test pcap_dump_open_append
    // First create a valid pcap file for appending
    pcap_dumper_t *temp_dumper = pcap_dump_open(pcap, "./dummy_file");
    if (temp_dumper) {
        pcap_dump_close(temp_dumper);
        pcap_dumper_t *dumper2 = pcap_dump_open_append(pcap, "./dummy_file");
        if (dumper2) {
            pcap_dump_close(dumper2);
        }
    }
    
    // Test pcap_dump_fopen
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        pcap_dumper_t *dumper3 = pcap_dump_fopen(pcap, fp);
        if (dumper3) {
            pcap_dump_flush(dumper3);
            pcap_dump_close(dumper3);
        } else {
            // If pcap_dump_fopen fails, library should close fp
            // So we don't close it here
        }
    }
    
    // Clean up
    pcap_close(pcap);
    unlink("./dummy_file");
    
    return 0;
}