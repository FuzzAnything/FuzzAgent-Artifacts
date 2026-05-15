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

static int validate_ether_string(const char *str) {
    if (!str || strlen(str) != 17) {
        return 0;
    }
    
    for (int i = 0; i < 17; i++) {
        int pos = i % 3;
        char c = str[i];
        
        if (pos == 2) {
            if (c != ':' && c != '.' && c != '-') {
                return 0;
            }
        } else {
            if (!((c >= '0' && c <= '9') || 
                  (c >= 'a' && c <= 'f') || 
                  (c >= 'A' && c <= 'F'))) {
                return 0;
            }
        }
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. pcap_lib_version
    const char *version = pcap_lib_version();
    if (version) {
        // Diagnostic output can be used during debugging
        // fprintf(stderr, "Version: %s\n", version);
    }

    // 2. pcap_strerror
    // Use first byte as error number
    int err = Data[0] % 256;
    const char *err_str = pcap_strerror(err);
    if (err_str) {
        // fprintf(stderr, "Error %d: %s\n", err, err_str);
    }

    // 3. pcap_ether_aton
    // Create null-terminated string from fuzzer data
    size_t str_len = Size > 100 ? 100 : Size;
    char *ether_str = (char *)malloc(str_len + 1);
    if (ether_str) {
        memcpy(ether_str, Data, str_len);
        ether_str[str_len] = '\0';
        
        // Validate before calling
        if (validate_ether_string(ether_str)) {
            u_char *mac = pcap_ether_aton(ether_str);
            if (mac) {
                free(mac);
            }
        }
        free(ether_str);
    }

    // 4. pcap_open_offline
    // Write fuzzer data to file and try to open it
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

    // 5. pcap_ether_hostton
    // Use fuzzer data as hostname string
    size_t hostname_len = Size > 50 ? 50 : Size;
    char *hostname = (char *)malloc(hostname_len + 1);
    if (hostname) {
        memcpy(hostname, Data, hostname_len);
        hostname[hostname_len] = '\0';
        
        u_char *mac2 = pcap_ether_hostton(hostname);
        if (mac2) {
            free(mac2);
        }
        free(hostname);
    }

    // 6. pcap_next_etherent
    // Create a file with fuzzer data and read Ethernet addresses from it
    FILE *fp2 = fopen("./dummy_file", "wb");
    if (fp2) {
        fwrite(Data, 1, Size, fp2);
        fclose(fp2);
        
        FILE *fp3 = fopen("./dummy_file", "rb");
        if (fp3) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp3)) != NULL) {
                // Do nothing, just parse entries
            }
            fclose(fp3);
        }
    }

    return 0;
}