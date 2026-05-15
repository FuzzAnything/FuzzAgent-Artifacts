// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void test_pcap_geterr(void) {
    // Create a dummy pcap_t structure using pcap_open_dead
    pcap_t *p = pcap_open_dead(DLT_EN10MB, 65535);
    if (p != NULL) {
        // Call pcap_geterr - may return empty string or previous error
        char *err = pcap_geterr(p);
        (void)err; // Use variable to avoid unused warning
        pcap_close(p);
    }
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    // Ensure we have at least 17 bytes for a minimal MAC address (6*2 + 5 separators)
    if (size < 17) return;
    
    // Create a null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (str == NULL) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    // Replace any null bytes in the middle with '0' to keep string contiguous
    for (size_t i = 0; i < size; i++) {
        if (str[i] == '\0') str[i] = '0';
    }
    
    // Ensure the string looks like a MAC address (has colons)
    // Add some separators to increase chance of valid format
    if (size >= 12) {
        // Format as XX:XX:XX:XX:XX:XX
        for (int i = 2; i < 17 && i < (int)size; i += 3) {
            if (i < (int)size - 1) {
                str[i] = ':';
            }
        }
    }
    
    u_char *result = pcap_ether_aton(str);
    if (result != NULL) {
        free(result);
    }
    free(str);
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open file for reading
    fp = fopen("./dummy_file", "rb");
    if (fp == NULL) return;
    
    // Try to read entries until EOF
    struct pcap_etherent *etherent;
    while ((etherent = pcap_next_etherent(fp)) != NULL) {
        // Do nothing with the entry, just free if needed
        // Note: pcap_next_etherent returns a static buffer or allocated memory?
        // According to documentation, it returns a pointer to a static structure
        // or allocated memory. We don't need to free.
    }
    
    fclose(fp);
    remove("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    // Create null-terminated hostname string
    if (size == 0) return;
    
    char *hostname = malloc(size + 1);
    if (hostname == NULL) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    // Replace any null bytes in the middle
    for (size_t i = 0; i < size; i++) {
        if (hostname[i] == '\0') hostname[i] = 'a';
    }
    
    u_char *result = pcap_ether_hostton(hostname);
    if (result != NULL) {
        free(result);
    }
    free(hostname);
}

static void test_pcap_lib_version(void) {
    const char *version = pcap_lib_version();
    (void)version; // Use variable to avoid unused warning
}

static void test_pcap_strerror(const uint8_t *data, size_t size) {
    // Use fuzzer data to generate an errno value
    int errnum = 0;
    if (size >= sizeof(int)) {
        memcpy(&errnum, data, sizeof(int));
    } else if (size > 0) {
        errnum = data[0];
    }
    
    // Test with both positive and negative error numbers
    const char *errstr = pcap_strerror(errnum);
    (void)errstr;
    
    // Also test with some common error numbers
    const char *errstr2 = pcap_strerror(EINVAL);
    (void)errstr2;
    const char *errstr3 = pcap_strerror(ENOMEM);
    (void)errstr3;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to decide which functions to test
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size > 1 ? Size - 1 : 0;
    
    // Test all functions, but vary the order based on selector
    switch (selector % 6) {
        case 0:
            test_pcap_geterr();
            test_pcap_ether_aton(remaining_data, remaining_size);
            test_pcap_next_etherent(remaining_data, remaining_size);
            test_pcap_ether_hostton(remaining_data, remaining_size);
            test_pcap_lib_version();
            test_pcap_strerror(remaining_data, remaining_size);
            break;
        case 1:
            test_pcap_ether_aton(remaining_data, remaining_size);
            test_pcap_next_etherent(remaining_data, remaining_size);
            test_pcap_lib_version();
            test_pcap_strerror(remaining_data, remaining_size);
            test_pcap_geterr();
            test_pcap_ether_hostton(remaining_data, remaining_size);
            break;
        case 2:
            test_pcap_next_etherent(remaining_data, remaining_size);
            test_pcap_lib_version();
            test_pcap_strerror(remaining_data, remaining_size);
            test_pcap_geterr();
            test_pcap_ether_hostton(remaining_data, remaining_size);
            test_pcap_ether_aton(remaining_data, remaining_size);
            break;
        case 3:
            test_pcap_lib_version();
            test_pcap_strerror(remaining_data, remaining_size);
            test_pcap_geterr();
            test_pcap_ether_hostton(remaining_data, remaining_size);
            test_pcap_ether_aton(remaining_data, remaining_size);
            test_pcap_next_etherent(remaining_data, remaining_size);
            break;
        case 4:
            test_pcap_strerror(remaining_data, remaining_size);
            test_pcap_geterr();
            test_pcap_ether_hostton(remaining_data, remaining_size);
            test_pcap_ether_aton(remaining_data, remaining_size);
            test_pcap_next_etherent(remaining_data, remaining_size);
            test_pcap_lib_version();
            break;
        case 5:
            test_pcap_geterr();
            test_pcap_ether_hostton(remaining_data, remaining_size);
            test_pcap_lib_version();
            test_pcap_strerror(remaining_data, remaining_size);
            test_pcap_next_etherent(remaining_data, remaining_size);
            test_pcap_ether_aton(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}