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
#include <unistd.h>
#include <errno.h>

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzzer input
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

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open file for reading
    fp = fopen("./dummy_file", "r");
    if (!fp) {
        unlink("./dummy_file");
        return;
    }
    
    // Read entries until EOF
    struct pcap_etherent *entry;
    while ((entry = pcap_next_etherent(fp)) != NULL) {
        // Entry is in static buffer, no need to free
    }
    
    fclose(fp);
    unlink("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated hostname from fuzzer input
    char *hostname = (char *)malloc(size + 1);
    if (!hostname) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    u_char *result = pcap_ether_hostton(hostname);
    if (result) {
        free(result);
    }
    
    free(hostname);
}

static void test_pcap_open_offline(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Try to open as pcap file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
    
    unlink("./dummy_file");
}

static void test_pcap_strerror(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    // Use fuzzer data to create error number
    int errnum;
    memcpy(&errnum, data, sizeof(int));
    
    const char *errstr = pcap_strerror(errnum);
    (void)errstr; // Use result to avoid dead code elimination
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Test all target functions with the same input
    test_pcap_ether_aton(Data, Size);
    test_pcap_next_etherent(Data, Size);
    test_pcap_ether_hostton(Data, Size);
    
    // pcap_lib_version has no input parameters
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid dead code elimination
    
    test_pcap_open_offline(Data, Size);
    test_pcap_strerror(Data, Size);
    
    return 0;
}