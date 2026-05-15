// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int is_hex_digit(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static int validate_ether_string(const char *str, size_t max_len) {
    if (!str || max_len < 17) return 0;
    
    int sep_count = 0;
    int hex_count = 0;
    char last_char = '\0';
    
    for (size_t i = 0; i < max_len && str[i] != '\0'; i++) {
        char c = str[i];
        
        if (c == ':' || c == '.' || c == '-') {
            if (hex_count != 2) return 0;
            if (i == max_len - 1) return 0;
            sep_count++;
            hex_count = 0;
        } else if (is_hex_digit(c)) {
            hex_count++;
            if (hex_count > 2) return 0;
        } else {
            return 0;
        }
        last_char = c;
    }
    
    if (sep_count != 5) return 0;
    if (hex_count != 2) return 0;
    if (last_char == ':' || last_char == '.' || last_char == '-') return 0;
    
    return 1;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test pcap_ether_aton
    {
        char *ether_str = (char *)malloc(Size + 1);
        if (ether_str) {
            memcpy(ether_str, Data, Size);
            ether_str[Size] = '\0';
            
            if (validate_ether_string(ether_str, Size)) {
                u_char *mac = pcap_ether_aton(ether_str);
                if (mac) {
                    free(mac);
                }
            }
            free(ether_str);
        }
    }
    
    // Test pcap_open
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        char *source = NULL;
        
        if (Size > 10) {
            source = (char *)malloc(Size + 1);
            if (source) {
                memcpy(source, Data, Size);
                source[Size] = '\0';
                
                pcap_t *pcap = pcap_open(source, 65535, PCAP_OPENFLAG_PROMISCUOUS,
                                         1000, NULL, errbuf);
                if (pcap) {
                    pcap_close(pcap);
                }
                free(source);
            }
        }
        
        // Test with dummy file
        FILE *dummy = fopen("./dummy_file", "wb");
        if (dummy) {
            if (Size > 0) {
                fwrite(Data, 1, Size, dummy);
            }
            fclose(dummy);
            
            pcap_t *pcap = pcap_open("file:./dummy_file", 65535, 0, 1000, NULL, errbuf);
            if (pcap) {
                pcap_close(pcap);
            }
        }
    }
    
    // Test pcap_lookupdev
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        char *dev = pcap_lookupdev(errbuf);
        (void)dev;
    }
    
    // Test pcap_next_etherent
    {
        FILE *dummy = fopen("./dummy_file", "wb");
        if (dummy) {
            if (Size > 0) {
                fwrite(Data, 1, Size, dummy);
            }
            fclose(dummy);
            
            dummy = fopen("./dummy_file", "rb");
            if (dummy) {
                struct pcap_etherent *ent;
                while ((ent = pcap_next_etherent(dummy)) != NULL) {
                    (void)ent;
                }
                fclose(dummy);
            }
        }
    }
    
    // Test pcap_lookupnet
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        bpf_u_int32 net, mask;
        
        if (Size > 0) {
            char *dev = (char *)malloc(Size + 1);
            if (dev) {
                memcpy(dev, Data, Size);
                dev[Size] = '\0';
                
                int ret = pcap_lookupnet(dev, &net, &mask, errbuf);
                (void)ret;
                free(dev);
            }
        }
    }
    
    // Test pcap_ether_hostton
    {
        if (Size > 0) {
            char *hostname = (char *)malloc(Size + 1);
            if (hostname) {
                memcpy(hostname, Data, Size);
                hostname[Size] = '\0';
                
                u_char *mac = pcap_ether_hostton(hostname);
                if (mac) {
                    free(mac);
                }
                free(hostname);
            }
        }
    }
    
    return 0;
}