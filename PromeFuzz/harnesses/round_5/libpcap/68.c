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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int validate_ether_string(const char *str) {
    if (!str || strlen(str) < 17) return 0;
    
    int separators = 0;
    char sep = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (i % 3 == 2) {
            if (str[i] != ':' && str[i] != '.' && str[i] != '-') return 0;
            if (sep == 0) sep = str[i];
            else if (str[i] != sep) return 0;
            separators++;
        } else {
            if (!((str[i] >= '0' && str[i] <= '9') ||
                  (str[i] >= 'a' && str[i] <= 'f') ||
                  (str[i] >= 'A' && str[i] <= 'F'))) return 0;
        }
    }
    return separators == 5 && str[strlen(str)-1] != sep;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // 1. pcap_lib_version
    const char *version = pcap_lib_version();
    (void)version;

    // 2. pcap_strerror - test with various errno values
    int err_val = Data[0] % 256;
    const char *err_str = pcap_strerror(err_val);
    (void)err_str;

    // 3. pcap_ether_aton - validate and test with string from input
    if (Size >= 18) {
        char *ether_str = (char *)malloc(Size + 1);
        if (ether_str) {
            memcpy(ether_str, Data, Size);
            ether_str[Size] = '\0';
            
            if (validate_ether_string(ether_str)) {
                u_char *mac = pcap_ether_aton(ether_str);
                if (mac) free(mac);
            }
            free(ether_str);
        }
    }

    // 4. pcap_open_offline - write input to file and try to open
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
        if (pcap) {
            pcap_close(pcap);
        }
    }

    // 5. pcap_ether_hostton - test with string from input
    if (Size > 0) {
        char *hostname = (char *)malloc(Size + 1);
        if (hostname) {
            memcpy(hostname, Data, Size);
            hostname[Size] = '\0';
            
            u_char *mac = pcap_ether_hostton(hostname);
            if (mac) free(mac);
            free(hostname);
        }
    }

    // 6. pcap_next_etherent - read from file created earlier
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        struct pcap_etherent *ether;
        while ((ether = pcap_next_etherent(fp)) != NULL) {
            // Just read entries until NULL
        }
        fclose(fp);
    }

    return 0;
}