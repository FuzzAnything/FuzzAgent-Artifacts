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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pcap.h>
#include <namedb.h>

/*
 * Fuzz driver for libpcap name resolution and conversion functions.
 * Target APIs:
 * 1. pcap_ether_aton
 * 2. pcap_nametoaddrinfo
 * 3. pcap_ether_hostton
 * 4. pcap_nametonetaddr
 * 5. pcap_next_etherent
 * 6. pcap_nametoaddr (deprecated)
 */

static void write_dummy_ethers_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "# Dummy /etc/ethers file for fuzzing\n");
    fprintf(f, "00:11:22:33:44:55  host1\n");
    fprintf(f, "aa:bb:cc:dd:ee:ff  host2\n");
    fprintf(f, "invalid line\n");
    fprintf(f, "11-22-33-44-55-66  host3\n");
    fprintf(f, "11.22.33.44.55.66  host4\n");
    fclose(f);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure null termination for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. pcap_ether_aton: Convert hex MAC string to binary */
    u_char *ether_addr = pcap_ether_aton(input_str);
    if (ether_addr) {
        /* Use the result to avoid unused warnings */
        (void)ether_addr[0];
        free(ether_addr);
    }

    /* 2. pcap_nametoaddrinfo: Resolve hostname to addrinfo */
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    if (ai) {
        /* Traverse the list to exercise memory */
        struct addrinfo *cur = ai;
        while (cur) {
            cur = cur->ai_next;
        }
        freeaddrinfo(ai);
    }

    /* 3. pcap_ether_hostton: Convert hostname to Ethernet address */
    u_char *host_ether = pcap_ether_hostton(input_str);
    if (host_ether) {
        (void)host_ether[0];
        free(host_ether);
    }

    /* 4. pcap_nametonetaddr: Convert network name to numeric address */
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr;

    /* 5. pcap_next_etherent: Parse Ethernet entries from a file */
    const char *dummy_file = "./dummy_file";
    write_dummy_ethers_file(dummy_file);
    FILE *f = fopen(dummy_file, "r");
    if (f) {
        struct pcap_etherent *ent;
        while ((ent = pcap_next_etherent(f)) != NULL) {
            (void)ent->addr[0];
            (void)ent->name[0];
        }
        fclose(f);
    }

    /* 6. pcap_nametoaddr: Legacy hostname to IP array (deprecated) */
    struct in_addr *addr_list = pcap_nametoaddr(input_str);
    if (addr_list) {
        /* Just access to avoid unused warning; do not free (static) */
        (void)addr_list[0].s_addr;
    }

    free(input_str);
    return 0;
}