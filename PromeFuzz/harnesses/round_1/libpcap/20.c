// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap.h"
#include "namedb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static void cleanup_ether_addr(u_char *addr) {
    if (addr) {
        free(addr);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // For pcap_strerror: use first 4 bytes as error number
    int errnum = 0;
    if (Size >= sizeof(int)) {
        memcpy(&errnum, Data, sizeof(int));
    } else if (Size > 0) {
        errnum = Data[0];
    }
    const char *errstr = pcap_strerror(errnum);
    (void)errstr;

    // For pcap_lib_version: simple call
    const char *version = pcap_lib_version();
    (void)version;

    // For pcap_open_offline: write data to file and try to open
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
        cleanup_pcap(pcap);
        unlink("./dummy_file");
    }

    // For pcap_next_etherent: write data to file and parse
    fp = fopen("./dummy_ethers", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        fp = fopen("./dummy_ethers", "r");
        if (fp) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                // Process each entry
                (void)etherent;
            }
            fclose(fp);
        }
        unlink("./dummy_ethers");
    }

    // For pcap_ether_hostton: use first part of data as hostname
    if (Size > 0) {
        // Create null-terminated string from input with safe bounds
        size_t str_len = Size < 1024 ? Size : 1023;
        char *hostname = (char *)malloc(str_len + 1);
        if (hostname) {
            memcpy(hostname, Data, str_len);
            hostname[str_len] = '\0';
            
            u_char *ether_addr = pcap_ether_hostton(hostname);
            cleanup_ether_addr(ether_addr);
            free(hostname);
        }
    }

    // For pcap_ether_aton: use first part of data as ethernet address string
    // Use a fixed-size buffer to avoid heap overflow issues
    if (Size > 0) {
        // Create a safe buffer with room for null terminator
        char ether_str[1024];
        size_t copy_len = Size < sizeof(ether_str) - 1 ? Size : sizeof(ether_str) - 1;
        memcpy(ether_str, Data, copy_len);
        ether_str[copy_len] = '\0';
        
        u_char *ether_addr = pcap_ether_aton(ether_str);
        cleanup_ether_addr(ether_addr);
    }

    return 0;
}