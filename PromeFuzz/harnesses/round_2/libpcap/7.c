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
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
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

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Create dummy file for pcap_dump_open
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fclose(fp);
    
    // Initialize pcap handle using pcap_open_dead since we need an activated handle
    int linktype = DLT_EN10MB; // Ethernet link type
    int snaplen = 65535;
    pcap_t *pcap_handle = pcap_open_dead(linktype, snaplen);
    if (!pcap_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Prepare filter string from fuzzer input
    char *filter_str = NULL;
    if (Size > 0) {
        filter_str = (char *)malloc(Size + 1);
        if (!filter_str) {
            pcap_close(pcap_handle);
            cleanup_dummy_file();
            return 0;
        }
        memcpy(filter_str, Data, Size);
        filter_str[Size] = '\0';
    } else {
        filter_str = strdup("");
        if (!filter_str) {
            pcap_close(pcap_handle);
            cleanup_dummy_file();
            return 0;
        }
    }
    
    struct bpf_program bpf_prog;
    bpf_u_int32 netmask = 0;
    int optimize = 1;
    
    // 1. pcap_compile
    int compile_ret = pcap_compile(pcap_handle, &bpf_prog, filter_str, optimize, netmask);
    
    // 2. pcap_geterr (first call)
    char *err_msg1 = pcap_geterr(pcap_handle);
    (void)err_msg1; // Use error message to avoid unused variable warning
    
    // 3. pcap_setfilter (only if compilation succeeded)
    if (compile_ret == 0) {
        int setfilter_ret = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_ret; // Use return value to avoid unused variable warning
    }
    
    // 4. pcap_geterr (second call)
    char *err_msg2 = pcap_geterr(pcap_handle);
    (void)err_msg2;
    
    // 5. pcap_dump_open
    pcap_dumper_t *dumper = pcap_dump_open(pcap_handle, "./dummy_file");
    
    // 6. pcap_geterr (third call)
    char *err_msg3 = pcap_geterr(pcap_handle);
    (void)err_msg3;
    
    // 7. pcap_datalink
    int datalink_ret = pcap_datalink(pcap_handle);
    (void)datalink_ret;
    
    // Cleanup
    if (compile_ret == 0) {
        pcap_freecode(&bpf_prog);
    }
    if (dumper) {
        pcap_dump_close(dumper);
    }
    free(filter_str);
    pcap_close(pcap_handle);
    cleanup_dummy_file();
    
    return 0;
}