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
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pcap.h>
#include <pcap/namedb.h>

static void safe_free(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void safe_freeaddrinfo(struct addrinfo *ptr) {
    if (ptr) {
        freeaddrinfo(ptr);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Fuzz pcap_ether_hostton */
    /* Converts hostname to Ethernet address. Allocates memory. */
    u_char *ether_addr = pcap_ether_hostton(input_str);
    safe_free(ether_addr);

    /* 2. Fuzz pcap_nametoaddrinfo */
    /* Resolves hostname to addrinfo. Returns linked list. */
    struct addrinfo *addr_info = pcap_nametoaddrinfo(input_str);
    safe_freeaddrinfo(addr_info);

    /* 3. Fuzz pcap_ether_aton */
    /* Converts hex MAC string to binary. Allocates memory. */
    u_char *ether_bin = pcap_ether_aton(input_str);
    safe_free(ether_bin);

    /* 4. Fuzz pcap_next_etherent */
    /* Parses Ethernet entries from a file. Requires FILE*. */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "w");
    if (fp) {
        /* Write some potential etherent data or garbage */
        fwrite(input_str, 1, Size, fp);
        fclose(fp);

        fp = fopen(dummy_file, "r");
        if (fp) {
            struct pcap_etherent *etherent;
            /* Loop to consume entries or until NULL */
            while ((etherent = pcap_next_etherent(fp)) != NULL) {
                /* Access fields to trigger any potential issues */
                volatile u_char addr = etherent->addr[0];
                volatile char name = etherent->name[0];
                (void)addr;
                (void)name;
            }
            fclose(fp);
        }
        /* Cleanup dummy file */
        unlink(dummy_file);
    }

    /* 5. Fuzz pcap_nametoaddr */
    /* Legacy hostname to IP array. Non-reentrant. */
    struct in_addr *addr_list = pcap_nametoaddr(input_str);
    if (addr_list) {
        /* Just access to ensure validity, no cleanup needed (static) */
        volatile bpf_u_int32 ip = addr_list->s_addr;
        (void)ip;
    }

    /* 6. Fuzz pcap_nametonetaddr */
    /* Converts network name to numeric address. */
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr;

    safe_free(input_str);
    return 0;
}