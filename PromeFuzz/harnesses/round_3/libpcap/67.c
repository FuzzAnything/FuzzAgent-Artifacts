// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
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
#include <stdint.h>

static void cleanup_pcap_handle(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_file(FILE *fp) {
    if (fp) {
        fclose(fp);
    }
}

static void cleanup_memory(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split input for different functions
    size_t split = Size / 2;
    const uint8_t *first_part = Data;
    size_t first_size = split;
    const uint8_t *second_part = Data + split;
    size_t second_size = Size - split;

    // Create null-terminated strings from input
    char *str1 = malloc(first_size + 1);
    char *str2 = malloc(second_size + 1);
    if (!str1 || !str2) {
        cleanup_memory(str1);
        cleanup_memory(str2);
        return 0;
    }
    
    memcpy(str1, first_part, first_size);
    str1[first_size] = '\0';
    memcpy(str2, second_part, second_size);
    str2[second_size] = '\0';

    // 1. Test pcap_lookupdev (deprecated but still callable)
    char errbuf1[PCAP_ERRBUF_SIZE];
    char *dev = pcap_lookupdev(errbuf1);
    (void)dev; // Result is volatile, just call it

    // 2. Test pcap_ether_hostton
    u_char *mac1 = pcap_ether_hostton(str1);
    cleanup_memory(mac1);

    // 3. Test pcap_ether_aton with validation
    // Check if string looks vaguely like a MAC address
    int looks_like_mac = 0;
    if (strlen(str1) >= 12 && strlen(str1) <= 17) {
        // Count separators
        int colons = 0, dots = 0, dashes = 0;
        for (size_t i = 0; str1[i]; i++) {
            if (str1[i] == ':') colons++;
            else if (str1[i] == '.') dots++;
            else if (str1[i] == '-') dashes++;
        }
        // Must have exactly 5 separators of one type
        if (colons == 5 || dots == 5 || dashes == 5) {
            looks_like_mac = 1;
        }
    }
    
    if (looks_like_mac) {
        u_char *mac2 = pcap_ether_aton(str1);
        cleanup_memory(mac2);
    }

    // 4. Test pcap_lookupnet
    bpf_u_int32 net, mask;
    char errbuf2[PCAP_ERRBUF_SIZE];
    int ret = pcap_lookupnet(str2, &net, &mask, errbuf2);
    (void)ret;

    // 5. Test pcap_open with various configurations
    char errbuf3[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth = {0};
    
    // Try with "any" device
    pcap_t *handle1 = pcap_open("any", 65535, PCAP_OPENFLAG_PROMISCUOUS, 
                                1000, NULL, errbuf3);
    cleanup_pcap_handle(handle1);
    
    // Try with string from input
    pcap_t *handle2 = pcap_open(str2, 1514, 0, 100, &auth, errbuf3);
    cleanup_pcap_handle(handle2);
    
    // Try with file source
    if (write_dummy_file(Data, Size) == 0) {
        pcap_t *handle3 = pcap_open("file:./dummy_file", 65535, 0, 
                                    0, NULL, errbuf3);
        cleanup_pcap_handle(handle3);
        remove("./dummy_file");
    }

    // 6. Test pcap_next_etherent
    if (write_dummy_file(Data, Size) == 0) {
        FILE *fp = fopen("./dummy_file", "r");
        if (fp) {
            struct pcap_etherent *etherent;
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                // Just iterate through entries
            }
            cleanup_file(fp);
        }
        remove("./dummy_file");
    }

    cleanup_memory(str1);
    cleanup_memory(str2);
    return 0;
}