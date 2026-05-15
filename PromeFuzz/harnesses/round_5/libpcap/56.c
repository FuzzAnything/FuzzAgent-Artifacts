// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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

static int is_valid_ether_string(const char *str, size_t len) {
    if (len < 17) return 0; /* Minimum length for MAC address */
    if (len > 17) return 0; /* Maximum length for MAC address */
    
    /* Check format: xx:xx:xx:xx:xx:xx or xx-xx-xx-xx-xx-xx or xx.xx.xx.xx.xx.xx */
    for (size_t i = 0; i < len; i++) {
        if (i % 3 == 2) {
            /* Position for separator */
            if (str[i] != ':' && str[i] != '-' && str[i] != '.') {
                return 0;
            }
        } else {
            /* Position for hex digit */
            if (!((str[i] >= '0' && str[i] <= '9') ||
                  (str[i] >= 'a' && str[i] <= 'f') ||
                  (str[i] >= 'A' && str[i] <= 'F'))) {
                return 0;
            }
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* 1. Test pcap_ether_aton */
    if (Size >= 18) { /* Need space for null terminator */
        char *ether_str = (char *)malloc(Size + 1);
        if (!ether_str) return 0;
        
        memcpy(ether_str, Data, Size);
        ether_str[Size] = '\0';
        
        /* Validate string format before calling */
        if (is_valid_ether_string(ether_str, 17)) {
            u_char *result = pcap_ether_aton(ether_str);
            if (result) free(result);
        }
        free(ether_str);
    }
    
    /* 2. Test pcap_ether_hostton */
    if (Size > 0) {
        char *hostname = (char *)malloc(Size + 1);
        if (!hostname) return 0;
        
        memcpy(hostname, Data, Size);
        hostname[Size] = '\0';
        
        u_char *result = pcap_ether_hostton(hostname);
        if (result) free(result);
        free(hostname);
    }
    
    /* 3. Test pcap_lookupdev */
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        char *dev = pcap_lookupdev(errbuf);
        (void)dev; /* Result is static buffer, don't free */
    }
    
    /* 4. Test pcap_findalldevs */
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_if_t *alldevs = NULL;
        int ret = pcap_findalldevs(&alldevs, errbuf);
        if (ret == 0) {
            pcap_freealldevs(alldevs);
        }
    }
    
    /* 5. Test pcap_findalldevs_ex */
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_if_t *alldevs = NULL;
        struct pcap_rmtauth auth;
        
        /* Prepare source string from fuzz data */
        char *source = NULL;
        if (Size > 0) {
            source = (char *)malloc(Size + 1);
            if (source) {
                memcpy(source, Data, Size);
                source[Size] = '\0';
            }
        }
        
        /* Set up authentication structure */
        auth.type = RPCAP_RMTAUTH_NULL;
        auth.username = NULL;
        auth.password = NULL;
        
        int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
        if (ret == 0) {
            pcap_freealldevs(alldevs);
        }
        
        if (source) free(source);
    }
    
    /* 6. Test pcap_next_etherent */
    {
        /* Write fuzz data to dummy file */
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            /* Open for reading and test the function */
            fp = fopen("./dummy_file", "rb");
            if (fp) {
                struct pcap_etherent *ent;
                while ((ent = pcap_next_etherent(fp)) != NULL) {
                    /* Do nothing, just parse entries */
                }
                fclose(fp);
            }
            remove("./dummy_file");
        }
    }
    
    return 0;
}