// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // 1. pcap_ether_aton
    if (Size > 0) {
        // Create null-terminated string from fuzzer input
        char *eth_str = malloc(Size + 1);
        if (eth_str) {
            memcpy(eth_str, Data, Size);
            eth_str[Size] = '\0';
            u_char *result = pcap_ether_aton(eth_str);
            if (result) free(result);
            free(eth_str);
        }
    }

    // 2. pcap_lookupdev
    char *dev = pcap_lookupdev(NULL);
    (void)dev; // Suppress unused warning

    // 3. pcap_next_etherent
    write_dummy_file(Data, Size);
    FILE *fp = fopen("./dummy_file", "r");
    if (fp) {
        struct pcap_etherent *etherent;
        while ((etherent = pcap_next_etherent(fp)) != NULL) {
            // Process each entry
        }
        fclose(fp);
    }

    // 4. pcap_ether_hostton
    if (Size > 0) {
        char *hostname = malloc(Size + 1);
        if (hostname) {
            memcpy(hostname, Data, Size);
            hostname[Size] = '\0';
            u_char *mac = pcap_ether_hostton(hostname);
            if (mac) free(mac);
            free(hostname);
        }
    }

    // 5. pcap_findalldevs
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0) {
        pcap_freealldevs(alldevs);
    }

    // 6. pcap_findalldevs_ex
    if (Size > 0) {
        // Create source string from fuzzer input
        char *source = malloc(Size + 1);
        if (source) {
            memcpy(source, Data, Size);
            source[Size] = '\0';
            
            // Test with NULL auth
            pcap_if_t *alldevs_ex = NULL;
            char errbuf_ex[PCAP_ERRBUF_SIZE];
            int ret_ex = pcap_findalldevs_ex(source, NULL, &alldevs_ex, errbuf_ex);
            if (ret_ex == 0) {
                pcap_freealldevs(alldevs_ex);
            }
            
            // Test with dummy auth structure
            struct pcap_rmtauth auth;
            auth.type = RPCAP_RMTAUTH_NULL;
            auth.username = NULL;
            auth.password = NULL;
            
            alldevs_ex = NULL;
            ret_ex = pcap_findalldevs_ex(source, &auth, &alldevs_ex, errbuf_ex);
            if (ret_ex == 0) {
                pcap_freealldevs(alldevs_ex);
            }
            
            free(source);
        }
    }

    return 0;
}