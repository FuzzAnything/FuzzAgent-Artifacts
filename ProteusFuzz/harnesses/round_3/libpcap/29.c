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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <pcap.h>
#include <pcap/namedb.h>

static void dummy_file_init(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_etherent(FILE *f) {
    if (f) {
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *str_input = (char *)malloc(Size + 1);
    if (!str_input) {
        return 0;
    }
    memcpy(str_input, Data, Size);
    str_input[Size] = '\0';

    // 1. Fuzz pcap_ether_hostton
    u_char *ether_addr = pcap_ether_hostton(str_input);
    if (ether_addr) {
        free(ether_addr);
    }

    // 2. Fuzz pcap_nametonetaddr
    bpf_u_int32 net_addr = pcap_nametonetaddr(str_input);
    (void)net_addr; // Suppress unused warning

    // 3. Fuzz pcap_ether_aton
    u_char *ether_bin = pcap_ether_aton(str_input);
    if (ether_bin) {
        free(ether_bin);
    }

    // 4. Fuzz pcap_next_etherent
    // Create dummy file with input data
    dummy_file_init(Data, Size);
    FILE *ether_file = fopen("./dummy_file", "r");
    if (ether_file) {
        struct pcap_etherent *ent;
        while ((ent = pcap_next_etherent(ether_file)) != NULL) {
            // Consume result to avoid static buffer issues in fuzzing
            (void)ent->addr[0];
            (void)ent->name[0];
        }
        cleanup_etherent(ether_file);
    }

    // 5. Fuzz pcap_nametoaddr (deprecated but tested for coverage)
    struct hostent *host = pcap_nametoaddr(str_input);
    if (host) {
        // Do not use host->h_addr_list as it points to static memory
        // Just acknowledge it was called
    }

    // 6. Fuzz pcap_nametoaddrinfo
    struct addrinfo *addr_info = pcap_nametoaddrinfo(str_input);
    if (addr_info) {
        freeaddrinfo(addr_info);
    }

    free(str_input);
    return 0;
}