// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for pcap_dump_open
    write_dummy_file(Data, Size);
    
    // Initialize pcap handle using pcap_open_dead since we need an activated handle
    // DLT_EN10MB (Ethernet) is a common link type
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap_handle) return 0;
    
    // Prepare filter expression from fuzzer input
    // Use first part of input as filter string, null-terminated
    size_t filter_len = Size > 256 ? 256 : Size;
    char *filter_str = (char *)malloc(filter_len + 1);
    if (!filter_str) {
        pcap_close(pcap_handle);
        return 0;
    }
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';
    
    // Prepare bpf_program structure
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // 1. pcap_compile
    // Use optimize=1 and netmask=0 (common values)
    int compile_result = pcap_compile(pcap_handle, &fp, filter_str, 1, 0);
    
    // 2. pcap_geterr (after compile)
    char *err1 = pcap_geterr(pcap_handle);
    (void)err1; // Use variable to avoid unused warning
    
    // 3. pcap_setfilter (only if compile succeeded)
    if (compile_result == 0) {
        int setfilter_result = pcap_setfilter(pcap_handle, &fp);
        (void)setfilter_result;
        
        // 4. pcap_geterr (after setfilter)
        char *err2 = pcap_geterr(pcap_handle);
        (void)err2;
    }
    
    // 5. pcap_dump_open
    pcap_dumper_t *dumper = pcap_dump_open(pcap_handle, "./dummy_file");
    
    // 6. pcap_geterr (after dump_open)
    char *err3 = pcap_geterr(pcap_handle);
    (void)err3;
    
    // 7. pcap_datalink
    int datalink = pcap_datalink(pcap_handle);
    (void)datalink;
    
    // Cleanup
    if (dumper) {
        pcap_dump_close(dumper);
    }
    pcap_freecode(&fp);
    free(filter_str);
    pcap_close(pcap_handle);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}