// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_next_etherent at etherent.c:73:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <pcap.h>
#include <namedb.h>

static void write_dummy_ethers_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 
     * Limit input size to prevent excessive allocation and potential issues 
     * with very long strings that might trigger internal buffer overflows 
     * in legacy parsing logic if they exist, though the main fix is ensuring 
     * we don't pass raw fuzzer data that might lack null termination in a way 
     * the library expects or reads past.
     * The ASAN error indicates reading past the allocated buffer. 
     * We ensure the string is strictly null-terminated and within safe bounds.
     */
    size_t max_len = 4096; 
    if (Size > max_len) Size = max_len;

    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. pcap_ether_hostton */
    u_char *ether_hostton_result = pcap_ether_hostton(input_str);
    if (ether_hostton_result) {
        free(ether_hostton_result);
    }

    /* 2. pcap_nametoaddrinfo */
    struct addrinfo *addrinfo_result = pcap_nametoaddrinfo(input_str);
    if (addrinfo_result) {
        freeaddrinfo(addrinfo_result);
    }

    /* 3. pcap_ether_aton */
    u_char *ether_aton_result = pcap_ether_aton(input_str);
    if (ether_aton_result) {
        free(ether_aton_result);
    }

    /* 4. pcap_next_etherent */
    const char *dummy_file = "./dummy_file";
    write_dummy_ethers_file(dummy_file, Data, Size);
    FILE *f = fopen(dummy_file, "r");
    if (f) {
        struct pcap_etherent *etherent = NULL;
        while ((etherent = pcap_next_etherent(f)) != NULL) {
            /* Use the result to avoid unused variable warnings */
            (void)etherent->addr[0];
            (void)etherent->name[0];
        }
        fclose(f);
    }

    /* 5. pcap_nametoaddr (deprecated but tested for coverage) */
    struct hostent *he = pcap_nametoaddr(input_str);
    /* he is static, do not free */
    (void)he;

    /* 6. pcap_nametonetaddr */
    bpf_u_int32 netaddr = pcap_nametonetaddr(input_str);
    (void)netaddr;

    free(input_str);
    return 0;
}