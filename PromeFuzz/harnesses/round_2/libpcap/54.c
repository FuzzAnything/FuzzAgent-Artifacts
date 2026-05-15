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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    // Call target function
    u_char *result = pcap_ether_aton(str);
    
    // Clean up
    if (result) free(result);
    free(str);
}

static void test_pcap_lookupdev(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char *result = pcap_lookupdev(errbuf);
    // No cleanup needed - returns pointer to static buffer
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open file for reading
    fp = fopen("./dummy_file", "r");
    if (!fp) return;
    
    // Call function repeatedly until EOF
    struct pcap_etherent *entry;
    while ((entry = pcap_next_etherent(fp)) != NULL) {
        // Process entry if needed
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
    
    // Call target function
    u_char *result = pcap_ether_hostton(str);
    
    // Clean up
    if (result) free(result);
    free(str);
}

static void test_pcap_findalldevs(void) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    
    // Clean up
    if (alldevs) pcap_freealldevs(alldevs);
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    
    // Prepare source string from fuzzer input
    if (size == 0) return;
    
    char *source = malloc(size + 1);
    if (!source) return;
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    // Prepare auth structure
    auth.type = RPCAP_RMTAUTH_NULL;
    auth.username = NULL;
    auth.password = NULL;
    
    // If we have enough data, try with non-NULL auth
    if (size > 10) {
        auth.type = RPCAP_RMTAUTH_PWD;
        
        // Use first half for username, second half for password
        size_t split = size / 2;
        
        char *username = malloc(split + 1);
        char *password = malloc(size - split + 1);
        
        if (username && password) {
            memcpy(username, data, split);
            username[split] = '\0';
            
            memcpy(password, data + split, size - split);
            password[size - split] = '\0';
            
            auth.username = username;
            auth.password = password;
        }
        
        // Call function with auth
        int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
        
        // Clean up auth strings
        if (username) free(username);
        if (password) free(password);
        
        // Clean up device list
        if (alldevs) pcap_freealldevs(alldevs);
        alldevs = NULL;
    }
    
    // Also test with NULL auth
    int result = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
    
    // Clean up
    if (alldevs) pcap_freealldevs(alldevs);
    free(source);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Test each API function with the fuzzer input
    
    test_pcap_ether_aton(Data, Size);
    test_pcap_lookupdev();
    test_pcap_next_etherent(Data, Size);
    test_pcap_ether_hostton(Data, Size);
    test_pcap_findalldevs();
    test_pcap_findalldevs_ex(Data, Size);
    
    return 0;
}