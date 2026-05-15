// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/namedb.h"
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    u_char *result = pcap_ether_aton(str);
    if (result) {
        free(result);
    }
    
    free(str);
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "r");
    if (!fp) return;
    
    struct pcap_etherent *etherent;
    while ((etherent = pcap_next_etherent(fp)) != NULL) {
        // Just read entries, no processing needed
    }
    
    fclose(fp);
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    u_char *result = pcap_ether_hostton(str);
    if (result) {
        free(result);
    }
    
    free(str);
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

static void test_pcap_strerror(const uint8_t *data) {
    int errnum = 0;
    if (data) {
        // Use first byte as error number
        errnum = (int)(*data);
    }
    
    const char *errstr = pcap_strerror(errnum);
    (void)errstr; // Use result to avoid unused variable warning
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test pcap_ether_aton
    test_pcap_ether_aton(Data, Size);
    
    // Test pcap_next_etherent
    test_pcap_next_etherent(Data, Size);
    
    // Test pcap_ether_hostton
    test_pcap_ether_hostton(Data, Size);
    
    // Test pcap_lib_version (no input needed)
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid unused variable warning
    
    // Test pcap_open_offline
    test_pcap_open_offline(Data, Size);
    
    // Test pcap_strerror
    test_pcap_strerror(Data);
    
    return 0;
}