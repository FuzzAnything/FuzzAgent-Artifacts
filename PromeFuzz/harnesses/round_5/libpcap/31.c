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
    if (!str || strlen(str) != 17) return 0;
    
    for (int i = 0; i < 17; i++) {
        if (i % 3 == 2) {
            if (str[i] != ':' && str[i] != '.' && str[i] != '-') return 0;
        } else {
            if (!((str[i] >= '0' && str[i] <= '9') ||
                  (str[i] >= 'a' && str[i] <= 'f') ||
                  (str[i] >= 'A' && str[i] <= 'F'))) return 0;
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Test pcap_lib_version */
    const char *version = pcap_lib_version();
    (void)version;

    /* Test pcap_strerror with various errno values */
    int err = Data[0] % 256;
    const char *errstr = pcap_strerror(err);
    (void)errstr;

    /* Test pcap_ether_aton with valid string from fuzzer input */
    if (Size >= 18) {
        char ether_str[18];
        memcpy(ether_str, Data, 17);
        ether_str[17] = '\0';
        
        if (is_valid_ether_string(ether_str)) {
            u_char *ether_addr = pcap_ether_aton(ether_str);
            if (ether_addr) free(ether_addr);
        }
    }

    /* Test pcap_open_offline with dummy file */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
        if (pcap) pcap_close(pcap);
    }

    /* Test pcap_ether_hostton with string from fuzzer input */
    if (Size > 0) {
        char *hostname = (char *)malloc(Size + 1);
        if (hostname) {
            memcpy(hostname, Data, Size);
            hostname[Size] = '\0';
            u_char *ether_addr = pcap_ether_hostton(hostname);
            if (ether_addr) free(ether_addr);
            free(hostname);
        }
    }

    /* Test pcap_next_etherent with dummy file */
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        fp = fopen("./dummy_file", "rb");
        if (fp) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                /* Process etherent if needed */
            }
            fclose(fp);
        }
    }

    return 0;
}