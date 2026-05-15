// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
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
#include <unistd.h>
#include <fcntl.h>

static void dummy_pcap_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Dummy handler for pcap_dispatch/pcap_loop if needed
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a dummy pcap handle using pcap_open_dead
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return 0;
    }

    // Prepare filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (filter_str == NULL) {
            pcap_close(pcap);
            return 0;
        }
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
    } else {
        filter_str = strdup("");
        if (filter_str == NULL) {
            pcap_close(pcap);
            return 0;
        }
    }

    // Initialize bpf_program structure to zero
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));

    // 1. pcap_compile
    int compile_ret = pcap_compile(pcap, &fp, filter_str, 0, 0);
    
    // 2. pcap_geterr
    char *err = pcap_geterr(pcap);
    (void)err; // Use err to avoid unused variable warning
    
    // 3. pcap_setfilter (only if compilation succeeded)
    int setfilter_ret = -1;
    if (compile_ret == 0) {
        setfilter_ret = pcap_setfilter(pcap, &fp);
        
        // 4. pcap_geterr
        err = pcap_geterr(pcap);
        (void)err;
    }

    // 5. pcap_dump_open
    const char *dump_filename = "./dummy_file";
    pcap_dumper_t *dumper = pcap_dump_open(pcap, dump_filename);
    
    // 6. pcap_geterr
    err = pcap_geterr(pcap);
    (void)err;
    
    // 7. pcap_datalink
    int dlt = pcap_datalink(pcap);
    (void)dlt;

    // Cleanup
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
    
    // Always free the bpf_program structure
    pcap_freecode(&fp);
    
    free(filter_str);
    pcap_close(pcap);
    
    return 0;
}