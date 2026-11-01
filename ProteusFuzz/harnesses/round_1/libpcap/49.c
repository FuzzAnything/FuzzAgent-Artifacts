// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
// pcap_next_etherent at etherent.c:73:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pcap.h>
#include <namedb.h>

static void cleanup_ether_aton(u_char *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void cleanup_ether_hostton(u_char *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void cleanup_addrinfo(struct addrinfo *ptr) {
    if (ptr) {
        freeaddrinfo(ptr);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. pcap_nametonetaddr
    // Converts network name to numeric address
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr;

    // 2. pcap_nametoaddr
    // Resolves hostname to IP addresses (deprecated, non-reentrant)
    // We cast to void* to suppress warnings about deprecated function usage if needed,
    // but calling it directly as per signature.
    struct in_addr *addr = pcap_nametoaddr(input_str);
    (void)addr;

    // 3. pcap_nametoaddrinfo
    // Resolves hostname to addrinfo
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(ai);

    // 4. pcap_ether_aton
    // Converts hex MAC string to binary
    u_char *ether_addr = pcap_ether_aton(input_str);
    cleanup_ether_aton(ether_addr);

    // 5. pcap_ether_hostton
    // Converts hostname to Ethernet address
    u_char *ether_host_addr = pcap_ether_hostton(input_str);
    cleanup_ether_hostton(ether_host_addr);

    // 6. pcap_next_etherent
    // Parses Ethernet address/name pairs from a file
    // Create a dummy file with some potential etherent data
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "w");
    if (fp) {
        // Write some data to the file, potentially including the fuzz input
        size_t write_size = (Size > 256) ? 256 : Size;
        fwrite(input_str, 1, write_size, fp);
        fclose(fp);
    }

    fp = fopen(dummy_file, "r");
    if (fp) {
        struct pcap_etherent *etherent = NULL;
        // Iterate through entries until EOF
        while ((etherent = pcap_next_etherent(fp)) != NULL) {
            (void)etherent; // Use the result to prevent unused variable warnings
        }
        fclose(fp);
    }

    // Cleanup dummy file
    remove(dummy_file);

    free(input_str);
    return 0;
}