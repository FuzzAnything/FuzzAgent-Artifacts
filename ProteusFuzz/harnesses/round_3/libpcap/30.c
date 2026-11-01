// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_next_etherent at etherent.c:73:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>

static void cleanup_etherent(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void cleanup_addrinfo(struct addrinfo *ptr) {
    if (ptr) {
        freeaddrinfo(ptr);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string-based APIs
    // Allocate Size + 1 to safely hold the null terminator
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    
    // Copy data and ensure null termination
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test pcap_ether_hostton
    u_char *ether_hostton_result = pcap_ether_hostton(input_str);
    cleanup_etherent(ether_hostton_result);

    // 2. Test pcap_nametonetaddr
    bpf_u_int32 netaddr = pcap_nametonetaddr(input_str);
    (void)netaddr; // Use the return value to avoid unused warnings

    // 3. Test pcap_ether_aton
    u_char *ether_aton_result = pcap_ether_aton(input_str);
    cleanup_etherent(ether_aton_result);

    // 4. Test pcap_next_etherent
    // Create a dummy file with some content to parse
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        // Write input data to dummy file
        fwrite(input_str, 1, Size, fp);
        fclose(fp);

        fp = fopen("./dummy_file", "r");
        if (fp) {
            struct pcap_etherent *etherent = NULL;
            // Call multiple times to explore state
            for (int i = 0; i < 5; i++) {
                etherent = pcap_next_etherent(fp);
                // No cleanup needed for pcap_etherent as it returns static data
                (void)etherent;
            }
            fclose(fp);
        }
    }

    // 5. Test pcap_nametoaddr (Deprecated but tested for coverage)
    struct hostent *hostent_result = pcap_nametoaddr(input_str);
    (void)hostent_result;

    // 6. Test pcap_nametoaddrinfo
    struct addrinfo *addrinfo_result = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(addrinfo_result);

    free(input_str);
    return 0;
}