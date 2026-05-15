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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. pcap_ether_hostton
    // Use first part of input as hostname (null-terminated)
    size_t hostname_len = Size > 1023 ? 1023 : Size;
    char *hostname = malloc(hostname_len + 1);
    if (hostname) {
        memcpy(hostname, Data, hostname_len);
        hostname[hostname_len] = '\0';
        u_char *mac = pcap_ether_hostton(hostname);
        if (mac) free(mac);
        free(hostname);
    }

    // 2. pcap_open_offline
    // Write input to file and try to open it
    write_dummy_file(Data, Size);
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }

    // 3. pcap_ether_aton
    // Use input as Ethernet address string (null-terminated)
    size_t eth_str_len = Size > 255 ? 255 : Size;
    char *eth_str = malloc(eth_str_len + 1);
    if (eth_str) {
        memcpy(eth_str, Data, eth_str_len);
        eth_str[eth_str_len] = '\0';
        u_char *eth_addr = pcap_ether_aton(eth_str);
        if (eth_addr) free(eth_addr);
        free(eth_str);
    }

    // 4. pcap_lib_version
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid unused variable warning

    // 5. pcap_strerror
    // Use first byte as error number
    int errnum = Data[0];
    const char *errstr = pcap_strerror(errnum);
    (void)errstr;

    // 6. pcap_next_etherent
    // Reuse the dummy file for reading Ethernet entries
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        struct pcap_etherent *etherent;
        while ((etherent = pcap_next_etherent(fp)) != NULL) {
            // Just consume the entry
        }
        fclose(fp);
    }

    remove_dummy_file();
    return 0;
}