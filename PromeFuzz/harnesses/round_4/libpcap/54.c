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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static void test_pcap_strerror(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int err;
    memcpy(&err, data, sizeof(int));
    
    const char *error_str = pcap_strerror(err);
    (void)error_str; // Use result to avoid optimization
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0 || size > 1024) return;
    
    // Create null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    // Basic validation to avoid obvious crashes
    // Check for null terminator in the middle
    for (size_t i = 0; i < size; i++) {
        if (str[i] == '\0') {
            free(str);
            return;
        }
    }
    
    u_char *result = pcap_ether_aton(str);
    if (result) {
        free(result);
    }
    
    free(str);
}

static void test_pcap_lib_version(void) {
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid optimization
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    
    // Read entries until NULL
    struct pcap_etherent *entry;
    while ((entry = pcap_next_etherent(fp)) != NULL) {
        // Do nothing with entry, just consume it
    }
    
    fclose(fp);
    remove("./dummy_file");
}

static void test_pcap_open_offline(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Prepare error buffer as required
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Try to open the file
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
    
    remove("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0 || size > 1024) return;
    
    // Create null-terminated string from fuzzer input
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    // Check for null terminator in the middle
    for (size_t i = 0; i < size; i++) {
        if (str[i] == '\0') {
            free(str);
            return;
        }
    }
    
    u_char *result = pcap_ether_hostton(str);
    if (result) {
        free(result);
    }
    
    free(str);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    
    // Adjust pointer and size for the remaining data
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = (Size > 1) ? Size - 1 : 0;
    
    switch (selector) {
        case 0:
            test_pcap_strerror(remaining_data, remaining_size);
            break;
        case 1:
            test_pcap_ether_aton(remaining_data, remaining_size);
            break;
        case 2:
            test_pcap_lib_version();
            break;
        case 3:
            test_pcap_next_etherent(remaining_data, remaining_size);
            break;
        case 4:
            test_pcap_open_offline(remaining_data, remaining_size);
            break;
        case 5:
            test_pcap_ether_hostton(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}