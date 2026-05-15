// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write data to dummy file for potential file operations
    write_dummy_file(Data, Size);
    
    // Initialize variables
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp = 0, maskp = 0;
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Create dummy pcap handle using pcap_open_dead
    int linktype = DLT_EN10MB;
    int snaplen = 65535;
    pcap_t *pcap_handle = pcap_open_dead(linktype, snaplen);
    if (!pcap_handle) return 0;
    
    // First pcap_statustostr -> pcap_geterr sequence
    int status1 = (int)Data[0] - 128; // Generate both positive and negative values
    const char *status_str1 = pcap_statustostr(status1);
    char *err1 = pcap_geterr(pcap_handle);
    
    // Second pcap_statustostr -> pcap_geterr sequence
    int status2 = (int)Data[1 % Size];
    const char *status_str2 = pcap_statustostr(status2);
    char *err2 = pcap_geterr(pcap_handle);
    
    // pcap_lookupnet - use first few bytes as device name
    char device_name[256];
    size_t device_len = Size < 255 ? Size : 255;
    memcpy(device_name, Data, device_len);
    device_name[device_len] = '\0';
    
    int lookup_result = pcap_lookupnet(device_name, &netp, &maskp, errbuf);
    
    // pcap_compile - use a null-terminated filter string
    // Create a null-terminated copy of the input data for filter string
    char *filter_str = NULL;
    if (Size > 0) {
        // Limit filter string size to avoid excessive memory usage
        size_t filter_len = Size > 1024 ? 1024 : Size;
        filter_str = (char *)malloc(filter_len + 1);
        if (filter_str) {
            memcpy(filter_str, Data, filter_len);
            filter_str[filter_len] = '\0';
        }
    }
    
    int compile_result = -1;
    if (filter_str) {
        int optimize = Data[Size/2] & 1;
        bpf_u_int32 netmask = maskp;
        
        compile_result = pcap_compile(pcap_handle, &fp, filter_str, optimize, netmask);
        free(filter_str);
    }
    char *err3 = pcap_geterr(pcap_handle);
    
    // pcap_setfilter
    int setfilter_result = -1;
    if (compile_result == 0) {
        setfilter_result = pcap_setfilter(pcap_handle, &fp);
    }
    char *err4 = pcap_geterr(pcap_handle);
    
    // pcap_breakloop
    pcap_breakloop(pcap_handle);
    
    // pcap_close
    pcap_close(pcap_handle);
    
    // pcap_freecode
    pcap_freecode(&fp);
    
    // Try another sequence with different parameters
    if (Size > 8) {
        // Create another handle with different parameters
        pcap_t *pcap_handle2 = pcap_open_dead(DLT_RAW, 1500);
        if (pcap_handle2) {
            // Try different status codes
            int status3 = (int)Data[2 % Size] * 100;
            const char *status_str3 = pcap_statustostr(status3);
            
            // Try another filter compilation with safe string
            struct bpf_program fp2;
            memset(&fp2, 0, sizeof(fp2));
            
            // Create another null-terminated filter string
            char *filter_str2 = NULL;
            if (Size > 8) {
                size_t filter_len2 = (Size - 8) > 512 ? 512 : (Size - 8);
                filter_str2 = (char *)malloc(filter_len2 + 1);
                if (filter_str2) {
                    memcpy(filter_str2, Data + 8, filter_len2);
                    filter_str2[filter_len2] = '\0';
                }
            }
            
            if (filter_str2) {
                int compile_result2 = pcap_compile(pcap_handle2, &fp2, filter_str2, 1, 0xFFFFFF00);
                free(filter_str2);
                
                if (compile_result2 == 0) {
                    pcap_setfilter(pcap_handle2, &fp2);
                    pcap_freecode(&fp2);
                }
            }
            
            pcap_breakloop(pcap_handle2);
            pcap_close(pcap_handle2);
        }
    }
    
    return 0;
}