// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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
#include <stdint.h>

static int is_valid_mac_string(const char *str, size_t max_len) {
    if (str == NULL || max_len < 17) return 0;
    
    size_t len = strnlen(str, max_len);
    if (len != 17) return 0;
    
    char sep = str[2];
    if (sep != ':' && sep != '.' && sep != '-') return 0;
    
    for (size_t i = 0; i < len; i++) {
        if ((i + 1) % 3 == 0) {
            if (str[i] != sep) return 0;
        } else {
            char c = str[i];
            if (!((c >= '0' && c <= '9') || 
                  (c >= 'a' && c <= 'f') || 
                  (c >= 'A' && c <= 'F'))) return 0;
        }
    }
    return 1;
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_if_t *alldevs = NULL;
    
    const char *source = "rpcap://";
    struct pcap_rmtauth auth;
    auth.type = RPCAP_RMTAUTH_NULL;
    auth.username = NULL;
    auth.password = NULL;
    
    pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size < 18) return;
    
    char mac_str[18];
    memcpy(mac_str, data, 17);
    mac_str[17] = '\0';
    
    if (is_valid_mac_string(mac_str, 18)) {
        u_char *mac = pcap_ether_aton(mac_str);
        if (mac != NULL) {
            free(mac);
        }
    }
}

static void test_pcap_lookupdev(void) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char *dev = pcap_lookupdev(errbuf);
    (void)dev;
}

static void test_pcap_findalldevs(void) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_if_t *alldevs = NULL;
    
    int ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *hostname = malloc(size + 1);
    if (hostname == NULL) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    u_char *mac = pcap_ether_hostton(hostname);
    if (mac != NULL) {
        free(mac);
    }
    free(hostname);
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "rb");
    if (fp == NULL) return;
    
    struct pcap_etherent *ent;
    while ((ent = pcap_next_etherent(fp)) != NULL) {
        (void)ent;
    }
    
    fclose(fp);
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_pcap_findalldevs_ex(Data, Size);
    test_pcap_ether_aton(Data, Size);
    test_pcap_lookupdev();
    test_pcap_findalldevs();
    test_pcap_ether_hostton(Data, Size);
    test_pcap_next_etherent(Data, Size);
    
    return 0;
}