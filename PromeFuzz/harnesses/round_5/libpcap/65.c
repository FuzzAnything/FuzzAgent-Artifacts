// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int is_valid_ether_string(const char *str) {
    int i = 0;
    int hexdigits = 0;
    int separators = 0;
    char sep = '\0';
    
    if (!str || strlen(str) < 12) return 0;
    
    while (str[i] != '\0') {
        if ((str[i] >= '0' && str[i] <= '9') || 
            (str[i] >= 'a' && str[i] <= 'f') || 
            (str[i] >= 'A' && str[i] <= 'F')) {
            hexdigits++;
            i++;
            continue;
        }
        
        if (str[i] == ':' || str[i] == '.' || str[i] == '-') {
            if (sep == '\0') sep = str[i];
            else if (sep != str[i]) return 0;
            
            separators++;
            i++;
            continue;
        }
        
        return 0;
    }
    
    if (hexdigits != 12) return 0;
    if (separators != 5) return 0;
    if (str[strlen(str) - 1] == ':' || 
        str[strlen(str) - 1] == '.' || 
        str[strlen(str) - 1] == '-') return 0;
    
    return 1;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    const char *version = pcap_lib_version();
    if (version) {
        (void)version;
    }
    
    int err = Data[0] % 256;
    const char *errstr = pcap_strerror(err);
    if (errstr) {
        (void)errstr;
    }
    
    char *str = (char *)malloc(Size + 1);
    if (str) {
        memcpy(str, Data, Size);
        str[Size] = '\0';
        
        if (is_valid_ether_string(str)) {
            u_char *addr = pcap_ether_aton(str);
            if (addr) {
                free(addr);
            }
        }
        
        u_char *host_addr = pcap_ether_hostton(str);
        if (host_addr) {
            free(host_addr);
        }
        
        free(str);
    }
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
        if (pcap) {
            pcap_close(pcap);
        }
        
        fp = fopen("./dummy_file", "rb");
        if (fp) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                (void)etherent;
            }
            fclose(fp);
        }
        
        remove("./dummy_file");
    }
    
    return 0;
}