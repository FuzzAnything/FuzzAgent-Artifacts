// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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
#include <unistd.h>

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    // Create null-terminated string from fuzz data
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

static void test_pcap_lookupdev(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char *result = pcap_lookupdev(errbuf);
    (void)result; // Result is static buffer, don't free
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    
    struct pcap_etherent *entry;
    while ((entry = pcap_next_etherent(fp)) != NULL) {
        // Process entry if needed
    }
    
    fclose(fp);
    unlink("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
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

static void test_pcap_findalldevs(void) {
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    
    // Always allocate source string, never pass NULL
    char *source = malloc(size + 1);
    if (!source) return;
    
    if (size > 0) {
        memcpy(source, data, size);
    }
    source[size] = '\0';  // Ensure null termination even for empty string
    
    // Set up auth structure
    auth.type = RPCAP_RMTAUTH_NULL;
    auth.username = NULL;
    auth.password = NULL;
    
    // Try with NULL auth
    int result = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
    if (result == 0) {
        pcap_freealldevs(alldevs);
    }
    
    // Try with auth structure
    result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (result == 0) {
        pcap_freealldevs(alldevs);
    }
    
    free(source);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte to select which function to test
    unsigned char selector = data[0] % 6;
    const uint8_t *fuzz_data = data + 1;
    size_t fuzz_size = size - 1;
    
    switch (selector) {
        case 0:
            test_pcap_ether_aton(fuzz_data, fuzz_size);
            break;
        case 1:
            test_pcap_lookupdev();
            break;
        case 2:
            test_pcap_next_etherent(fuzz_data, fuzz_size);
            break;
        case 3:
            test_pcap_ether_hostton(fuzz_data, fuzz_size);
            break;
        case 4:
            test_pcap_findalldevs();
            break;
        case 5:
            test_pcap_findalldevs_ex(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}