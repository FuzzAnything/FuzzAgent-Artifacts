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
#include <pcap.h>
#include <namedb.h>
#include <arpa/inet.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. pcap_nametonetaddr
    // Converts network name to numeric address
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr; // Suppress unused variable warning

    // 2. pcap_nametoaddr
    // Deprecated, non-reentrant. Returns pointer to static struct.
    // We call it to exercise the path, but do not rely on the result across calls.
    struct in_addr *addr_ptr = pcap_nametoaddr(input_str);
    if (addr_ptr != NULL) {
        // Just access to ensure it's valid, no deep logic needed for fuzzing
        (void)addr_ptr->s_addr;
    }

    // 3. pcap_nametoaddrinfo
    // Returns linked list of addrinfo, must be freed
    struct addrinfo *ai_list = pcap_nametoaddrinfo(input_str);
    if (ai_list != NULL) {
        freeaddrinfo(ai_list);
    }

    // 4. pcap_ether_aton
    // Converts hex MAC string to binary, returns malloc'd memory
    u_char *ether_addr = pcap_ether_aton(input_str);
    if (ether_addr != NULL) {
        free(ether_addr);
    }

    // 5. pcap_ether_hostton
    // Converts hostname to Ethernet address, returns malloc'd memory
    u_char *ether_host_addr = pcap_ether_hostton(input_str);
    if (ether_host_addr != NULL) {
        free(ether_host_addr);
    }

    // 6. pcap_next_etherent
    // Requires a FILE* pointing to an ethers file.
    // We create a dummy file with the fuzz input to test parsing.
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f != NULL) {
        // Write input to file to test parsing logic
        fwrite(input_str, 1, Size, f);
        fclose(f);

        // Reopen for reading
        f = fopen(dummy_file, "r");
        if (f != NULL) {
            struct pcap_etherent *ent;
            // Read a few entries to exercise the state machine
            for (int i = 0; i < 5; i++) {
                ent = pcap_next_etherent(f);
                if (ent == NULL) {
                    break;
                }
                // Access fields to ensure validity
                (void)ent->addr[0];
                (void)ent->name[0];
            }
            fclose(f);
        }
        // Clean up dummy file
        remove(dummy_file);
    }

    free(input_str);
    return 0;
}