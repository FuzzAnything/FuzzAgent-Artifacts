// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
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

static int is_valid_ether_string(const char *str, size_t len) {
    if (len < 17 || len > 17) return 0;
    
    int separators = 0;
    char sep = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (i % 3 == 2) {
            if (str[i] != ':' && str[i] != '.' && str[i] != '-') return 0;
            if (sep == 0) sep = str[i];
            else if (str[i] != sep) return 0;
            separators++;
        } else {
            char c = str[i];
            if (!((c >= '0' && c <= '9') || 
                  (c >= 'a' && c <= 'f') || 
                  (c >= 'A' && c <= 'F'))) return 0;
        }
    }
    return separators == 5;
}

static void test_pcap_strerror(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    int errnum = *(const int*)data;
    const char *result = pcap_strerror(errnum);
    (void)result;
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    if (is_valid_ether_string(str, size)) {
        u_char *result = pcap_ether_aton(str);
        if (result) free(result);
    }
    
    free(str);
}

static void test_pcap_lib_version(void) {
    const char *version = pcap_lib_version();
    (void)version;
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
        /* Do not free ent - it points to static memory managed by libpcap */
    }
    
    fclose(fp);
}

static void test_pcap_open_offline(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *hostname = malloc(size + 1);
    if (!hostname) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    u_char *result = pcap_ether_hostton(hostname);
    if (result) free(result);
    
    free(hostname);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_pcap_strerror(Data, Size);
    test_pcap_ether_aton(Data, Size);
    test_pcap_lib_version();
    test_pcap_next_etherent(Data, Size);
    test_pcap_open_offline(Data, Size);
    test_pcap_ether_hostton(Data, Size);
    
    return 0;
}