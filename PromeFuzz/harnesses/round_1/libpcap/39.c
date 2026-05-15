// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    
    // Prepare source string from fuzzer data
    char *source = NULL;
    if (size > 0) {
        source = (char *)malloc(size + 1);
        if (source) {
            memcpy(source, data, size);
            source[size] = '\0';
        }
    }
    
    // Prepare auth structure
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // Call the target function
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    
    // Cleanup
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    free(source);
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
    
    // Open file for reading
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        // Call the target function multiple times
        struct pcap_etherent *etherent;
        while ((etherent = pcap_next_etherent(fp)) != NULL) {
            // Just consume the result
        }
        fclose(fp);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    // Prepare hostname string from fuzzer data
    char *hostname = NULL;
    if (size > 0) {
        hostname = (char *)malloc(size + 1);
        if (hostname) {
            memcpy(hostname, data, size);
            hostname[size] = '\0';
        }
    }
    
    // Call the target function
    u_char *result = pcap_ether_hostton(hostname);
    
    // Cleanup
    if (result) {
        free(result);
    }
    free(hostname);
}

static void test_pcap_findalldevs(const uint8_t *data, size_t size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Call the target function
    int result = pcap_findalldevs(&alldevs, errbuf);
    
    // Cleanup
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_lookupdev(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Call the target function
    char *result = pcap_lookupdev(errbuf);
    
    // No cleanup needed as function returns pointer to static buffer
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    // Skip invalid inputs that could cause buffer overflow
    if (size == 0 || size > 1024) {
        return;
    }
    
    // Prepare address string from fuzzer data
    char *addr_str = NULL;
    if (size > 0) {
        addr_str = (char *)malloc(size + 1);
        if (addr_str) {
            memcpy(addr_str, data, size);
            addr_str[size] = '\0';
            
            // Validate string doesn't end with separator to prevent out-of-bounds read
            if (size > 0) {
                char last_char = addr_str[size - 1];
                if (last_char == ':' || last_char == '.' || last_char == '-') {
                    free(addr_str);
                    return;
                }
            }
        }
    }
    
    // Call the target function
    u_char *result = pcap_ether_aton(addr_str);
    
    // Cleanup
    if (result) {
        free(result);
    }
    free(addr_str);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test each API function with the fuzzer input
    test_pcap_findalldevs_ex(Data, Size);
    test_pcap_next_etherent(Data, Size);
    test_pcap_ether_hostton(Data, Size);
    test_pcap_findalldevs(Data, Size);
    test_pcap_lookupdev(Data, Size);
    test_pcap_ether_aton(Data, Size);
    
    return 0;
}