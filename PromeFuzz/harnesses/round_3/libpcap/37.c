// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
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
#include <unistd.h>
#include <errno.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Handle pcap_ether_hostton
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

    // Handle pcap_open_offline
    create_dummy_file(Data, Size);
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
    cleanup_dummy_file();

    // Handle pcap_ether_aton
    if (Size > 0) {
        char *addr_str = (char *)malloc(Size + 1);
        if (addr_str) {
            memcpy(addr_str, Data, Size);
            addr_str[Size] = '\0';
            u_char *mac = pcap_ether_aton(addr_str);
            if (mac) {
                free(mac);
            }
            free(addr_str);
        }
    }

    // Handle pcap_lib_version
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid unused variable warning

    // Handle pcap_strerror
    if (Size > 0) {
        int err = Data[0] % 256; // Use first byte for error code
        const char *errstr = pcap_strerror(err);
        (void)errstr; // Use result to avoid unused variable warning
    }

    // Handle pcap_next_etherent
    create_dummy_file(Data, Size);
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        struct pcap_etherent *ent;
        while ((ent = pcap_next_etherent(fp)) != NULL) {
            // Do nothing with the entry, just iterate through file
        }
        fclose(fp);
    }
    cleanup_dummy_file();

    return 0;
}