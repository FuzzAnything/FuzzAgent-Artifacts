// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_next_etherent at etherent.c:73:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pcap.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>

static void write_dummy_ether_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure null termination for string-based functions */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Fuzz pcap_ether_aton */
    u_char *ether_addr = pcap_ether_aton(input_str);
    if (ether_addr) {
        /* Do something with ether_addr to avoid unused variable warnings */
        volatile u_char dummy = ether_addr[0];
        (void)dummy;
        free(ether_addr);
    }

    /* 2. Fuzz pcap_nametoaddrinfo */
    struct addrinfo *addrinfo = pcap_nametoaddrinfo(input_str);
    if (addrinfo) {
        /* Use addrinfo briefly */
        volatile struct addrinfo *ai = addrinfo;
        (void)ai;
        freeaddrinfo(addrinfo);
    }

    /* 3. Fuzz pcap_ether_hostton */
    u_char *host_ether = pcap_ether_hostton(input_str);
    if (host_ether) {
        volatile u_char h = host_ether[0];
        (void)h;
        free(host_ether);
    }

    /* 4. Fuzz pcap_nametonetaddr */
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    volatile bpf_u_int32 na = net_addr;
    (void)na;

    /* 5. Fuzz pcap_next_etherent */
    /* Write input data to dummy file and try to parse */
    write_dummy_ether_file("./dummy_file", Data, Size);
    FILE *ether_file = fopen("./dummy_file", "r");
    if (ether_file) {
        struct pcap_etherent *etherent = pcap_next_etherent(ether_file);
        if (etherent) {
            volatile u_char ea = etherent->addr[0];
            (void)ea;
        }
        fclose(ether_file);
    }

    /* 6. Fuzz pcap_nametoaddr (deprecated, but still testable) */
    struct hostent *hostent = pcap_nametoaddr(input_str);
    if (hostent) {
        volatile char *h_name = hostent->h_name;
        (void)h_name;
    }

    free(input_str);
    return 0;
}