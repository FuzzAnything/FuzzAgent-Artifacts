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
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // 1. pcap_strerror - convert errno value to error string
    int err_val = 0;
    if (Size > 0) {
        err_val = Data[0] % 256;  // Use first byte for errno-like value
    }
    const char *err_str = pcap_strerror(err_val);
    (void)err_str;  // Use result to avoid unused variable warning
    
    // 2. pcap_open_offline - open saved capture file
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *filename = "./dummy_file";
    
    // Write fuzz data to dummy file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }
    
    pcap_t *pcap_offline = pcap_open_offline(filename, errbuf);
    
    // 3. pcap_datalink_name_to_val - convert link-layer name to DLT value
    // Extract a string from fuzz data for link-layer name
    char link_name[256];
    size_t name_len = Size < sizeof(link_name) - 1 ? Size : sizeof(link_name) - 1;
    if (name_len > 0) {
        memcpy(link_name, Data, name_len);
        link_name[name_len] = '\0';
    } else {
        link_name[0] = '\0';
    }
    int dlt_val = pcap_datalink_name_to_val(link_name);
    (void)dlt_val;  // Use result
    
    // 4. pcap_open_dead - create dummy pcap handle
    int linktype = 1;  // DLT_EN10MB
    int snaplen = 65535;
    if (Size > 1) {
        linktype = Data[0] % 256;
        snaplen = Data[1] % 65536;
    }
    pcap_t *pcap_dead = pcap_open_dead(linktype, snaplen);
    
    // 5. pcap_compile - compile filter expression
    if (pcap_dead != NULL) {
        struct bpf_program fp;
        memset(&fp, 0, sizeof(fp));
        
        // Extract filter string from fuzz data
        char filter[512];
        size_t filter_len = Size < sizeof(filter) - 1 ? Size : sizeof(filter) - 1;
        if (filter_len > 0) {
            memcpy(filter, Data, filter_len);
            filter[filter_len] = '\0';
        } else {
            filter[0] = '\0';
        }
        
        int optimize = 1;
        bpf_u_int32 netmask = 0xFFFFFF00;
        
        int compile_result = pcap_compile(pcap_dead, &fp, filter, optimize, netmask);
        (void)compile_result;  // Use result
        
        pcap_freecode(&fp);
        pcap_close(pcap_dead);
    }
    
    // Clean up offline pcap handle if opened
    if (pcap_offline != NULL) {
        pcap_close(pcap_offline);
    }
    
    // Remove dummy file
    unlink(filename);
    
    return 0;
}