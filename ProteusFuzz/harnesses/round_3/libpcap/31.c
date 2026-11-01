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
#include <pcap.h>
#include <pcap/namedb.h>
#include <arpa/inet.h>

/* 
 * Note: pcap_nametoaddr returns bpf_u_int32 ** in namedb.h, not struct in_addr *.
 * We rely on the header declaration and do not redefine it.
 */

static void cleanup_ether_hostton(u_char *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void cleanup_ether_aton(u_char *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void cleanup_addrinfo(struct addrinfo *ptr) {
    if (ptr) {
        freeaddrinfo(ptr);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Fuzz pcap_ether_hostton
    u_char *ether_hostton_result = pcap_ether_hostton(input_str);
    cleanup_ether_hostton(ether_hostton_result);

    // 2. Fuzz pcap_nametonetaddr
    bpf_u_int32 netaddr = pcap_nametonetaddr(input_str);
    (void)netaddr; // Suppress unused variable warning

    // 3. Fuzz pcap_ether_aton
    u_char *ether_aton_result = pcap_ether_aton(input_str);
    cleanup_ether_aton(ether_aton_result);

    // 4. Fuzz pcap_next_etherent
    // Create a dummy file with the input data to simulate /etc/ethers
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(input_str, 1, Size, fp);
        fclose(fp);

        fp = fopen("./dummy_file", "r");
        if (fp) {
            struct pcap_etherent *etherent;
            // Call multiple times to explore different states (EOF, valid, invalid)
            for (int i = 0; i < 3; i++) {
                etherent = pcap_next_etherent(fp);
                (void)etherent; // Result is static, just call to trigger logic
            }
            fclose(fp);
        }
    }

    // 5. Fuzz pcap_nametoaddr (Deprecated but available for testing)
    // Returns bpf_u_int32 **, cast to void to suppress warnings if unused
    bpf_u_int32 **addr_list = pcap_nametoaddr(input_str);
    (void)addr_list;

    // 6. Fuzz pcap_nametoaddrinfo
    struct addrinfo *addrinfo_result = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(addrinfo_result);

    // Cleanup temporary file
    remove("./dummy_file");
    free(input_str);

    return 0;
}