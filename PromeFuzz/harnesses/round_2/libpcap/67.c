// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"
#include <unistd.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    // Handle empty input
    if (Size == 0) {
        return 0;
    }

    // 1. Test pcap_lib_version (no dependencies)
    const char *version = pcap_lib_version();
    (void)version; // Use version to avoid unused variable warning

    // 2. Test pcap_strerror with various errno values
    // Use first byte to generate an errno value
    int err_val = Data[0] % 256;
    const char *err_str = pcap_strerror(err_val);
    (void)err_str;

    // 3. Test pcap_ether_aton with null-terminated string from fuzz data
    // Ensure we have enough data and create null-terminated string
    size_t str_len = Size > 100 ? 100 : Size;
    char *eth_str = (char *)malloc(str_len + 1);
    if (eth_str) {
        memcpy(eth_str, Data, str_len);
        eth_str[str_len] = '\0';
        
        u_char *eth_addr = pcap_ether_aton(eth_str);
        if (eth_addr) {
            free(eth_addr);
        }
        free(eth_str);
    }

    // 4. Test pcap_ether_hostton with another null-terminated string
    // Use different portion of data
    size_t hostname_len = Size > 50 ? 50 : Size;
    char *hostname = (char *)malloc(hostname_len + 1);
    if (hostname) {
        // Start from middle of data if possible
        size_t start_pos = Size > 100 ? 100 : 0;
        size_t copy_len = (Size - start_pos) > hostname_len ? hostname_len : (Size - start_pos);
        if (copy_len > 0) {
            memcpy(hostname, Data + start_pos, copy_len);
            hostname[copy_len] = '\0';
            
            u_char *mac_addr = pcap_ether_hostton(hostname);
            if (mac_addr) {
                free(mac_addr);
            }
        }
        free(hostname);
    }

    // 5. Test pcap_next_etherent with file input
    // Write fuzz data to a file and parse it
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        fp = fopen("./dummy_file", "rb");
        if (fp) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                // Just consume the data
                (void)etherent;
            }
            fclose(fp);
        }
        unlink("./dummy_file");
    }

    // 6. Test pcap_geterr - need a pcap_t structure first
    // Create a dummy pcap structure using pcap_open_dead
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap) {
        // Try to get error (should be empty or default)
        char *err = pcap_geterr(pcap);
        (void)err;
        
        // Try to compile a bogus filter to potentially set an error
        struct bpf_program fp;
        char filter_str[64];
        if (Size > 0) {
            // Create a simple filter string from first few bytes
            size_t filter_len = Size > 63 ? 63 : Size;
            memcpy(filter_str, Data, filter_len);
            filter_str[filter_len] = '\0';
            
            // This may fail and set error
            int compile_result = pcap_compile(pcap, &fp, filter_str, 0, 0);
            if (compile_result == 0) {
                pcap_freecode(&fp);
            }
            
            // Get error again
            err = pcap_geterr(pcap);
            (void)err;
        }
        
        pcap_close(pcap);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif