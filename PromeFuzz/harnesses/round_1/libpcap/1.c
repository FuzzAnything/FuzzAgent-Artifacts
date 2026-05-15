// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // 1. pcap_strerror
    // Use first byte as errno value
    int err = Data[0];
    const char *error_str = pcap_strerror(err);
    (void)error_str; // Use result to avoid unused variable warning

    // 2. pcap_open_offline
    // Write fuzzer data to dummy file and try to open it
    write_dummy_file(Data, Size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_offline = pcap_open_offline("./dummy_file", errbuf);
    
    // 3. pcap_datalink_name_to_val
    // Use part of fuzzer data as link-layer type name
    // Ensure we have at least 1 byte for null terminator
    size_t name_len = Size > 100 ? 100 : Size;
    char *link_name = malloc(name_len + 1);
    if (link_name) {
        memcpy(link_name, Data, name_len);
        link_name[name_len] = '\0';
        int dlt_val = pcap_datalink_name_to_val(link_name);
        (void)dlt_val; // Use result
        free(link_name);
    }

    // 4. pcap_open_dead
    // Use fuzzer data for linktype and snaplen
    int linktype = (Size > 0) ? Data[0] : 1;
    int snaplen = (Size > 1) ? Data[1] : 65535;
    pcap_t *pcap_dead = pcap_open_dead(linktype, snaplen);

    // 5. pcap_compile
    if (pcap_dead) {
        struct bpf_program fp;
        // Use part of fuzzer data as filter expression
        size_t filter_len = Size > 50 ? 50 : Size;
        char *filter = malloc(filter_len + 1);
        if (filter) {
            memcpy(filter, Data, filter_len);
            filter[filter_len] = '\0';
            
            bpf_u_int32 netmask = (Size > 4) ? 
                (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5] : 0xFFFFFF00;
            int optimize = (Size > 0) ? Data[0] & 1 : 0;
            
            int compile_result = pcap_compile(pcap_dead, &fp, filter, optimize, netmask);
            (void)compile_result; // Use result
            
            if (compile_result == 0) {
                pcap_freecode(&fp);
            }
            free(filter);
        }
        pcap_close(pcap_dead);
    }

    // Clean up offline pcap if opened
    if (pcap_offline) {
        pcap_close(pcap_offline);
    }

    cleanup_dummy_file();
    return 0;
}