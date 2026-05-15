// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static void cleanup_devices(pcap_if_t *alldevs) {
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_ether_addr(u_char *addr) {
    if (addr) {
        free(addr);
    }
}

static void cleanup_file(FILE *file) {
    if (file) {
        fclose(file);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test pcap_lookupdev */
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        char *dev = pcap_lookupdev(errbuf);
        (void)dev; /* Use result to avoid unused variable warning */
    }

    /* Test pcap_ether_hostton */
    {
        /* Create null-terminated hostname from fuzzer data */
        size_t hostname_len = Size > 255 ? 255 : Size;
        char *hostname = (char *)malloc(hostname_len + 1);
        if (hostname) {
            memcpy(hostname, Data, hostname_len);
            hostname[hostname_len] = '\0';
            
            u_char *ether_addr = pcap_ether_hostton(hostname);
            cleanup_ether_addr(ether_addr);
            
            free(hostname);
        }
    }

    /* Test pcap_findalldevs */
    {
        pcap_if_t *alldevs = NULL;
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        int ret = pcap_findalldevs(&alldevs, errbuf);
        (void)ret; /* Use result to avoid unused variable warning */
        cleanup_devices(alldevs);
    }

    /* Test pcap_ether_aton */
    {
        /* Create null-terminated MAC address string from fuzzer data */
        size_t mac_str_len = Size > 17 ? 17 : Size;
        char *mac_str = (char *)malloc(mac_str_len + 1);
        if (mac_str) {
            memcpy(mac_str, Data, mac_str_len);
            mac_str[mac_str_len] = '\0';
            
            u_char *ether_addr = pcap_ether_aton(mac_str);
            cleanup_ether_addr(ether_addr);
            
            free(mac_str);
        }
    }

    /* Test pcap_findalldevs_ex */
    {
        pcap_if_t *alldevs = NULL;
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        
        /* Create null-terminated source string from fuzzer data */
        size_t source_len = Size > 100 ? 100 : Size;
        char *source = (char *)malloc(source_len + 1);
        if (source) {
            memcpy(source, Data, source_len);
            source[source_len] = '\0';
            
            struct pcap_rmtauth auth;
            auth.type = RPCAP_RMTAUTH_NULL;
            auth.username = NULL;
            auth.password = NULL;
            
            int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
            (void)ret; /* Use result to avoid unused variable warning */
            cleanup_devices(alldevs);
            
            free(source);
        }
    }

    /* Test pcap_next_etherent */
    {
        if (write_dummy_file(Data, Size) == 0) {
            FILE *fp = fopen("./dummy_file", "rb");
            if (fp) {
                struct pcap_etherent *etherent;
                while ((etherent = pcap_next_etherent(fp)) != NULL) {
                    /* Process the entry if needed */
                    (void)etherent;
                }
                cleanup_file(fp);
            }
            remove("./dummy_file");
        }
    }

    return 0;
}