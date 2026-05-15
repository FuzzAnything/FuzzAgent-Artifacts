// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *str = (char *)malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    u_char *result = pcap_ether_aton(str);
    if (result) {
        free(result);
    }
    
    free(str);
}

static void test_pcap_lookupdev(void) {
    char *dev = pcap_lookupdev(NULL);
    (void)dev;
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    
    struct pcap_etherent *ent;
    while ((ent = pcap_next_etherent(fp)) != NULL) {
        (void)ent;
    }
    
    fclose(fp);
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *str = (char *)malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    u_char *result = pcap_ether_hostton(str);
    if (result) {
        free(result);
    }
    
    free(str);
}

static void test_pcap_findalldevs(void) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *source = (char *)malloc(size + 1);
    if (!source) return;
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    
    auth.type = RPCAP_RMTAUTH_NULL;
    auth.username = NULL;
    auth.password = NULL;
    
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    free(source);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_pcap_ether_aton(Data, Size);
    test_pcap_lookupdev();
    test_pcap_next_etherent(Data, Size);
    test_pcap_ether_hostton(Data, Size);
    test_pcap_findalldevs();
    test_pcap_findalldevs_ex(Data, Size);
    
    return 0;
}