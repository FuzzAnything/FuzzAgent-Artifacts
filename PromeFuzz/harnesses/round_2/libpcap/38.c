// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which function to test
    if (Size == 0) return 0;
    
    uint8_t function_selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (function_selector) {
        case 0: { // pcap_ether_aton
            if (remaining_size == 0) break;
            
            // Limit input size to prevent overflow
            size_t max_len = 64; // Reasonable limit for Ethernet address string
            size_t str_len = remaining_size < max_len ? remaining_size : max_len;
            
            // Create null-terminated string from fuzzer input
            char *eth_str = (char *)malloc(str_len + 1);
            if (!eth_str) break;
            
            memcpy(eth_str, remaining_data, str_len);
            eth_str[str_len] = '\0';
            
            u_char *result = pcap_ether_aton(eth_str);
            if (result) {
                free(result);
            }
            
            free(eth_str);
            break;
        }
        
        case 1: { // pcap_next_etherent
            if (remaining_size == 0) break;
            
            // Write fuzzer data to dummy file
            FILE *fp = fopen("./dummy_file", "wb");
            if (!fp) break;
            
            fwrite(remaining_data, 1, remaining_size, fp);
            fclose(fp);
            
            // Open for reading and parse
            fp = fopen("./dummy_file", "rb");
            if (!fp) break;
            
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                // Just parse through entries
            }
            
            fclose(fp);
            break;
        }
        
        case 2: { // pcap_ether_hostton
            if (remaining_size == 0) break;
            
            // Limit input size to prevent overflow
            size_t max_len = 256; // Reasonable limit for hostname
            size_t str_len = remaining_size < max_len ? remaining_size : max_len;
            
            // Create null-terminated hostname from fuzzer input
            char *hostname = (char *)malloc(str_len + 1);
            if (!hostname) break;
            
            memcpy(hostname, remaining_data, str_len);
            hostname[str_len] = '\0';
            
            u_char *result = pcap_ether_hostton(hostname);
            if (result) {
                free(result);
            }
            
            free(hostname);
            break;
        }
        
        case 3: { // pcap_lib_version
            const char *version = pcap_lib_version();
            // Just call it - no cleanup needed
            break;
        }
        
        case 4: { // pcap_open_offline
            if (remaining_size == 0) break;
            
            // Write fuzzer data to dummy file
            FILE *fp = fopen("./dummy_file", "wb");
            if (!fp) break;
            
            fwrite(remaining_data, 1, remaining_size, fp);
            fclose(fp);
            
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
            if (pcap) {
                pcap_close(pcap);
            }
            break;
        }
        
        case 5: { // pcap_strerror
            if (remaining_size == 0) break;
            
            // Use fuzzer data to create an error number
            int errnum = 0;
            for (size_t i = 0; i < remaining_size && i < sizeof(int); i++) {
                errnum |= (remaining_data[i] << (8 * i));
            }
            
            const char *errstr = pcap_strerror(errnum);
            // Just call it - no cleanup needed
            break;
        }
    }
    
    return 0;
}