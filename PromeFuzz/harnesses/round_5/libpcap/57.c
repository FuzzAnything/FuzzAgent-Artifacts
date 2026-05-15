// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
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
    if (len < 17 || len > 17) return 0;
    
    int separators[6] = {0};
    int sep_type = -1;
    
    for (int i = 0; i < 17; i++) {
        int pos = i % 3;
        char c = str[i];
        
        if (pos == 2) {
            if (c != ':' && c != '.' && c != '-') return 0;
            if (sep_type == -1) sep_type = c;
            else if (sep_type != c) return 0;
            separators[i/3] = 1;
        } else {
            if (!((c >= '0' && c <= '9') || 
                  (c >= 'a' && c <= 'f') || 
                  (c >= 'A' && c <= 'F'))) return 0;
        }
    }
    
    for (int i = 0; i < 6; i++) {
        if (!separators[i]) return 0;
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    
    pcap_findalldevs(&alldevs, errbuf);
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    pcap_findalldevs_ex(input_str, NULL, &alldevs, errbuf);
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    pcap_lookupdev(errbuf);
    
    if (is_valid_ether_string(input_str, Size)) {
        u_char *mac = pcap_ether_aton(input_str);
        if (mac) free(mac);
    }
    
    u_char *host_mac = pcap_ether_hostton(input_str);
    if (host_mac) free(host_mac);
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        fp = fopen("./dummy_file", "r");
        if (fp) {
            struct pcap_etherent *ether_entry;
            while ((ether_entry = pcap_next_etherent(fp)) != NULL) {
            }
            fclose(fp);
        }
    }
    
    free(input_str);
    return 0;
}