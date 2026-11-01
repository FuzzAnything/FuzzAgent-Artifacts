// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_next_etherent at etherent.c:73:1 in namedb.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_ether_aton at nametoaddr.c:820:1 in namedb.h
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ether.h>
#include <pcap.h>
#include <namedb.h>

/* 
 * Static helper to create a temporary file with fuzzed content
 * for pcap_next_etherent testing.
 */
static int create_ether_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string-based APIs */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 
     * 1. Fuzz pcap_nametonetaddr 
     * Converts network name to numeric address.
     */
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr; /* Suppress unused variable warning */

    /* 
     * 2. Fuzz pcap_nametoaddr 
     * Resolves hostname to IP (deprecated, non-reentrant).
     */
    struct hostent *host = pcap_nametoaddr(input_str);
    if (host) {
        /* Just access to ensure no crash, do not store pointer long term */
        (void)host->h_addr;
    }

    /* 
     * 3. Fuzz pcap_nametoaddrinfo 
     * Resolves hostname to addrinfo.
     */
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    if (ai) {
        freeaddrinfo(ai);
    }

    /* 
     * 4. Fuzz pcap_ether_aton 
     * Converts hex MAC string to binary.
     */
    u_char *ether_addr = pcap_ether_aton(input_str);
    if (ether_addr) {
        /* Perform a dummy read to ensure memory is valid */
        (void)ether_addr[0];
        free(ether_addr);
    }

    /* 
     * 5. Fuzz pcap_ether_hostton 
     * Converts hostname to Ethernet address.
     */
    u_char *ether_host = pcap_ether_hostton(input_str);
    if (ether_host) {
        (void)ether_host[0];
        free(ether_host);
    }

    /* 
     * 6. Fuzz pcap_next_etherent 
     * Parses Ethernet address/name pairs from a file.
     */
    const char *ether_file = "./dummy_file";
    if (create_ether_file(Data, Size, ether_file) == 0) {
        FILE *fp = fopen(ether_file, "r");
        if (fp) {
            struct pcap_etherent *ent;
            /* Loop to consume all entries or until EOF */
            while ((ent = pcap_next_etherent(fp)) != NULL) {
                /* Access fields to ensure validity */
                (void)ent->addr[0];
                (void)ent->name[0];
            }
            fclose(fp);
        }
        /* Cleanup dummy file */
        remove(ether_file);
    }

    free(input_str);
    return 0;
}