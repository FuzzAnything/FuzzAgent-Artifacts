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
#include <unistd.h>

static void test_pcap_strerror(int err) {
    const char *err_str = pcap_strerror(err);
    (void)err_str; // Use result to avoid unused variable warning
}

static void test_pcap_lib_version(void) {
    const char *version = pcap_lib_version();
    (void)version; // Use result to avoid unused variable warning
}

static void test_pcap_open_offline(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    if (size > 0) {
        fwrite(data, 1, size, fp);
    }
    fclose(fp);
    
    // Try to open the file with libpcap
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
    
    // Clean up
    unlink("./dummy_file");
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    if (size > 0) {
        fwrite(data, 1, size, fp);
    }
    fclose(fp);
    
    // Open for reading
    fp = fopen("./dummy_file", "r");
    if (!fp) return;
    
    // Read ethernet entries until EOF
    struct pcap_etherent *etherent;
    while ((etherent = pcap_next_etherent(fp)) != NULL) {
        // Do not free the pointer - it's statically allocated by libpcap
        // Just access the data to exercise the code
        volatile u_char dummy = etherent->addr[0];
        volatile char dummy2 = etherent->name[0];
        (void)dummy;
        (void)dummy2;
    }
    
    fclose(fp);
    unlink("./dummy_file");
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    // Create null-terminated hostname string from fuzzer data
    char hostname[256];
    size_t copy_size = size < sizeof(hostname) - 1 ? size : sizeof(hostname) - 1;
    
    if (copy_size > 0) {
        memcpy(hostname, data, copy_size);
    }
    hostname[copy_size] = '\0';
    
    u_char *mac = pcap_ether_hostton(hostname);
    if (mac) {
        free(mac);
    }
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    // Create null-terminated string from fuzzer data
    char addr_str[256];
    size_t copy_size = size < sizeof(addr_str) - 1 ? size : sizeof(addr_str) - 1;
    
    if (copy_size > 0) {
        memcpy(addr_str, data, copy_size);
    }
    addr_str[copy_size] = '\0';
    
    // Basic validation to avoid obvious out-of-bounds reads
    // Count separators to ensure basic format
    int sep_count = 0;
    for (size_t i = 0; i < copy_size && addr_str[i] != '\0'; i++) {
        if (addr_str[i] == ':' || addr_str[i] == '.' || addr_str[i] == '-') {
            sep_count++;
        }
    }
    
    // Ethernet addresses should have 5 separators (6 octets)
    if (sep_count == 5) {
        u_char *mac = pcap_ether_aton(addr_str);
        if (mac) {
            free(mac);
        }
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Test pcap_strerror with various error codes
    if (Size > 0) {
        int err = Data[0] % 256; // Use first byte as error code
        test_pcap_strerror(err);
    }
    
    // Test pcap_lib_version
    test_pcap_lib_version();
    
    // Test pcap_open_offline
    test_pcap_open_offline(Data, Size);
    
    // Test pcap_next_etherent
    test_pcap_next_etherent(Data, Size);
    
    // Test pcap_ether_hostton
    test_pcap_ether_hostton(Data, Size);
    
    // Test pcap_ether_aton
    test_pcap_ether_aton(Data, Size);
    
    return 0;
}