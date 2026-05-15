// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
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
#include <errno.h>

static void cleanup_file(const char *filename) {
    if (access(filename, F_OK) == 0) {
        unlink(filename);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    if (size > 0 && data) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Create a dummy pcap_t handle for offline operations
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return 0;
    }

    // Create a dummy file for file-based operations
    const char *dummy_filename = "./dummy_file";
    cleanup_file(dummy_filename);
    
    // Write some data to the dummy file (could be a minimal pcap header)
    if (write_dummy_file(Data, Size > 128 ? 128 : Size, dummy_filename) < 0) {
        pcap_close(pcap);
        return 0;
    }

    // Test pcap_dump_open
    pcap_dumper_t *dumper = pcap_dump_open(pcap, dummy_filename);
    if (dumper) {
        // Test pcap_dump_file
        FILE *dump_file = pcap_dump_file(dumper);
        if (dump_file) {
            // Try to write something to the file
            if (Size > 0) {
                fwrite(Data, 1, Size > 1024 ? 1024 : Size, dump_file);
                fflush(dump_file);
            }
        }
        pcap_dump_close(dumper);
    }

    // Test pcap_dump_open_append - requires an existing valid pcap file
    // First create a minimal valid pcap file
    cleanup_file(dummy_filename);
    pcap_t *pcap2 = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap2) {
        pcap_dumper_t *dumper2 = pcap_dump_open(pcap2, dummy_filename);
        if (dumper2) {
            // Write a minimal pcap header by writing a dummy packet
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.caplen = 0;
            hdr.len = 0;
            pcap_dump((u_char *)dumper2, &hdr, NULL);
            pcap_dump_close(dumper2);
            
            // Now try to append
            pcap_dumper_t *append_dumper = pcap_dump_open_append(pcap2, dummy_filename);
            if (append_dumper) {
                pcap_dump_close(append_dumper);
            }
        }
        pcap_close(pcap2);
    }

    // Test pcap_file - should return NULL for our pcap_open_dead handle
    FILE *pcap_f = pcap_file(pcap);
    if (pcap_f) {
        // This shouldn't happen for pcap_open_dead, but if it does, don't close it
        // as it's owned by libpcap
    }

    // Test pcap_dump_fopen
    cleanup_file(dummy_filename);
    FILE *fp = fopen(dummy_filename, "wb");
    if (fp) {
        pcap_dumper_t *fopen_dumper = pcap_dump_fopen(pcap, fp);
        if (fopen_dumper) {
            // Write a dummy packet
            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.caplen = Size > 64 ? 64 : Size;
            hdr.len = hdr.caplen;
            pcap_dump((u_char *)fopen_dumper, &hdr, Data);
            pcap_dump_close(fopen_dumper);
        } else {
            // If pcap_dump_fopen fails, we must not close fp
            // as libpcap takes ownership even on failure
        }
        // Note: DO NOT close fp here - libpcap owns it now
    }

    // Cleanup
    pcap_close(pcap);
    cleanup_file(dummy_filename);
    
    return 0;
}